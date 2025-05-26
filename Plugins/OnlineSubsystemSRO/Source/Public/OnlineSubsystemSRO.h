// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "OnlineSubsystem.h"
#include "OnlineSubsystemImpl.h"
#include "OnlineSubsystemSROPackage.h"

#ifndef SRO_SUBSYSTEM
const FName SRO_SUBSYSTEM(TEXT("SRO"));
#endif

/** Forward declarations of all interface classes */
typedef TSharedPtr<class FOnlineIdentitySRO, ESPMode::ThreadSafe> FOnlineIdentitySROPtr;
typedef TSharedPtr<class FOnlineFriendsSRO, ESPMode::ThreadSafe> FOnlineFriendsSROPtr;
typedef TSharedPtr<class FOnlineSharingSRO, ESPMode::ThreadSafe> FOnlineSharingSROPtr;
typedef TSharedPtr<class FOnlineUserSRO, ESPMode::ThreadSafe> FOnlineUserSROPtr;
typedef TSharedPtr<class FOnlineExternalUISRO, ESPMode::ThreadSafe> FOnlineExternalUISROPtr;
typedef TSharedPtr<class FOnlineIdentitySRO, ESPMode::ThreadSafe> FOnlineIdentitySROPtr;
typedef TSharedPtr<class FOnlineSessionSRO, ESPMode::ThreadSafe> FOnlineSessionSROPtr;

/**
 * Configuration structure for overriding auth id parameters required by SRO
 */
class FSROAuthConfig
{
public:
	FSROAuthConfig() {}

	/** 
	 * Backend Name
	 *
	 * Added to [OnlineSubsystemSRO <Backend>] to search the ini for specific overrides
	 */
	FString Backend;
};

/**
 *	OnlineSubsystemSRO - Implementation of the online subsystem for SRO services
 */
class ONLINESUBSYSTEMSRO_API FOnlineSubsystemSRO 
	: public FOnlineSubsystemImpl
{
public:

	// IOnlineSubsystem Interface
	virtual IOnlineSessionPtr GetSessionInterface() const override;
	virtual IOnlineFriendsPtr GetFriendsInterface() const override;
	virtual IOnlinePartyPtr GetPartyInterface() const override;
	virtual IOnlineGroupsPtr GetGroupsInterface() const override;
	virtual IOnlineSharedCloudPtr GetSharedCloudInterface() const override;
	virtual IOnlineUserCloudPtr GetUserCloudInterface() const override;
	virtual IOnlineLeaderboardsPtr GetLeaderboardsInterface() const override;
	virtual IOnlineVoicePtr GetVoiceInterface() const override;
	virtual IOnlineExternalUIPtr GetExternalUIInterface() const override;
	virtual IOnlineTimePtr GetTimeInterface() const override;
	virtual IOnlineIdentityPtr GetIdentityInterface() const override;
	virtual IOnlineTitleFilePtr GetTitleFileInterface() const override;
	virtual IOnlineEntitlementsPtr GetEntitlementsInterface() const override;
	virtual IOnlineStoreV2Ptr GetStoreV2Interface() const override { return nullptr; }
	virtual IOnlinePurchasePtr GetPurchaseInterface() const override { return nullptr; }
	virtual IOnlineEventsPtr GetEventsInterface() const override;
	virtual IOnlineAchievementsPtr GetAchievementsInterface() const override;
	virtual IOnlineSharingPtr GetSharingInterface() const override;
	virtual IOnlineUserPtr GetUserInterface() const override;
	virtual IOnlineMessagePtr GetMessageInterface() const override;
	virtual IOnlinePresencePtr GetPresenceInterface() const override;
	virtual IOnlineChatPtr GetChatInterface() const override;
	virtual IOnlineStatsPtr GetStatsInterface() const override;
	virtual IOnlineTurnBasedPtr GetTurnBasedInterface() const override;
	virtual IOnlineTournamentPtr GetTournamentInterface() const override;
	virtual FText GetOnlineServiceName() const override;
	virtual bool Init() override;
	virtual bool Shutdown() override;
	virtual FString GetAppId() const override;
	virtual bool Exec(class UWorld* InWorld, const TCHAR* Cmd, FOutputDevice& Ar) override;

	// FTickerBaseObject

	virtual bool Tick(float DeltaTime) override;

	// FOnlineSubsystemSRO

	/**
	 * Destructor
	 */
	virtual ~FOnlineSubsystemSRO();

	/**
	 * Delegate fired at initialization allowing the application to override the configuration parameters 
	 *
	 * @param ConfigOverride unused
	 * @param OutConfig config parameters needed to be filled in in order to override the default engine ini settings
	 */
	DECLARE_DELEGATE_TwoParams(FSROConfigurationDelegate, const FString& /*ConfigOverride*/, FSROAuthConfig& /*OutConfig*/);
	static FOnlineSubsystemSRO::FSROConfigurationDelegate& GetConfigurationDelegate();

PACKAGE_SCOPE:

	/** Only the factory makes instances */
	FOnlineSubsystemSRO();
	FOnlineSubsystemSRO(FName InInstanceName);

	/** @return the backend server client id */
	FString GetClientId() const { return ClientId; }

	/** @return the backend server client id */
	FString GetServerClientId() const { return ServerClientId; }

	/** Return the async task manager owned by this subsystem */
	class FOnlineAsyncTaskManagerSRO* GetAsyncTaskManager() { return OnlineAsyncTaskThreadRunnable.Get(); }

	/** SRO implementation of identity interface */
	FOnlineIdentitySROPtr SROIdentity;

	/** SRO implementation of the external ui */
	FOnlineExternalUISROPtr SROExternalUI;

	/** SRO implementation of session interface */
	FOnlineSessionSROPtr SROSession;

protected:

	/** The client id given to us by SRO Dashboard */
	FString ClientId = "sro-gameclient";

	/** Server client id that this client will be engaging with */
	FString ServerClientId = "sro-gameserver";
	
	/** Does the implementation need a Client Id configured*/
	bool bPlatformRequiresClientId;

	/** Does the implementation need a Client Id configured*/
	bool bPlatformAllowsClientIdOverride;

	/** Does the implementation need a Server Client Id configured*/
	bool bPlatformRequiresServerClientId;

private:
	/** Online async task runnable */
	TUniquePtr<class FOnlineAsyncTaskManagerSRO> OnlineAsyncTaskThreadRunnable;

	/** Online async task thread */
	TUniquePtr<FRunnableThread> OnlineAsyncTaskThread;

};

typedef TSharedPtr<FOnlineSubsystemSRO, ESPMode::ThreadSafe> FOnlineSubsystemSROPtr;
