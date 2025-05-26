// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "OnlineSubsystemSRO.h"
#include "OnlineExternalUISRO.h"
#include "OnlineSubsystemSROPackage.h"
#include "Interfaces/IHttpRequest.h"
#include "Interfaces/OnlineExternalUIInterface.h"

class ILoginFlowManager;
class FOnlineSubsystemSRO;

/** 
 * Implementation for the SRO external UIs
 */
class FOnlineExternalUISRO : public IOnlineExternalUI
{

PACKAGE_SCOPE:

	/** 
	 * Constructor
	 * @param InSubsystem The owner of this external UI interface.
	 */
	FOnlineExternalUISRO(FOnlineSubsystemSRO* InSubsystem);

	/** Reference to the owning subsystem */
	FOnlineSubsystemSRO* SROSubsystem;

public:

	/**
	 * Destructor.
	 */
	virtual ~FOnlineExternalUISRO()
	{
	}

	// IOnlineExternalUI
	virtual bool ShowLoginUI(const int ControllerIndex, bool bShowOnlineOnly, bool bShowSkipButton, const FOnLoginUIClosedDelegate& Delegate = FOnLoginUIClosedDelegate()) override;
	virtual bool ShowAccountCreationUI(const int ControllerIndex, const FOnAccountCreationUIClosedDelegate& Delegate = FOnAccountCreationUIClosedDelegate()) override { /** NYI */ return false; }
	virtual bool ShowFriendsUI(int32 LocalUserNum) override;
	virtual bool ShowInviteUI(int32 LocalUserNum, FName SessionName = NAME_GameSession) override;
	virtual bool ShowAchievementsUI(int32 LocalUserNum) override;
	virtual bool ShowLeaderboardUI(const FString& LeaderboardName) override;
	virtual bool ShowWebURL(const FString& Url, const FShowWebUrlParams& ShowParams, const FOnShowWebUrlClosedDelegate& Delegate = FOnShowWebUrlClosedDelegate()) override;
	virtual bool CloseWebURL() override;
	virtual bool ShowProfileUI(const FUniqueNetId& Requestor, const FUniqueNetId& Requestee, const FOnProfileUIClosedDelegate& Delegate = FOnProfileUIClosedDelegate()) override;
	virtual bool ShowAccountUpgradeUI(const FUniqueNetId& UniqueId) override;
	virtual bool ShowStoreUI(int32 LocalUserNum, const FShowStoreParams& ShowParams, const FOnShowStoreUIClosedDelegate& Delegate = FOnShowStoreUIClosedDelegate()) override;
	virtual bool ShowSendMessageUI(int32 LocalUserNum, const FShowSendMessageParams& ShowParams, const FOnShowSendMessageUIClosedDelegate& Delegate = FOnShowSendMessageUIClosedDelegate()) override;

private: 

	/**
	 * Delegate fired when redirect URLs from the login flow are passed back for parsing
	 * We are looking for the success or error completion state from SRO to grab the access token or complete the flow
	 *
	 * @param RedirectURL URL received from the login flow for parsing
	 */
	FLoginFlowResult OnLoginRedirectURL(const FString& RedirectURL);

	/**
	 * Delegate fired when the login flow is complete
	 *
	 * @param Result final result of the login flow action
	 * @param ControllerIndex index of the local user initiating the request
	 * @param Delegate UI closed delegate to fire, completing the external UIs part in the login process
	 */
	void OnExternalLoginFlowComplete(const FLoginFlowResult& Result, int ControllerIndex, const FOnLoginUIClosedDelegate Delegate);

	/**
	 * Delegate fired when the SRO identity interface has completed login using the token retrieved from the login flow
	 *
	 * @param LocalUserNum index of the local user initiating the request
	 * @param bWasSuccessful was the login call successful
	 * @param UserId user id of the logged in user, or null if login failed
	 * @param Error error string if applicable
	 * @param Delegate UI closed delegate to fire, completing the external UIs part in the login process
	 */
	void OnAccessTokenLoginComplete(int32 LocalUserNum, bool bWasSuccessful, const FUniqueNetId& UserId, const FString& Error, FOnLoginUIClosedDelegate Delegate);

	FString EOSClientId;
	FString EOSClientSecret;

#if UE_BUILD_DEVELOPMENT
	/** LoginFlowManager used to show login UI */
	TSharedPtr<ILoginFlowManager> LoginFlowManager;

	DECLARE_DELEGATE(FOnPopupDismissed);
	
	/** Delegate called when login popup dismissed */
	FOnPopupDismissed OnLoginPopupDismissed;

	/** Delegate called when account creation popup is dismissed */
	FOnPopupDismissed OnCreationPopupDismissed;
#endif
	
};

typedef TSharedPtr<FOnlineExternalUISRO, ESPMode::ThreadSafe> FOnlineExternalUISROPtr;
