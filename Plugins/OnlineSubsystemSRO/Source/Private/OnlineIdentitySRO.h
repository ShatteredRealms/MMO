// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "AuthTypes.h"
#include "OnlineIdentitySRO.h"
#include "OnlineSubsystemSROTypes.h"
#include "OnlineAccountSRO.h"
#include "PlatformHttp.h"
#include "Interfaces/OnlineIdentityInterface.h"
#include "Interfaces/IHttpRequest.h"
#include "OnlineSubsystemSROPackage.h"

#define SRO_PERM_PUBLIC_PROFILE "profile"

class FOnlineSubsystemSRO;
class FUserOnlineAccountSRO;

#define AUTH_TYPE_SRO TEXT("SRO")
#define SRO_AUTH_EXPIRED_CREDS TEXT("com.epicgames.sro.oauth.expiredcreds")

/**
 * Delegate fired after a SRO profile request has been completed
 *
 * @param LocalUserNum the controller number of the associated user
 * @param bWasSuccessful was the request successful
 * @param ErrorStr error associated with the request
 */
DECLARE_DELEGATE_ThreeParams(FOnProfileRequestComplete, int32 /*LocalUserNum*/, bool /*bWasSuccessful*/, const FString& /*ErrorStr*/);

/** Mapping from user id to internal online account info (only one per user) */
typedef TMap<FString, TSharedRef<FUserOnlineAccountSRO> > FUserOnlineAccountSROMap;


class FOnlineSubsystemSRO;

/**
 * Delegate fired after an exchange token to access/refresh token request has been completed
 *
 * @param LocalUserNum the controller number of the associated user
 * @param bWasSuccessful was the request successful
 * @param AuthToken the new auth token returned from the exchange
 * @param ErrorStr error associated with the request
 */
DECLARE_DELEGATE_FourParams(FOnExchangeRequestComplete, int32 /*LocalUserNum*/, bool /*bWasSuccessful*/, const FAuthTokenSRO& /*AuthToken*/, const FString& /*ErrorStr*/);

/**
 * Delegate fired after a refresh auth from existing token request has been completed
 *
 * @param LocalUserNum the controller number of the associated user
 * @param bWasSuccessful was the request successful
 * @param AuthToken the new auth token returned from the refresh 
 * @param ErrorStr error associated with the request
 */
DECLARE_DELEGATE_FourParams(FOnRefreshAuthRequestComplete, int32 /*LocalUserNum*/, bool /*bWasSuccessful*/, const FAuthTokenSRO& /*AuthToken*/, const FString& /*ErrorStr*/);

/**
 * SRO service implementation of the online identity interface
 */
class FOnlineIdentitySRO :
	public IOnlineIdentity
{
protected:
	/** Parent subsystem */
	FOnlineSubsystemSRO* SROSubsystem;
	/** Endpoint configurations retrieved from SRO discovery service */
	FSROOpenIDConfiguration Endpoints;
	/** Users that have been registered/authenticated */
	FUserOnlineAccountSROMap UserAccounts;
	/** Ids mapped to locally registered users */
	TMap<int32, FUniqueNetIdPtr > UserIds;
	/** Platform can know the client access token */
	bool bAccessTokenAvailableToPlatform = true;

	typedef TFunction<void(bool)> PendingLoginRequestCb;

	/** Server client id that this client will be engaging with */
	FString ServerClientId = "sro-gameserver";
	/** Server client secret this client with use for login */
	FString ServerClientSecret = "";

	/** Const details about communicating with SRO API */
	FSROLoginURL LoginURLDetails;
	
	/** Whether we have a registration in flight or not */
	bool bHasLoginOutstanding;

	/** Domains used for login, for cookie management */
	TArray<FString> LoginDomains;

public:
	// IOnlineIdentity
	virtual bool AutoLogin(int32 LocalUserNum) override;
	virtual TSharedPtr<FUserOnlineAccount> GetUserAccount(const FUniqueNetId& UserId) const override;
	virtual TArray<TSharedPtr<FUserOnlineAccount> > GetAllUserAccounts() const override;
	virtual FUniqueNetIdPtr GetUniquePlayerId(int32 LocalUserNum) const override;
	virtual FUniqueNetIdPtr CreateUniquePlayerId(uint8* Bytes, int32 Size) override;
	virtual FUniqueNetIdPtr CreateUniquePlayerId(const FString& Str) override;
	virtual ELoginStatus::Type GetLoginStatus(int32 LocalUserNum) const override;
	virtual ELoginStatus::Type GetLoginStatus(const FUniqueNetId& UserId) const override;
	virtual FString GetPlayerNickname(int32 LocalUserNum) const override;
	virtual FString GetPlayerNickname(const FUniqueNetId& UserId) const override;
	virtual FString GetAuthToken(int32 LocalUserNum) const override;
	virtual void GetUserPrivilege(const FUniqueNetId& UserId, EUserPrivileges::Type Privilege, const FOnGetUserPrivilegeCompleteDelegate& Delegate, EShowPrivilegeResolveUI ShowResolveUI=EShowPrivilegeResolveUI::Default) override;
	virtual FPlatformUserId GetPlatformUserIdFromUniqueNetId(const FUniqueNetId& UniqueNetId) const override;
	virtual FString GetAuthType() const override;
	virtual void RevokeAuthToken(const FUniqueNetId& UserId, const FOnRevokeAuthTokenCompleteDelegate& Delegate) override;
	virtual bool Login(int32 LocalUserNum, const FOnlineAccountCredentials& AccountCredentials) override;
	virtual bool Logout(int32 LocalUserNum) override;

	int32 GetLocalUserNumFromUniqueNetId(const FUniqueNetId& NetId) const;

	const FSROOpenIDConfiguration& GetEndpoints()
	{
		return Endpoints;
	}

	void AddUser(TSharedRef<FUserOnlineAccountSRO> User, int32 LocalUserNum, bool bAsyncUpdateAuth = true);
	void UpdateUserAuth(int32 LocalUserNum, const FAuthTokenSRO& InAuthToken, bool bAsyncUpdateAuth = true);
	
	FOnlineIdentitySRO(FOnlineSubsystemSRO* InSubsystem);

	/**
	 * Destructor
	 */
	virtual ~FOnlineIdentitySRO()
	{
	}
	

PACKAGE_SCOPE:
	
	/** Checks config to know if we should request offline access (Server Auth code on mobile or refresh token on REST implementation)*/
	static bool ShouldRequestOfflineAccess();

	/**
	 * Retrieve the profile for a given user and access token
	 *
	 * @param LocalUserNum the controller number of the associated user
	 * @param AccessToken associated access token to make the request
	 * @param InCompletionDelegate delegate to fire when request is completed
	 */
	void ProfileRequest(int32 LocalUserNum, const FAuthTokenSRO& InAuthToken, FOnProfileRequestComplete& InCompletionDelegate);


	/**
	 * Retrieve auth endpoints from SRO discovery service
	 *
	 * @param LoginCb the login function to call after this request completes
	 */
	void RetrieveDiscoveryDocument(PendingLoginRequestCb&& LoginCb);


	void UpdateLoginURLWithEndpointData() { LoginURLDetails.LoginUrl = Endpoints.AuthEndpoint; };

	/**
	 * Login with an existing token
	 * If an exchange token has been given, it will convert to access/refresh before continuing login
	 * If an older access token has been given, it will refresh the token before continuing login
	 *
	 * @param LocalUserNum id of the local user initiating the request
	 * @param InToken exchange or refresh token already received from SRO
	 * @param InCompletionDelegate delegate to fire when operation completes
	 */
	void Login(int32 LocalUserNum, const FAuthTokenSRO& InToken, const FOnLoginCompleteDelegate& InCompletionDelegate);

	/** @return the login configuration details */
	const FSROLoginURL& GetLoginURLDetails() const { return LoginURLDetails; }

	/**
	 * Exchange the SRO auth token for actual user access/refresh tokens
	 *
	 * @param LocalUserNum id of the local user initiating the request
	 * @param InExchangeToken token received from user consent login flow
	 * @param InCompletionDelegate delegate to fire when operation completes
	 */
	void ExchangeCode(int32 LocalUserNum, const FAuthTokenSRO& InExchangeToken, FOnExchangeRequestComplete& InCompletionDelegate);

	/**
	 * Refresh an existing SRO auth token
	 *
	 * @param LocalUserNum id of the local user initiating the request
	 * @param InAuthToken existing valid auth token with refresh token included
	 * @param InCompletionDelegate delegate to fire when operation completes
	 */
	void RefreshAuth(int32 LocalUserNum, const FAuthTokenSRO& InAuthToken, FOnRefreshAuthRequestComplete& InCompletionDelegate);
	
	/**
	 * Login with server client credentials 
	 *
	 * @param InCompletionDelegate delegate to fire when operation completes
	 */
	void ClientAuth(FOnRefreshAuthRequestComplete& InCompletionDelegate);

	void RegisterAsyncTokenUpdate(int32 LocalUserNum);
	
private:
	/**
	 * Delegate fired when the discover service request has completed
	 */
	virtual void DiscoveryRequest_HttpRequestComplete(FHttpRequestPtr HttpRequest, FHttpResponsePtr HttpResponse, bool bSucceeded, PendingLoginRequestCb LoginCb);
	
	/**
	 * Delegate called when a user /me request from SRO is complete
	 */
	void MeUser_HttpRequestComplete(FHttpRequestPtr HttpRequest, FHttpResponsePtr HttpResponse, bool bSucceeded, int32 InLocalUserNum, FAuthTokenSRO InAuthToken, FOnProfileRequestComplete InCompletionDelegate);


	void OnLoginAttemptComplete(int32 LocalUserNum, const FString& ErrorStr);

	/**
	 * Delegate called when a token exchange has completed
	 */
	void ExchangeRequest_HttpRequestComplete(FHttpRequestPtr HttpRequest, FHttpResponsePtr HttpResponse, bool bSucceeded, int32 InLocalUserNum, FAuthTokenSRO InExchangeToken, FOnExchangeRequestComplete InCompletionDelegate);

	/**
	 * Delegate called when a refresh auth request has completed
	 */
	void RefreshAuthRequest_HttpRequestComplete(FHttpRequestPtr HttpRequest, FHttpResponsePtr HttpResponse, bool bSucceeded, int32 InLocalUserNum, FAuthTokenSRO InOldAuthToken, FOnRefreshAuthRequestComplete InCompletionDelegate);
	
	/**
	 * Delegate called when a client credentials request has completed
	 */
	void ClientCredentialsRequest_HttpRequestComplete(FHttpRequestPtr HttpRequest, FHttpResponsePtr HttpResponse, bool bSucceeded, FOnRefreshAuthRequestComplete InCompletionDelegate);

	/**
	 * Delegate fired when the internal call to Login() with AccessToken is specified
	 *
	 * @param LocalUserNum index of the local user initiating the request
	 * @param bWasSuccessful was the login call successful
	 * @param UserId user id of the logged in user, or null if login failed
	 * @param Error error string if applicable
	 */
	void OnAccessTokenLoginComplete(int32 LocalUserNum, bool bWasSuccessful, const FUniqueNetId& UserId, const FString& Error);

	/**
	 * Delegate fired when the call to ShowLoginUI completes 
	 */
	void OnExternalUILoginComplete(FUniqueNetIdPtr UniqueId, const int ControllerIndex, const FOnlineError& Error);

	FString ValidateToken(const FAuthTokenSRO& InToken);
};

typedef TSharedPtr<FOnlineIdentitySRO, ESPMode::ThreadSafe> FOnlineIdentitySROPtr;
