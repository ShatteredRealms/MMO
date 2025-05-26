// Copyright Epic Games, Inc. All Rights Reserved.

#include "OnlineAccountSRO.h"
#include "OnlineSubsystemSROPrivate.h"

bool FUserOnlineAccountSRO::Parse(const FAuthTokenSRO& InAuthToken, const FString& InJsonStr)
{
	bool bResult = false;
	if (InAuthToken.IsValid())
	{
		if (!InJsonStr.IsEmpty())
		{
			TSharedPtr<FJsonObject> JsonUser;
			TSharedRef< TJsonReader<> > JsonReader = TJsonReaderFactory<>::Create(InJsonStr);

			if (FJsonSerializer::Deserialize(JsonReader, JsonUser) &&
				JsonUser.IsValid())
			{
				bResult = Parse(InAuthToken, JsonUser);
			}
			else
			{
				UE_LOG_ONLINE(Warning, TEXT("FUserOnlineAccountSRO: Can't deserialize payload=%s"), *InJsonStr);
			}
		}
		else
		{
			UE_LOG_ONLINE(Warning, TEXT("FUserOnlineAccountSRO: Empty Json string"));
		}
	}
	else
	{
		UE_LOG_ONLINE(Warning, TEXT("FUserOnlineAccountSRO: Invalid auth token"));
	}

	return bResult;
}

bool FUserOnlineAccountSRO::Parse(const FAuthTokenSRO& InAuthToken, TSharedPtr<FJsonObject> InJsonObject)
{
	bool bResult = false;
	if (InAuthToken.IsValid())
	{
		if (InJsonObject.IsValid())
		{
			if (FromJson(InJsonObject))
			{
				if (!UserId.IsEmpty())
				{
					UserIdPtr = FUniqueNetIdSRO::Create(UserId);

					// update the access token
					AuthToken = InAuthToken;

					bResult = true;
				}
				else
				{
					UE_LOG_ONLINE(Warning, TEXT("FUserOnlineAccountSRO: Missing user id in json object"));
				}
			}
			else
			{
				UE_LOG_ONLINE(Warning, TEXT("FUserOnlineAccountSRO: Invalid json object"));
			}
		}	
		else
		{
			UE_LOG_ONLINE(Warning, TEXT("FUserOnlineAccountSRO: Invalid json object pointer"));
		}
	}
	else
	{
		UE_LOG_ONLINE(Warning, TEXT("FUserOnlineAccountSRO: Invalid auth token"));
	}

	return bResult;
}

FUniqueNetIdRef FUserOnlineAccountSRO::GetUserId() const
{
	return UserIdPtr;
}

FString FUserOnlineAccountSRO::GetRealName() const
{
	return Username;
}

FString FUserOnlineAccountSRO::GetDisplayName(const FString& Platform) const
{
	return Username;
}

bool FUserOnlineAccountSRO::GetUserAttribute(const FString& AttrName, FString& OutAttrValue) const
{
	return GetAccountData(AttrName, OutAttrValue);
}

bool FUserOnlineAccountSRO::SetUserAttribute(const FString& AttrName, const FString& AttrValue)
{
	return SetAccountData(AttrName, AttrValue);
}

FString FUserOnlineAccountSRO::GetAccessToken() const
{
	return AuthToken.AccessToken;
}

bool FUserOnlineAccountSRO::GetAuthAttribute(const FString& AttrName, FString& OutAttrValue) const
{
	return AuthToken.GetAuthData(AttrName, OutAttrValue);
}

