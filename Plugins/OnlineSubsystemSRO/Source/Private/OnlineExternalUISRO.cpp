// Copyright Epic Games, Inc. All Rights Reserved.

#include "OnlineExternalUISRO.h"

#include "HttpModule.h"
#include "ILoginFlowManager.h"
#include "ILoginFlowModule.h"
#include "OnlineSubsystemSRO.h"
#include "OnlineIdentitySRO.h"
#include "OnlineError.h"
#include "Interfaces/IHttpResponse.h"
#include "Interfaces/OnlineExternalUIInterface.h"
#include "Kismet/GameplayStatics.h"

#define SRO_STATE_TOKEN TEXT("state")
#define SRO_ACCESS_TOKEN TEXT("code")
#define SRO_ERRORCODE_TOKEN TEXT("error")
#define SRO_ERRORCODE_DENY TEXT("access_denied")

FOnlineExternalUISRO::FOnlineExternalUISRO(FOnlineSubsystemSRO* InSubsystem):
	SROSubsystem(InSubsystem)
{
	if (!GConfig->GetString(TEXT("OnlineSubsystemSRO.OnlineExternalUISRO"), TEXT("EOSClientId"), EOSClientId, GEngineIni))
	{
		UE_LOG_ONLINE_IDENTITY(Warning, TEXT("Missing EOSClientId= in [OnlineSubsystemSRO.OnlineExternalUISRO] of DefaultEngine.ini"));
	}
	if (!GConfig->GetString(TEXT("OnlineSubsystemSRO.OnlineExternalUISRO"), TEXT("EOSClientSecret"), EOSClientSecret, GEngineIni))
	{
		UE_LOG_ONLINE_IDENTITY(Warning, TEXT("Missing EOSClientSecret= in [OnlineSubsystemSRO.OnlineExternalUISRO] of DefaultEngine.ini"));
	}
}

bool FOnlineExternalUISRO::ShowLoginUI(const int ControllerIndex, bool bShowOnlineOnly, bool bShowSkipButton, const FOnLoginUIClosedDelegate& Delegate)
{
	FString ErrorStr;
	bool bStarted = false;
	if (ControllerIndex >= 0 && ControllerIndex < MAX_LOCAL_PLAYERS)
	{
		FOnlineIdentitySROPtr IdentityInt = StaticCastSharedPtr<FOnlineIdentitySRO>(SROSubsystem->GetIdentityInterface());
		;
		if (IdentityInt.IsValid())
		{
			IdentityInt->RetrieveDiscoveryDocument([this, ControllerIndex, Delegate](bool bWasSuccessful)
				{
					FString ErrorStr;
					if (bWasSuccessful)
					{
						FOnlineIdentitySROPtr IdentityInt = StaticCastSharedPtr<FOnlineIdentitySRO>(SROSubsystem->GetIdentityInterface());
						IdentityInt->UpdateLoginURLWithEndpointData();
						const FSROLoginURL& URLDetails = IdentityInt->GetLoginURLDetails();
						
						if (URLDetails.IsValid())
						{
							FString URL = URLDetails.GetURL();

							bool bShouldContinueLoginFlow;
							FOnLoginRedirectURL OnRedirectURLDelegate = FOnLoginRedirectURL::CreateRaw(this, &FOnlineExternalUISRO::OnLoginRedirectURL);
							FOnLoginFlowComplete OnExternalLoginFlowCompleteDelegate = FOnLoginFlowComplete::CreateRaw(this, &FOnlineExternalUISRO::OnExternalLoginFlowComplete, ControllerIndex, Delegate);
							TriggerOnLoginFlowUIRequiredDelegates(URL, OnRedirectURLDelegate, OnExternalLoginFlowCompleteDelegate, bShouldContinueLoginFlow);
							if (bShouldContinueLoginFlow)
							{
								return;
							}
							else
							{
								ErrorStr = TEXT("ShowLoginUI: Login Flow interrupted");
							}
						}
						else
						{
							ErrorStr = TEXT("ShowLoginUI: Url Details not properly configured");
						}
					}
					else
					{
						ErrorStr = TEXT("ShowLoginUI: Could not retrieve Discovery Document");
					}

					FOnlineError Error;
					Error.SetFromErrorCode(MoveTemp(ErrorStr));

					SROSubsystem->ExecuteNextTick([ControllerIndex, Delegate, Error = MoveTemp(Error)]()
						{
							Delegate.ExecuteIfBound(nullptr, ControllerIndex, Error);
						});
				});
				bStarted = true;
		}
		else
		{
			ErrorStr = TEXT("ShowLoginUI: Missing identity interface");
		}
	}
	else
	{
		ErrorStr = FString::Printf(TEXT("ShowLoginUI: Invalid controller index (%d)"), ControllerIndex);
	}

	if (!bStarted)
	{
		UE_LOG_ONLINE_EXTERNALUI(Warning, TEXT("%s"), *ErrorStr);

		FOnlineError Error;
		Error.SetFromErrorCode(MoveTemp(ErrorStr));

		SROSubsystem->ExecuteNextTick([ControllerIndex, Delegate, Error = MoveTemp(Error)]()
		{
			Delegate.ExecuteIfBound(nullptr, ControllerIndex, Error);
		});
	}

	return bStarted;
}


FLoginFlowResult FOnlineExternalUISRO::OnLoginRedirectURL(const FString& RedirectURL)
{
	FLoginFlowResult Result;
	
	FOnlineIdentitySROPtr IdentityInt = StaticCastSharedPtr<FOnlineIdentitySRO>(SROSubsystem->GetIdentityInterface());
	if (IdentityInt.IsValid())
	{  
		const FSROLoginURL& URLDetails = IdentityInt->GetLoginURLDetails();
		if (URLDetails.IsValid())
		{
			// Wait for the RedirectURI to appear
			if (!RedirectURL.Contains(FPlatformHttp::UrlEncode(URLDetails.LoginUrl)) && RedirectURL.StartsWith(URLDetails.LoginRedirectUrl))
			{
				TMap<FString, FString> ParamsMap;
				{
					FString URLPrefix;
					FString ParamsOnly;
					if (!RedirectURL.Split(TEXT("?"), &URLPrefix, &ParamsOnly))
					{
						ParamsOnly = RedirectURL;
					}

					if (ParamsOnly[ParamsOnly.Len() - 1] == TEXT('#'))
					{
						ParamsOnly[ParamsOnly.Len() - 1] = TEXT('\0');
					}

					TArray<FString> Params;
					ParamsOnly.ParseIntoArray(Params, TEXT("&"));
					for (FString& Param : Params)
					{
						FString Key, Value;
						if (Param.Split(TEXT("="), &Key, &Value))
						{
							ParamsMap.Add(Key, Value);
						}
					}
				}

				const FString* AccessToken = ParamsMap.Find(SRO_ACCESS_TOKEN);
				if (AccessToken)
				{
					Result.Error.bSucceeded = true;
					Result.Token = *AccessToken;
				}
				else
				{
					const FString* ErrorCode = ParamsMap.Find(SRO_ERRORCODE_TOKEN);
					if (ErrorCode)
					{
						if (*ErrorCode == SRO_ERRORCODE_DENY)
						{
							Result.Error.ErrorRaw = LOGIN_CANCELLED;
							Result.Error.ErrorMessage = FText::FromString(LOGIN_CANCELLED);
							Result.Error.ErrorCode = LOGIN_CANCELLED;
							Result.Error.ErrorMessage = NSLOCTEXT("SROAuth", "SROAuthDeny", "SRO Auth Denied");
							Result.NumericErrorCode = -1;
						}
						else
						{
							Result.Error.ErrorRaw = RedirectURL;
							Result.Error.ErrorCode = *ErrorCode;
							// there is no descriptive error text
							Result.Error.ErrorMessage = NSLOCTEXT("SROAuth", "SROAuthError", "SRO Auth Error");
							// there is no error code
							Result.NumericErrorCode = 0;
						}
					}
					else
					{
						// Set some default in case parsing fails
						Result.Error.ErrorRaw = LOGIN_ERROR_UNKNOWN;
						Result.Error.ErrorMessage = FText::FromString(LOGIN_ERROR_UNKNOWN);
						Result.Error.ErrorCode = LOGIN_ERROR_UNKNOWN;
						Result.NumericErrorCode = -2;
					}
				}
			}
		}
	}

	return Result;
}

void FOnlineExternalUISRO::OnExternalLoginFlowComplete(const FLoginFlowResult& Result, int ControllerIndex, const FOnLoginUIClosedDelegate Delegate)
{
	UE_LOG_ONLINE_EXTERNALUI(Log, TEXT("OnExternalLoginFlowComplete %s"), *Result.ToDebugString());

	bool bStarted = false;
	if (Result.IsValid())
	{
		FOnlineIdentitySROPtr IdentityInt = StaticCastSharedPtr<FOnlineIdentitySRO>(SROSubsystem->GetIdentityInterface());
		if (IdentityInt.IsValid())
		{
			bStarted = true;

			FOnLoginCompleteDelegate CompletionDelegate;
			CompletionDelegate = FOnLoginCompleteDelegate::CreateRaw(this, &FOnlineExternalUISRO::OnAccessTokenLoginComplete, Delegate);

			FAuthTokenSRO AuthToken(Result.Token, ESROExchangeToken::SROExchangeToken);
			IdentityInt->Login(ControllerIndex, AuthToken, CompletionDelegate);
		}
	}

	if (!bStarted)
	{
		FOnlineError LoginFlowError = Result.Error;
		SROSubsystem->ExecuteNextTick([ControllerIndex, LoginFlowError, Delegate]()
		{
			Delegate.ExecuteIfBound(nullptr, ControllerIndex, LoginFlowError);
		});
	}
}

void FOnlineExternalUISRO::OnAccessTokenLoginComplete(int32 LocalUserNum, bool bWasSuccessful, const FUniqueNetId& UserId, const FString& Error, FOnLoginUIClosedDelegate Delegate)
{
	FUniqueNetIdPtr StrongUserId = UserId.AsShared();
	SROSubsystem->ExecuteNextTick([StrongUserId, LocalUserNum, bWasSuccessful, Delegate]()
	{
		Delegate.ExecuteIfBound(StrongUserId, LocalUserNum, FOnlineError(bWasSuccessful));
	});
}

bool FOnlineExternalUISRO::ShowFriendsUI(int32 LocalUserNum)
{
	return false;
}

bool FOnlineExternalUISRO::ShowInviteUI(int32 LocalUserNum, FName SessionName)
{
	return false;
}

bool FOnlineExternalUISRO::ShowAchievementsUI(int32 LocalUserNum)
{
	return false;
}

bool FOnlineExternalUISRO::ShowLeaderboardUI( const FString& LeaderboardName )
{
	return false;
}

bool FOnlineExternalUISRO::ShowWebURL(const FString& Url, const FShowWebUrlParams& ShowParams, const FOnShowWebUrlClosedDelegate& Delegate)
{
	return false;
}

bool FOnlineExternalUISRO::CloseWebURL()
{
	return false;
}

bool FOnlineExternalUISRO::ShowAccountUpgradeUI(const FUniqueNetId& UniqueId)
{
	return false;
}

bool FOnlineExternalUISRO::ShowStoreUI(int32 LocalUserNum, const FShowStoreParams& ShowParams, const FOnShowStoreUIClosedDelegate& Delegate)
{
	return false;
}

bool FOnlineExternalUISRO::ShowSendMessageUI(int32 LocalUserNum, const FShowSendMessageParams& ShowParams, const FOnShowSendMessageUIClosedDelegate& Delegate)
{
	return false;
}

bool FOnlineExternalUISRO::ShowProfileUI(const FUniqueNetId& Requestor, const FUniqueNetId& Requestee, const FOnProfileUIClosedDelegate& Delegate)
{
	return false;
}
