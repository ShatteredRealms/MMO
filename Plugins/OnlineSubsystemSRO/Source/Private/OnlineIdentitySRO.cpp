// Copyright Epic Games, Inc. All Rights Reserved.

#include "OnlineIdentitySRO.h"

#include "FOnlineAsyncTaskSRORefreshAuth.h"
#include "OnlineSubsystemSROPrivate.h"
#include "OnlineExternalUISRO.h"
#include "Misc/ConfigCacheIni.h"
#include "OnlineSubsystemSROTypes.h"
#include "OnlineSubsystemSRO.h"
#include "OnlineError.h"
#include "HttpModule.h"
#include "OnlineAsyncTaskManager.h"
#include "OnlineAsyncTaskManagerSRO.h"
#include "SROLoginWindow.h"
#include "Interfaces/IHttpResponse.h"
#include "Misc/ConfigCacheIni.h"
#include "Misc/Base64.h"
#include "AuthTypes.h"

void FOnlineIdentitySRO::AddUser(TSharedRef<FUserOnlineAccountSRO> User, int32 LocalUserNum, bool bAsyncUpdateAuth)
{
	// update/add cached entry for user
	UserAccounts.Add(User->GetUserId()->ToString(), User);
	// keep track of user ids for local users
	UserIds.Add(LocalUserNum, User->GetUserId());

	if (bAsyncUpdateAuth)
	{
		RegisterAsyncTokenUpdate(LocalUserNum);
	}
}

void FOnlineIdentitySRO::UpdateUserAuth(int32 LocalUserNum, const FAuthTokenSRO& InAuthToken, bool bAsyncUpdateAuth)
{
	const FUniqueNetIdPtr* UserId = UserIds.Find(LocalUserNum);
	check(UserId);

	const TSharedRef<FUserOnlineAccountSRO>* Account = UserAccounts.Find((*UserId)->ToString());
	check(Account);

	(*Account)->UpdateAuthToken(InAuthToken);
	
	if (bAsyncUpdateAuth)
	{
		RegisterAsyncTokenUpdate(LocalUserNum);
	}
}

FOnlineIdentitySRO::FOnlineIdentitySRO(FOnlineSubsystemSRO* InSubsystem)
	: SROSubsystem(InSubsystem), 
	bHasLoginOutstanding(false)
{
#if WITH_SERVER_CODE
	if(!FParse::Value(FCommandLine::Get(), TEXT("ServerClientId="), ServerClientId))
	{
		UE_LOG_ONLINE_IDENTITY(Log, TEXT("Missing ServerClientId= in command line"));
	}
	if(!FParse::Value(FCommandLine::Get(), TEXT("ServerClientSecret="), ServerClientSecret))
	{
		UE_LOG_ONLINE_IDENTITY(Log, TEXT("Missing ServerClientSecret= in command line. Checking environment variables."));
		FString Secret = FPlatformMisc::GetEnvironmentVariable(TEXT("SRO_AUTH_CLIENT_SECRET"));
		if (!Secret.IsEmpty())
		{
			ServerClientSecret = Secret;
		}
		else
		{
			UE_LOG_ONLINE_IDENTITY(Warning, TEXT("Missing SRO_AUTH_CLIENT_SECRET environment variable. No default secret is set. Login will fail."));
		}
	}
#endif
	
	if (!GConfig->GetString(TEXT("OnlineSubsystemSRO.OnlineIdentitySRO"), TEXT("LoginRedirectUrl"), LoginURLDetails.LoginRedirectUrl, GEngineIni))
	{
		UE_LOG_ONLINE_IDENTITY(Warning, TEXT("Missing LoginRedirectUrl= in [OnlineSubsystemSRO.OnlineIdentitySRO] of DefaultEngine.ini"));
	}
	if (!GConfig->GetInt(TEXT("OnlineSubsystemSRO.OnlineIdentitySRO"), TEXT("RedirectPort"), LoginURLDetails.RedirectPort, GEngineIni))
	{
		UE_LOG_ONLINE_IDENTITY(Warning, TEXT("Missing RedirectPort= in [OnlineSubsystemSRO.OnlineIdentitySRO] of DefaultEngine.ini"));
	}
	if (!GConfig->GetString(TEXT("OnlineSubsystemSRO.OnlineIdentitySRO"), TEXT("DiscoveryUrl"), Endpoints.DiscoveryUrl, GEngineIni))
	{
		UE_LOG_ONLINE_IDENTITY(Warning, TEXT("Missing DiscoveryUrl= in [OnlineSubsystemSRO.OnlineIdentitySRO] of DefaultEngine.ini"));
	}

	GConfig->GetArray(TEXT("OnlineSubsystemSRO.OnlineIdentitySRO"), TEXT("LoginDomains"), LoginDomains, GEngineIni);

	LoginURLDetails.ClientId = InSubsystem->GetAppId();

	// Setup permission scope fields
	GConfig->GetArray(TEXT("OnlineSubsystemSRO.OnlineIdentitySRO"), TEXT("ScopeFields"), LoginURLDetails.ScopeFields, GEngineIni);
	// always required login access fields
	LoginURLDetails.ScopeFields.AddUnique(TEXT("openid"));
	LoginURLDetails.ScopeFields.AddUnique(TEXT(SRO_PERM_PUBLIC_PROFILE));
	LoginURLDetails.bRequestOfflineAccess = ShouldRequestOfflineAccess();
}

void FOnlineIdentitySRO::DiscoveryRequest_HttpRequestComplete(FHttpRequestPtr HttpRequest, FHttpResponsePtr HttpResponse, bool bSucceeded, PendingLoginRequestCb LoginCb)
{
	if (bSucceeded &&
		HttpResponse.IsValid())
	{
		FString ResponseStr = HttpResponse->GetContentAsString();
		if (EHttpResponseCodes::IsOk(HttpResponse->GetResponseCode()))
		{
			UE_LOG_ONLINE_IDENTITY(Verbose, TEXT("Discovery request complete. url=%s code=%d response=%s"),
				*HttpRequest->GetURL(), HttpResponse->GetResponseCode(), *ResponseStr);
			if (!Endpoints.Parse(ResponseStr))
			{
				UE_LOG_ONLINE_IDENTITY(Warning, TEXT("Failed to parse SRO discovery endpoint"));
			}
		}
		else
		{
			UE_LOG_ONLINE_IDENTITY(Warning, TEXT("Bad response from SRO discovery endpoint"));
		}
	}
	else
	{
		UE_LOG_ONLINE_IDENTITY(Warning, TEXT("SRO discovery endpoint failure"));
	}

	LoginCb(Endpoints.IsValid());
	
	if (Endpoints.IsValid())
	{
		LoginURLDetails.LoginUrl = Endpoints.AuthEndpoint;
	}
}

bool FOnlineIdentitySRO::Login(int32 LocalUserNum, const FOnlineAccountCredentials& AccountCredentials)
{
	FString ErrorStr;

	if (bHasLoginOutstanding)
	{
		ErrorStr = FString::Printf(TEXT("Registration already pending for user"));
	}
	else if (!LoginURLDetails.IsValid())
	{
		ErrorStr = FString::Printf(TEXT("OnlineSubsystemSRO is improperly configured in DefaultEngine.ini LoginRedirectUrl=%s RedirectPort=%d ClientId=%s"),
			*LoginURLDetails.LoginRedirectUrl, LoginURLDetails.RedirectPort, *LoginURLDetails.ClientId);
	}
	else
	{
		if (LocalUserNum < 0 || LocalUserNum >= MAX_LOCAL_PLAYERS)
		{
			ErrorStr = FString::Printf(TEXT("Invalid LocalUserNum=%d"), LocalUserNum);
		}
		else
		{
			PendingLoginRequestCb PendingLoginFn;
			if (!AccountCredentials.Id.IsEmpty() && !AccountCredentials.Token.IsEmpty() && AccountCredentials.Type == GetAuthType())
			{
				const FString AccessToken = AccountCredentials.Token;
				PendingLoginFn = [this, LocalUserNum, AccessToken](bool bWasSuccessful)
				{
					if (bWasSuccessful)
					{
						LoginURLDetails.LoginUrl = Endpoints.AuthEndpoint;

						FAuthTokenSRO AuthToken(AccessToken, ESRORefreshToken::SRORefreshToken);
						Login(LocalUserNum, AuthToken, FOnLoginCompleteDelegate::CreateRaw(this, &FOnlineIdentitySRO::OnAccessTokenLoginComplete));
					}
					else
					{
						const FString ErrorStr = TEXT("Error retrieving discovery service");
						OnLoginAttemptComplete(LocalUserNum, ErrorStr);
					}
				};
			}
			else
			{
				PendingLoginFn = [this, LocalUserNum](bool bWasSuccessful)
				{
					if (bWasSuccessful)
					{
						LoginURLDetails.LoginUrl = Endpoints.AuthEndpoint;
						IOnlineExternalUIPtr OnlineExternalUI = SROSubsystem->GetExternalUIInterface();
						if (ensure(OnlineExternalUI.IsValid()))
						{
							LoginURLDetails.GenerateNonce();
							FOnLoginUIClosedDelegate CompletionDelegate = FOnLoginUIClosedDelegate::CreateRaw(this, &FOnlineIdentitySRO::OnExternalUILoginComplete);
							OnlineExternalUI->ShowLoginUI(LocalUserNum, true, false, CompletionDelegate);
						}
					}
					else
					{
						const FString ErrorStr = TEXT("Error retrieving discovery service");
						OnLoginAttemptComplete(LocalUserNum, ErrorStr);
					}
				};
			}

			bHasLoginOutstanding = true;
			if (Endpoints.IsValid())
			{
				PendingLoginFn(true);
			}
			else
			{
				RetrieveDiscoveryDocument(MoveTemp(PendingLoginFn));
			}
		}
	}

	if (!ErrorStr.IsEmpty())
	{
		UE_LOG_ONLINE_IDENTITY(Error, TEXT("FOnlineIdentitySRO::Login() failed: %s"),	*ErrorStr);
		OnLoginAttemptComplete(LocalUserNum, ErrorStr);
		return false;
	}
	return true;
}

void FOnlineIdentitySRO::Login(int32 LocalUserNum, const FAuthTokenSRO& InToken, const FOnLoginCompleteDelegate& InCompletionDelegate)
{	
	TFunction<void(int32, bool, const FAuthTokenSRO&, const FString&)> NextFn = 
	[this, InCompletionDelegate](int32 InLocalUserNum, bool bWasSuccessful, const FAuthTokenSRO& InAuthToken, const FString& ErrorStr)
	{
		FOnProfileRequestComplete ProfileCompletionDelegate = FOnProfileRequestComplete::CreateLambda([this, InCompletionDelegate](int32 ProfileLocalUserNum, bool bWasSuccessful, const FString& ErrorStr)
		{
			if (bWasSuccessful)
			{
				InCompletionDelegate.ExecuteIfBound(ProfileLocalUserNum, bWasSuccessful, *GetUniquePlayerId(ProfileLocalUserNum), ErrorStr);
			}
			else
			{
				InCompletionDelegate.ExecuteIfBound(ProfileLocalUserNum, bWasSuccessful, *FUniqueNetIdSRO::EmptyId(), ErrorStr);
			}
		});

		if (bWasSuccessful)
		{
			ProfileRequest(InLocalUserNum, InAuthToken, ProfileCompletionDelegate);
		}
		else
		{
			InCompletionDelegate.ExecuteIfBound(InLocalUserNum, bWasSuccessful, *FUniqueNetIdSRO::EmptyId(), ErrorStr);
		}
	};

	if (InToken.AuthType == ESROAuthTokenType::ExchangeToken)
	{
		FOnExchangeRequestComplete CompletionDelegate = FOnExchangeRequestComplete::CreateLambda([NextFn](int32 InLocalUserNum, bool bWasSuccessful, const FAuthTokenSRO& InAuthToken, const FString& ErrorStr)
		{
			NextFn(InLocalUserNum, bWasSuccessful, InAuthToken, ErrorStr);
		});

		ExchangeCode(LocalUserNum, InToken, CompletionDelegate);
	}
	else if (InToken.AuthType == ESROAuthTokenType::RefreshToken)
	{
		FOnRefreshAuthRequestComplete CompletionDelegate = FOnRefreshAuthRequestComplete::CreateLambda([NextFn](int32 InLocalUserNum, bool bWasSuccessful, const FAuthTokenSRO& InAuthToken, const FString& ErrorStr)
		{
			NextFn(InLocalUserNum, bWasSuccessful, InAuthToken, ErrorStr);
		});

		RefreshAuth(LocalUserNum, InToken, CompletionDelegate);
	}
	else if (InToken.AuthType == ESROAuthTokenType::ClientCredentials)
	{
		FOnRefreshAuthRequestComplete CompletionDelegate = FOnRefreshAuthRequestComplete::CreateLambda([this, InCompletionDelegate](int32 InLocalUserNum, bool bWasSuccessful, const FAuthTokenSRO& InAuthToken, const FString& ErrorStr)
		{
			FString NewErrorStr;
			TSharedRef<FUserOnlineAccountSRO> User = MakeShared<FUserOnlineAccountSRO>();
			if (bWasSuccessful)
			{
				bWasSuccessful = false;
				TArray<FString> Parts;
				InAuthToken.AccessToken.ParseIntoArray(Parts, TEXT("."));
				if (Parts.Num() == 3)
				{
					FString TokenDataJson;
					if(FBase64::Decode(Parts[1], TokenDataJson))
					{
						if (User->Parse(InAuthToken, TokenDataJson))
						{
							AddUser(User, InLocalUserNum);
							bWasSuccessful = true;
						}
						else
						{
							NewErrorStr = TEXT("failed to parse client from access token");
						}
					}
					else
					{
						NewErrorStr = TEXT("failed to parse access token body");
					}
				}
				else
				{
					NewErrorStr = TEXT("invalid access token format");
				}
			}
			else
			{
				NewErrorStr = TEXT("refresh auth failed: ")+ErrorStr;
			}
			
			InCompletionDelegate.ExecuteIfBound(InLocalUserNum, bWasSuccessful, User->GetUserId().Get(), NewErrorStr);
		});
		ClientAuth(CompletionDelegate);
	}
}

void FOnlineIdentitySRO::OnAccessTokenLoginComplete(int32 LocalUserNum, bool bWasSuccessful, const FUniqueNetId& UniqueId, const FString& Error)
{
	OnLoginAttemptComplete(LocalUserNum, Error);
}

void FOnlineIdentitySRO::OnExternalUILoginComplete(FUniqueNetIdPtr UniqueId, const int ControllerIndex, const FOnlineError& Error)
{
	const FString& ErrorStr = Error.GetErrorCode();
	const bool bWasSuccessful = Error.WasSuccessful() && UniqueId.IsValid() && UniqueId->IsValid();
	OnAccessTokenLoginComplete(ControllerIndex, bWasSuccessful, bWasSuccessful ? *UniqueId : *FUniqueNetIdSRO::EmptyId(), ErrorStr);
}

FString FOnlineIdentitySRO::ValidateToken(const FAuthTokenSRO& InToken)
{
	FString ErrorStr;
	//Verify that the value of iss in the ID token is SRO issued
	if (!InToken.IdToken.IsEmpty() &&
		!InToken.IdTokenJWT.Validate(SROSubsystem->SROIdentity->GetEndpoints().Issuer, {SROSubsystem->GetAppId(), SROSubsystem->GetServerClientId()}))
	{
		return TEXT("Failed to validate ID JWT");
	}

	return TEXT("");
}

void FOnlineIdentitySRO::OnLoginAttemptComplete(int32 LocalUserNum, const FString& ErrorStr)
{
	const FString ErrorStrCopy(ErrorStr);

	bHasLoginOutstanding = false;
	if (GetLoginStatus(LocalUserNum) == ELoginStatus::LoggedIn)
	{
		UE_LOG_ONLINE_IDENTITY(Display, TEXT("login was successful"));
		FUniqueNetIdPtr UserId = GetUniquePlayerId(LocalUserNum);
		check(UserId.IsValid());

		SROSubsystem->ExecuteNextTick([this, UserId, LocalUserNum, ErrorStrCopy]()
		{
			TriggerOnLoginCompleteDelegates(LocalUserNum, true, *UserId, ErrorStrCopy);
			TriggerOnLoginStatusChangedDelegates(LocalUserNum, ELoginStatus::NotLoggedIn, ELoginStatus::LoggedIn, *UserId);
		});
	}
	else
	{
		UE_LOG_ONLINE_IDENTITY(Warning, TEXT("login failed: %s"), *ErrorStr);
		SROSubsystem->ExecuteNextTick([this, LocalUserNum, ErrorStrCopy]()
		{
			TriggerOnLoginCompleteDelegates(LocalUserNum, false, *FUniqueNetIdSRO::EmptyId(), ErrorStrCopy);
		});
	}
}

void FOnlineIdentitySRO::ExchangeCode(int32 LocalUserNum, const FAuthTokenSRO& InExchangeToken, FOnExchangeRequestComplete& InCompletionDelegate)
{
	FString ErrorStr;
	bool bStarted = false;
	if (LocalUserNum >= 0 && LocalUserNum < MAX_LOCAL_PLAYERS)
	{
		if (Endpoints.IsValid() && !Endpoints.TokenEndpoint.IsEmpty())
		{
			if (InExchangeToken.IsValid())
			{
				check(InExchangeToken.AuthType == ESROAuthTokenType::ExchangeToken);
				bStarted = true;

				const FString RedirectURL = LoginURLDetails.GetRedirectURL();
				const FString PostContent = FString::Printf(TEXT("code=%s&client_id=%s&redirect_uri=%s&grant_type=authorization_code"),
					*InExchangeToken.AccessToken,
					*SROSubsystem->GetAppId(),
					*RedirectURL);

				// kick off http request to convert the exchange code to access token
				TSharedRef<IHttpRequest, ESPMode::ThreadSafe> HttpRequest = FHttpModule::Get().CreateRequest();

				HttpRequest->OnProcessRequestComplete().BindRaw(this, &FOnlineIdentitySRO::ExchangeRequest_HttpRequestComplete, LocalUserNum, InExchangeToken, InCompletionDelegate);
				HttpRequest->SetURL(Endpoints.TokenEndpoint);
				HttpRequest->SetHeader(TEXT("Content-Type"), TEXT("application/x-www-form-urlencoded"));
				HttpRequest->SetVerb(TEXT("POST"));

				HttpRequest->SetContentAsString(PostContent);
				HttpRequest->ProcessRequest();
			}
			else
			{
				ErrorStr = TEXT("No access token specified");
			}
		}
		else
		{
			ErrorStr = TEXT("Invalid SRO endpoint");
		}
	}
	else
	{
		ErrorStr = TEXT("Invalid local user num");
	}

	if (!bStarted)
	{
		FAuthTokenSRO EmptyAuthToken;
		InCompletionDelegate.ExecuteIfBound(LocalUserNum, false, EmptyAuthToken, ErrorStr);
	}
}

void FOnlineIdentitySRO::ExchangeRequest_HttpRequestComplete(FHttpRequestPtr HttpRequest, FHttpResponsePtr HttpResponse, bool bSucceeded, int32 InLocalUserNum, FAuthTokenSRO InExchangeToken, FOnExchangeRequestComplete InCompletionDelegate)
{
	bool bResult = false;
	FString ResponseStr, ErrorStr;

	FAuthTokenSRO AuthToken;

	if (bSucceeded &&
		HttpResponse.IsValid())
	{
		ResponseStr = HttpResponse->GetContentAsString();
		if (EHttpResponseCodes::IsOk(HttpResponse->GetResponseCode()))
		{
			UE_LOG_ONLINE_IDENTITY(Verbose, TEXT("ExchangeCode request complete. url=%s code=%d response=%s"),
				*HttpRequest->GetURL(), HttpResponse->GetResponseCode(), *ResponseStr);

			if (AuthToken.Parse(ResponseStr))
			{
				ErrorStr = ValidateToken(AuthToken);
				bResult = ErrorStr.IsEmpty();
			}
			else
			{
				ErrorStr = FString::Printf(TEXT("Failed to parse auth json"));
				Logout(InLocalUserNum);
			}
		}
		else
		{
			FErrorSRO Error;
			if (Error.FromJson(ResponseStr) && !Error.Error_Description.IsEmpty())
			{
				ErrorStr = Error.Error_Description;
			}
			else
			{
				ErrorStr = FString::Printf(TEXT("Failed to parse SRO error %s"), *ResponseStr);
			}
		}
	}
	else
	{
		ErrorStr = TEXT("No response");
	}

	if (!ErrorStr.IsEmpty())
	{
		UE_LOG_ONLINE_IDENTITY(Warning, TEXT("Exchange request failed. %s"), *ErrorStr);
	}

	InCompletionDelegate.ExecuteIfBound(InLocalUserNum, bResult, AuthToken, ErrorStr);
}

void FOnlineIdentitySRO::ClientAuth(FOnRefreshAuthRequestComplete& InCompletionDelegate)
{
	FString ErrorStr;
#if WITH_SERVER_CODE
	RetrieveDiscoveryDocument([this, InCompletionDelegate](bool bWasSuccessful)
	{
		FString ErrorStr;
		if (!bWasSuccessful)
		{
			ErrorStr = TEXT("Failed getting auth endpoints");
		}
		else if (ServerClientId.IsEmpty() || ServerClientSecret.IsEmpty())
		{
			ErrorStr = TEXT("Missing ServerClientId or ServerClientSecret");
		}
		else if (Endpoints.IsValid() && !Endpoints.TokenEndpoint.IsEmpty())
		{
			const FString PostContent = FString::Printf(TEXT("client_id=%s&client_secret=%s&grant_type=client_credentials&scope=openid"), *ServerClientId, *ServerClientSecret); // kick off http request to refresh the auth token
			TSharedRef<IHttpRequest, ESPMode::ThreadSafe> HttpRequest = FHttpModule::Get().CreateRequest();

			HttpRequest->OnProcessRequestComplete().BindRaw(this, &FOnlineIdentitySRO::ClientCredentialsRequest_HttpRequestComplete, InCompletionDelegate);
			HttpRequest->SetURL(Endpoints.TokenEndpoint);
			HttpRequest->SetHeader(TEXT("Content-Type"), TEXT("application/x-www-form-urlencoded"));
			HttpRequest->SetVerb(TEXT("POST"));

			HttpRequest->SetContentAsString(PostContent);
			HttpRequest->ProcessRequest();
		}
		else
		{
			ErrorStr = TEXT("Invalid SRO endpoints");
		}
			
		if (!ErrorStr.IsEmpty())
		{
			FAuthTokenSRO EmptyAuthToken;
			InCompletionDelegate.ExecuteIfBound(0, false, EmptyAuthToken, ErrorStr);
		}
	});
	return;
#else
	ErrorStr = "Client Auth not supported in client builds";
	FAuthTokenSRO EmptyAuthToken;
	InCompletionDelegate.ExecuteIfBound(0, false, EmptyAuthToken, ErrorStr);
#endif
}

void FOnlineIdentitySRO::RegisterAsyncTokenUpdate(int32 LocalUserNum)
{
	FOnlineAsyncTaskManagerSRO* AsyncTaskManager = SROSubsystem->GetAsyncTaskManager();
	FOnlineAsyncTaskSRORefreshAuth* RefreshTask = new FOnlineAsyncTaskSRORefreshAuth(SROSubsystem, LocalUserNum);
	AsyncTaskManager->AddToInQueue(RefreshTask);
}

void FOnlineIdentitySRO::RefreshAuth(int32 LocalUserNum, const FAuthTokenSRO& InAuthToken, FOnRefreshAuthRequestComplete& InCompletionDelegate)
{
	FString ErrorStr;
	bool bStarted = false;
	if (LocalUserNum >= 0 && LocalUserNum < MAX_LOCAL_PLAYERS)
	{
		if (Endpoints.IsValid() && !Endpoints.TokenEndpoint.IsEmpty())
		{
			if (InAuthToken.IsValid())
			{
				check(InAuthToken.AuthType == ESROAuthTokenType::AccessToken || InAuthToken.AuthType == ESROAuthTokenType::RefreshToken);
				bStarted = true;

				const FString PostContent = FString::Printf(TEXT("client_id=%s&refresh_token=%s&grant_type=refresh_token"),
					*SROSubsystem->GetAppId(),
					*InAuthToken.RefreshToken);

				// kick off http request to refresh the auth token
				TSharedRef<IHttpRequest, ESPMode::ThreadSafe> HttpRequest = FHttpModule::Get().CreateRequest();

				HttpRequest->OnProcessRequestComplete().BindRaw(this, &FOnlineIdentitySRO::RefreshAuthRequest_HttpRequestComplete, LocalUserNum, InAuthToken, InCompletionDelegate);
				HttpRequest->SetURL(Endpoints.TokenEndpoint);
				HttpRequest->SetHeader(TEXT("Content-Type"), TEXT("application/x-www-form-urlencoded"));
				HttpRequest->SetVerb(TEXT("POST"));

				HttpRequest->SetContentAsString(PostContent);
				HttpRequest->ProcessRequest();
			}
			else
			{
				ErrorStr = TEXT("No access token specified");
			}
		}
		else
		{
			ErrorStr = TEXT("Invalid SRO endpoint");
		}
	}
	else
	{
		ErrorStr = TEXT("Invalid local user num");
	}

	if (!bStarted)
	{
		FAuthTokenSRO EmptyAuthToken;
		InCompletionDelegate.ExecuteIfBound(LocalUserNum, false, EmptyAuthToken, ErrorStr);
	}
}

void FOnlineIdentitySRO::ClientCredentialsRequest_HttpRequestComplete(FHttpRequestPtr HttpRequest, FHttpResponsePtr HttpResponse, bool bSucceeded, FOnRefreshAuthRequestComplete InCompletionDelegate)
{
	bool bResult = false;
	FString ResponseStr, ErrorStr;

	FAuthTokenSRO AuthToken;

	if (bSucceeded &&
		HttpResponse.IsValid())
	{
		ResponseStr = HttpResponse->GetContentAsString();
		if (EHttpResponseCodes::IsOk(HttpResponse->GetResponseCode()))
		{
			UE_LOG_ONLINE_IDENTITY(Log, TEXT("ClientCredential request complete. url=%s code=%d"),
				*HttpRequest->GetURL(), HttpResponse->GetResponseCode());

			if (AuthToken.Parse(ResponseStr))
			{
				AuthToken.AuthType = ESROAuthTokenType::ClientCredentials;
				ErrorStr = ValidateToken(AuthToken);
				bResult = ErrorStr.IsEmpty();
			}
			else
			{
				ErrorStr = FString::Printf(TEXT("Failed to parse client credentials auth json"));
			}
		}
		else
		{
			FErrorSRO Error;
			if (Error.FromJson(ResponseStr) && !Error.Error_Description.IsEmpty())
			{
				ErrorStr = "Error Response: " + Error.Error_Description;
			}
			else
			{
				ErrorStr = FString::Printf(TEXT("Failed to parse SRO error %s"), *ResponseStr);
			}
		}
	}
	else
	{
		ErrorStr = TEXT("No response");
	}

	if (!ErrorStr.IsEmpty())
	{
		UE_LOG_ONLINE_IDENTITY(Warning, TEXT("ClientCredential request failed. %s"), *ErrorStr);
	}

	InCompletionDelegate.ExecuteIfBound(0, bResult, AuthToken, ErrorStr);
}

void FOnlineIdentitySRO::RefreshAuthRequest_HttpRequestComplete(FHttpRequestPtr HttpRequest, FHttpResponsePtr HttpResponse, bool bSucceeded, int32 InLocalUserNum, FAuthTokenSRO InOldAuthToken, FOnRefreshAuthRequestComplete InCompletionDelegate)
{
	bool bResult = false;
	FString ResponseStr, ErrorStr;

	FAuthTokenSRO AuthToken;

	if (bSucceeded &&
		HttpResponse.IsValid())
	{
		ResponseStr = HttpResponse->GetContentAsString();
		if (EHttpResponseCodes::IsOk(HttpResponse->GetResponseCode()))
		{
			UE_LOG_ONLINE_IDENTITY(Verbose, TEXT("RefreshAuth request complete. url=%s code=%d response=%s"),
				*HttpRequest->GetURL(), HttpResponse->GetResponseCode(), *ResponseStr);

			if (AuthToken.Parse(ResponseStr, InOldAuthToken))
			{
				AuthToken.AuthType = ESROAuthTokenType::RefreshToken;
				bResult = true;
			}
			else
			{
				ErrorStr = FString::Printf(TEXT("Failed to parse refresh auth json"));
			}
		}
		else
		{
			FErrorSRO Error;
			if (Error.FromJson(ResponseStr) && !Error.Error_Description.IsEmpty())
			{
				ErrorStr = Error.Error_Description;
			}
			else
			{
				ErrorStr = FString::Printf(TEXT("Failed to parse SRO error %s"), *ResponseStr);
			}
		}
	}
	else
	{
		ErrorStr = TEXT("No response");
	}

	if (!ErrorStr.IsEmpty())
	{
		UE_LOG_ONLINE_IDENTITY(Warning, TEXT("RefreshAuth request failed. %s"), *ErrorStr);
	}

	InCompletionDelegate.ExecuteIfBound(InLocalUserNum, bResult, AuthToken, ErrorStr);
}

bool FOnlineIdentitySRO::Logout(int32 LocalUserNum)
{
	bool bResult = false;
	FUniqueNetIdPtr UserId = GetUniquePlayerId(LocalUserNum);
	if (UserId.IsValid())
	{
		// remove cached user account
		UserAccounts.Remove(UserId->ToString());
		// remove cached user id
		UserIds.Remove(LocalUserNum);
		// reset scope permissions
		GConfig->GetArray(TEXT("OnlineSubsystemSRO.OnlineIdentitySRO"), TEXT("ScopeFields"), LoginURLDetails.ScopeFields, GEngineIni);
		// always required login access fields
		LoginURLDetails.ScopeFields.AddUnique(TEXT(SRO_PERM_PUBLIC_PROFILE));

		TriggerOnLoginFlowLogoutDelegates(LoginDomains);

		// not async but should call completion delegate anyway
		SROSubsystem->ExecuteNextTick([this, LocalUserNum, UserId]() 
		{
			TriggerOnLogoutCompleteDelegates(LocalUserNum, true);
			TriggerOnLoginStatusChangedDelegates(LocalUserNum, ELoginStatus::LoggedIn, ELoginStatus::NotLoggedIn, *UserId);
		});

		bResult = true;
	}
	else
	{
		UE_LOG_ONLINE_IDENTITY(Warning, TEXT("No logged in user found for LocalUserNum=%d."), LocalUserNum);
		SROSubsystem->ExecuteNextTick([this, LocalUserNum]() 
		{
			TriggerOnLogoutCompleteDelegates(LocalUserNum, false);
		});
	}
	
	return bResult;
}

bool FOnlineIdentitySRO::ShouldRequestOfflineAccess()
{
	bool bRequestOfflineAccess = false;
	GConfig->GetBool(TEXT("OnlineSubsystemSRO.OnlineIdentitySRO"), TEXT("bRequestOfflineAccess"), bRequestOfflineAccess, GEngineIni);
	return bRequestOfflineAccess;
}

TSharedPtr<FUserOnlineAccount> FOnlineIdentitySRO::GetUserAccount(const FUniqueNetId& UserId) const
{
	TSharedPtr<FUserOnlineAccount> Result;

	const TSharedRef<FUserOnlineAccountSRO>* FoundUserAccount = UserAccounts.Find(UserId.ToString());
	if (FoundUserAccount != nullptr)
	{
		Result = *FoundUserAccount;
	}

	return Result;
}

TArray<TSharedPtr<FUserOnlineAccount> > FOnlineIdentitySRO::GetAllUserAccounts() const
{
	TArray<TSharedPtr<FUserOnlineAccount> > Result;
	
	for (FUserOnlineAccountSROMap::TConstIterator It(UserAccounts); It; ++It)
	{
		Result.Add(It.Value());
	}

	return Result;
}

FUniqueNetIdPtr FOnlineIdentitySRO::GetUniquePlayerId(int32 LocalUserNum) const
{
	const FUniqueNetIdPtr* FoundId = UserIds.Find(LocalUserNum);
	if (FoundId != nullptr)
	{
		return *FoundId;
	}
	return nullptr;
}


void FOnlineIdentitySRO::RetrieveDiscoveryDocument(PendingLoginRequestCb&& LoginCb)
{
	if (!Endpoints.IsValid())
	{
		// kick off http request to get the discovery document
		TSharedRef<IHttpRequest, ESPMode::ThreadSafe> HttpRequest = FHttpModule::Get().CreateRequest();

		HttpRequest->OnProcessRequestComplete().BindRaw(this, &FOnlineIdentitySRO::DiscoveryRequest_HttpRequestComplete, LoginCb);
		HttpRequest->SetURL(Endpoints.DiscoveryUrl);
		HttpRequest->SetVerb(TEXT("GET"));
		HttpRequest->ProcessRequest();
	}
	else
	{
		LoginCb(true);
	}
}

void FOnlineIdentitySRO::ProfileRequest(int32 LocalUserNum, const FAuthTokenSRO& InAuthToken, FOnProfileRequestComplete& InCompletionDelegate)
{
	FString ErrorStr;
	bool bStarted = false;
	if (LocalUserNum >= 0 && LocalUserNum < MAX_LOCAL_PLAYERS)
	{
		if (Endpoints.IsValid() && !Endpoints.UserInfoEndpoint.IsEmpty())
		{
			if (InAuthToken.IsValid())
			{
				check(InAuthToken.AuthType == ESROAuthTokenType::AccessToken);
				bStarted = true;

				// kick off http request to get user info with the access token
				TSharedRef<IHttpRequest, ESPMode::ThreadSafe> HttpRequest = FHttpModule::Get().CreateRequest();

				const FString BearerToken = FString::Printf(TEXT("Bearer %s"), *InAuthToken.AccessToken);

				HttpRequest->OnProcessRequestComplete().BindRaw(this, &FOnlineIdentitySRO::MeUser_HttpRequestComplete, LocalUserNum, InAuthToken, InCompletionDelegate);
				HttpRequest->SetURL(Endpoints.UserInfoEndpoint);
				HttpRequest->SetHeader(TEXT("Authorization"), BearerToken);
				HttpRequest->SetHeader(TEXT("Content-Type"), TEXT("application/json"));
				HttpRequest->SetVerb(TEXT("GET"));
				HttpRequest->ProcessRequest();
			}
			else
			{
				ErrorStr = TEXT("Invalid access token specified");
			}
		}
		else
		{
			ErrorStr = TEXT("No MeURL specified in DefaultEngine.ini");
		}
	}
	else
	{
		ErrorStr = TEXT("Invalid local user num");
	}

	if (!bStarted)
	{
		InCompletionDelegate.ExecuteIfBound(LocalUserNum, false, ErrorStr);
	}
}

void FOnlineIdentitySRO::MeUser_HttpRequestComplete(FHttpRequestPtr HttpRequest, FHttpResponsePtr HttpResponse, bool bSucceeded, int32 InLocalUserNum, FAuthTokenSRO InAuthToken, FOnProfileRequestComplete InCompletionDelegate)
{
	bool bResult = false;
	FString ResponseStr, ErrorStr;
	
	if (bSucceeded &&
		HttpResponse.IsValid())
	{
		ResponseStr = HttpResponse->GetContentAsString();
		if (EHttpResponseCodes::IsOk(HttpResponse->GetResponseCode()))
		{
			UE_LOG_ONLINE_IDENTITY(Verbose, TEXT("RegisterUser request complete. url=%s code=%d response=%s"),
				*HttpRequest->GetURL(), HttpResponse->GetResponseCode(), *ResponseStr);

			TSharedRef<FUserOnlineAccountSRO> User = MakeShared<FUserOnlineAccountSRO>();
			if (User->Parse(InAuthToken, ResponseStr))
			{
				AddUser(User, InLocalUserNum);
				bResult = true;
			}
			else
			{
				ErrorStr = FString::Printf(TEXT("Error parsing login. payload=%s"),
					*ResponseStr);
			}
		}
		else
		{
			FErrorSRO Error;
			if (Error.FromJson(ResponseStr) && !Error.Error_Description.IsEmpty())
			{
				ErrorStr = Error.Error_Description;
			}
			else
			{
				ErrorStr = FString::Printf(TEXT("Failed to parse SRO error %s"), *ResponseStr);
			}
		}
	}
	else
	{
		ErrorStr = TEXT("No response");
	}

	if (!ErrorStr.IsEmpty())
	{
		UE_LOG_ONLINE_IDENTITY(Warning, TEXT("RegisterUser request failed. %s"), *ErrorStr);
	}

	InCompletionDelegate.ExecuteIfBound(InLocalUserNum, bResult, ErrorStr);
}

FUniqueNetIdPtr FOnlineIdentitySRO::CreateUniquePlayerId(uint8* Bytes, int32 Size)
{
	if (Bytes != nullptr && Size > 0)
	{
		FString StrId(Size, (TCHAR*)Bytes);
		return FUniqueNetIdSRO::Create(StrId);
	}
	return nullptr;
}

FUniqueNetIdPtr FOnlineIdentitySRO::CreateUniquePlayerId(const FString& Str)
{
	return FUniqueNetIdSRO::Create(Str);
}

bool FOnlineIdentitySRO::AutoLogin(int32 LocalUserNum)
{
#if UE_BUILD_DEVELOPMENT
	FSROLoginWindowModule* Module = static_cast<FSROLoginWindowModule*>(FModuleManager::Get().GetModule(FName("SROLoginWindow")));
	if (Module)
	{
		FAuthTokenSRO AuthToken = Module->GetAuthToken();
		if (AuthToken.IsValid())
		{
			FOnProfileRequestComplete OnComplete = FOnProfileRequestComplete::CreateLambda([this](int32 LocalUserNum, bool bWasSuccessful, const FString& ErrorStr)
			{
				FUniqueNetIdPtr NetId = bWasSuccessful ? GetUniquePlayerId(LocalUserNum) : FUniqueNetIdSRO::EmptyId();
				OnAccessTokenLoginComplete(LocalUserNum, bWasSuccessful, *NetId, ErrorStr);
			});
			ProfileRequest(0, AuthToken, OnComplete);
			return true;
		}
	}
#endif
	
#if WITH_SERVER_CODE
	if (IsRunningDedicatedServer())
	{
		FAuthTokenSRO EmptyAuthToken;
		EmptyAuthToken.AuthType = ESROAuthTokenType::ClientCredentials;
		Login(LocalUserNum, EmptyAuthToken, FOnLoginCompleteDelegate::CreateRaw(this, &FOnlineIdentitySRO::OnAccessTokenLoginComplete));
		return true;
	}
	UE_LOG_ONLINE_IDENTITY(Warning, TEXT("AutoLogin with server code, but not running as server"));
	return false;
#else
	UE_LOG_ONLINE_IDENTITY(Warning, TEXT("AutoLogin not valid for client builds"));
	return false;
#endif
}

ELoginStatus::Type FOnlineIdentitySRO::GetLoginStatus(int32 LocalUserNum) const
{
	FUniqueNetIdPtr UserId = GetUniquePlayerId(LocalUserNum);
	if (UserId.IsValid())
	{
		return GetLoginStatus(*UserId);
	}
	return ELoginStatus::NotLoggedIn;
}

ELoginStatus::Type FOnlineIdentitySRO::GetLoginStatus(const FUniqueNetId& UserId) const
{
	TSharedPtr<FUserOnlineAccount> UserAccount = GetUserAccount(UserId);
	if (UserAccount.IsValid() &&
		UserAccount->GetUserId()->IsValid() &&
		(!bAccessTokenAvailableToPlatform || !UserAccount->GetAccessToken().IsEmpty()))
	{
		return ELoginStatus::LoggedIn;
	}
	return ELoginStatus::NotLoggedIn;
}

FString FOnlineIdentitySRO::GetPlayerNickname(int32 LocalUserNum) const
{
	FUniqueNetIdPtr UserId = GetUniquePlayerId(LocalUserNum);
	if (UserId.IsValid())
	{
		return GetPlayerNickname(*UserId);
	}
	return TEXT("");
}

FString FOnlineIdentitySRO::GetPlayerNickname(const FUniqueNetId& UserId) const
{
	const TSharedRef<FUserOnlineAccountSRO>* FoundUserAccount = UserAccounts.Find(UserId.ToString());
	if (FoundUserAccount != nullptr)
	{
		const TSharedRef<FUserOnlineAccountSRO>& UserAccount = *FoundUserAccount;
		return UserAccount->GetRealName();
	}
	return TEXT("");
}

FString FOnlineIdentitySRO::GetAuthToken(int32 LocalUserNum) const
{
	FUniqueNetIdPtr UserId = GetUniquePlayerId(LocalUserNum);
	if (UserId.IsValid())
	{
		TSharedPtr<FUserOnlineAccount> UserAccount = GetUserAccount(*UserId);
		if (UserAccount.IsValid())
		{
			return UserAccount->GetAccessToken();
		}
	}
	return FString();
}

void FOnlineIdentitySRO::GetUserPrivilege(const FUniqueNetId& UserId, EUserPrivileges::Type Privilege, const FOnGetUserPrivilegeCompleteDelegate& Delegate, EShowPrivilegeResolveUI ShowResolveUI)
{
	Delegate.ExecuteIfBound(UserId, Privilege, (uint32)EPrivilegeResults::NoFailures);
}	

FPlatformUserId FOnlineIdentitySRO::GetPlatformUserIdFromUniqueNetId(const FUniqueNetId& UniqueNetId) const
{
	for (int i = 0; i < MAX_LOCAL_PLAYERS; ++i)
	{
		auto CurrentUniqueId = GetUniquePlayerId(i);
		if (CurrentUniqueId.IsValid() && (*CurrentUniqueId == UniqueNetId))
		{
			return GetPlatformUserIdFromLocalUserNum(i);
		}
	}

	return PLATFORMUSERID_NONE;
}

FString FOnlineIdentitySRO::GetAuthType() const
{
	return AUTH_TYPE_SRO;
}

void FOnlineIdentitySRO::RevokeAuthToken(const FUniqueNetId& UserId, const FOnRevokeAuthTokenCompleteDelegate& Delegate)
{
	UE_LOG_ONLINE_IDENTITY(Display, TEXT("FOnlineIdentitySRO::RevokeAuthToken not implemented"));
	FUniqueNetIdRef UserIdRef(UserId.AsShared());
	SROSubsystem->ExecuteNextTick([UserIdRef, Delegate]()
	{
		Delegate.ExecuteIfBound(*UserIdRef, FOnlineError(FString(TEXT("RevokeAuthToken not implemented"))));
	});
}

int32 FOnlineIdentitySRO::GetLocalUserNumFromUniqueNetId(const FUniqueNetId& NetId) const
{
	for (const auto& User : UserIds)
	{
		if (User.Value->AsShared() == NetId.AsShared())
		{
			return User.Key;
		}
	}
	return -1;
}

