#include "FOnlineAsyncTaskSRORefreshAuth.h"

#include "OnlineIdentitySRO.h"

void FOnlineAsyncTaskSRORefreshAuth::Finalize()
{
	FOnlineAsyncTaskBasic<FOnlineSubsystemSRO>::Finalize();
}

void FOnlineAsyncTaskSRORefreshAuth::Initialize()
{
	FUniqueNetIdPtr UserId = Subsystem->SROIdentity->GetUniquePlayerId(LocalUserNum);
	check(UserId.IsValid() && UserId->IsValid());
	
	TSharedPtr<FUserOnlineAccountSRO> Account = StaticCastSharedPtr<FUserOnlineAccountSRO>(Subsystem->SROIdentity->GetUserAccount(*UserId.Get()));
	check(Account.IsValid());

	AuthTokenSRO = Account->GetSROAuthToken();
}

void FOnlineAsyncTaskSRORefreshAuth::Tick()
{
	FOnlineAsyncTaskBasic<FOnlineSubsystemSRO>::Tick();
	if (!bRequestMade && FDateTime::UtcNow() + RefreshBufferTime > AuthTokenSRO.ExpiresInUTC)
	{
		bRequestMade = true;
		FOnRefreshAuthRequestComplete OnRefreshAuthComplete = FOnRefreshAuthRequestComplete::CreateLambda([this](int32 InLocalUserNum, bool bInWasSuccessful, const FAuthTokenSRO& InAuthToken, const FString& ErrorStr)
		{
			bIsComplete = true;
			bWasSuccessful = bInWasSuccessful;
			if (bInWasSuccessful)
			{
				UE_LOG_ONLINE(Log, TEXT("Updated auth token for user %d"), InLocalUserNum);
				Subsystem->SROIdentity->UpdateUserAuth(InLocalUserNum, InAuthToken);
			}
			else
			{
				UE_LOG_ONLINE(Warning, TEXT("Logging out user %d due to failing to refresh auth token: %s"), InLocalUserNum, *ErrorStr);
				Subsystem->SROIdentity->Logout(InLocalUserNum);
			}
		});

		if (AuthTokenSRO.AuthType == ESROAuthTokenType::ClientCredentials)
		{
			Subsystem->SROIdentity->ClientAuth(OnRefreshAuthComplete);
		}
		else if (AuthTokenSRO.AuthType == ESROAuthTokenType::RefreshToken || AuthTokenSRO.AuthType == ESROAuthTokenType::AccessToken)
		{
			Subsystem->SROIdentity->RefreshAuth(LocalUserNum, AuthTokenSRO, OnRefreshAuthComplete);
		}
		else
		{
			UE_LOG_ONLINE(Warning, TEXT("Invalid token type for refresh auth request"));
			bIsComplete = true;
			bWasSuccessful = false;
		}
	}
}

void FOnlineAsyncTaskSRORefreshAuth::TriggerDelegates()
{
	FOnlineAsyncTaskBasic<FOnlineSubsystemSRO>::TriggerDelegates();
}

FString FOnlineAsyncTaskSRORefreshAuth::ToString() const
{
	return FString::Printf(TEXT("AsyncRefreshToken_%d"), LocalUserNum);
}
