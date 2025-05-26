// Copyright Epic Games, Inc. All Rights Reserved.

#include "OnlineSubsystemSRO.h"

#include "OnlineAsyncTaskManagerSRO.h"
#include "OnlineSubsystemSROPrivate.h"
#include "OnlineIdentitySRO.h"
#include "OnlineExternalUISRO.h"
#include "OnlineSessionSRO.h"
#include "Misc/ConfigCacheIni.h"
#include "Misc/CommandLine.h"
#include "Stats/Stats.h"

#define SRO_CLIENTAUTH_ID TEXT("ClientId")
#define SRO_SERVERAUTH_ID TEXT("ServerClientId")


FOnlineSubsystemSRO::FOnlineSubsystemSRO(FName InInstanceName)
	: FOnlineSubsystemImpl(SRO_SUBSYSTEM, InInstanceName)
	, bPlatformRequiresClientId(false)
	, bPlatformAllowsClientIdOverride(false)
	, bPlatformRequiresServerClientId(false)
{
	bPlatformRequiresClientId = true;
	bPlatformAllowsClientIdOverride = true;
	bPlatformRequiresServerClientId = true;
}

FOnlineSubsystemSRO::~FOnlineSubsystemSRO()
{
}

bool FOnlineSubsystemSRO::Init()
{
	static FString ConfigSection(TEXT("OnlineSubsystemSRO"));
	if (bPlatformRequiresClientId)
	{
		if (!GConfig->GetString(*ConfigSection, SRO_CLIENTAUTH_ID, ClientId, GEngineIni))
		{
			UE_LOG_ONLINE(Warning, TEXT("Missing ClientId= in [%s] of DefaultEngine.ini"), *ConfigSection);
		}
	}
	
	if (bPlatformRequiresServerClientId)
	{
		if (!GConfig->GetString(*ConfigSection, SRO_SERVERAUTH_ID, ServerClientId, GEngineIni))
		{
			UE_LOG_ONLINE(Warning, TEXT("Missing ServerClientId= in [%s] of DefaultEngine.ini"), *ConfigSection);
		}
	}
	
	FString ConfigOverride;
	FSROAuthConfig NewConfig;
	if (GetConfigurationDelegate().ExecuteIfBound(ConfigOverride, NewConfig))
	{
		if (!NewConfig.Backend.IsEmpty())
		{
			FString IniSection = ConfigSection + FString(TEXT(" ")) + NewConfig.Backend;
			FPlatformMisc::LowLevelOutputDebugStringf(TEXT("FOnlineSubsystemSRO::Init IniSection:%s"), *IniSection);

			FString NewClientAuthId;
			if (GConfig->GetString(*IniSection, SRO_CLIENTAUTH_ID, NewClientAuthId, GEngineIni) && !NewClientAuthId.IsEmpty())
			{
				if (bPlatformRequiresClientId && bPlatformAllowsClientIdOverride)
				{
					FPlatformMisc::LowLevelOutputDebugStringf(TEXT("FOnlineSubsystemSRO::Init ClientId:%s"), *NewClientAuthId);
					ClientId = NewClientAuthId;
				}
				else
				{
					FPlatformMisc::LowLevelOutputDebugStringf(TEXT("FOnlineSubsystemSRO::Init Platform does not support ClientId override"), *NewClientAuthId);
				}
			}

			FString NewServerAuthId;
			if (GConfig->GetString(*IniSection, SRO_SERVERAUTH_ID, NewServerAuthId, GEngineIni) && !NewServerAuthId.IsEmpty())
			{
				if (bPlatformRequiresServerClientId)
				{
					FPlatformMisc::LowLevelOutputDebugStringf(TEXT("FOnlineSubsystemSRO::Init ServerClientId:%s"), *NewServerAuthId);
					ServerClientId = NewServerAuthId;
				}
				else
				{
					FPlatformMisc::LowLevelOutputDebugStringf(TEXT("FOnlineSubsystemSRO::Init Platform does not support ServerClientId override"), *NewServerAuthId);
				}
			}
		}
	}
	else
	{
		FPlatformMisc::LowLevelOutputDebugStringf(TEXT("GetConfigurationDelegate was not bound!"));
	}
	
	OnlineAsyncTaskThreadRunnable.Reset(new FOnlineAsyncTaskManagerSRO());
	OnlineAsyncTaskThread.Reset(FRunnableThread::Create(OnlineAsyncTaskThreadRunnable.Get(), *FString::Printf(TEXT("OnlineAsyncTaskThread %s"), *InstanceName.ToString())));

	
	
	SROIdentity = MakeShareable(new FOnlineIdentitySRO(this));
	SROExternalUI = MakeShareable(new FOnlineExternalUISRO(this));
	SROSession = MakeShareable(new FOnlineSessionSRO(this));

	return true;
}

bool FOnlineSubsystemSRO::Shutdown()
{
	UE_LOG_ONLINE(Display, TEXT("FOnlineSubsystemSRO::Shutdown()"));
	FOnlineSubsystemImpl::Shutdown();
#define DESTRUCT_INTERFACE(Interface) \
	if (Interface.IsValid()) \
	{ \
		ensure(Interface.IsUnique()); \
		Interface = nullptr; \
	}

	DESTRUCT_INTERFACE(SROExternalUI);
	DESTRUCT_INTERFACE(SROIdentity);

#undef DESTRUCT_INTERFACE

	OnlineAsyncTaskThread.Reset();
	OnlineAsyncTaskThreadRunnable.Reset();

	return true;
}

FOnlineSubsystemSRO::FSROConfigurationDelegate& FOnlineSubsystemSRO::GetConfigurationDelegate()
{
	static FSROConfigurationDelegate Delegate;
	return Delegate;
}

bool FOnlineSubsystemSRO::Tick(float DeltaTime)
{
	QUICK_SCOPE_CYCLE_COUNTER(STAT_FOnlineSubsystemSRO_Tick);

	if (!FOnlineSubsystemImpl::Tick(DeltaTime))
	{
		return false;
	}

	if (OnlineAsyncTaskThreadRunnable)
	{
		OnlineAsyncTaskThreadRunnable->GameTick();
	}
	
	return true;
}

FString FOnlineSubsystemSRO::GetAppId() const
{
	return ClientId;
}

bool FOnlineSubsystemSRO::Exec(UWorld* InWorld, const TCHAR* Cmd, FOutputDevice& Ar)
{
	if (FOnlineSubsystemImpl::Exec(InWorld, Cmd, Ar))
	{
		return true;
	}
	return false;
}

IOnlineSessionPtr FOnlineSubsystemSRO::GetSessionInterface() const
{
	return SROSession;
}

IOnlineFriendsPtr FOnlineSubsystemSRO::GetFriendsInterface() const
{
	return nullptr;
}

IOnlinePartyPtr FOnlineSubsystemSRO::GetPartyInterface() const
{
	return nullptr;
}

IOnlineGroupsPtr FOnlineSubsystemSRO::GetGroupsInterface() const
{
	return nullptr;
}

IOnlineSharedCloudPtr FOnlineSubsystemSRO::GetSharedCloudInterface() const
{
	return nullptr;
}

IOnlineUserCloudPtr FOnlineSubsystemSRO::GetUserCloudInterface() const
{
	return nullptr;
}

IOnlineLeaderboardsPtr FOnlineSubsystemSRO::GetLeaderboardsInterface() const
{
	return nullptr;
}

IOnlineVoicePtr FOnlineSubsystemSRO::GetVoiceInterface() const
{
	return nullptr;
}

IOnlineExternalUIPtr FOnlineSubsystemSRO::GetExternalUIInterface() const	
{
	return SROExternalUI;
}

IOnlineTimePtr FOnlineSubsystemSRO::GetTimeInterface() const
{
	return nullptr;
}

IOnlineIdentityPtr FOnlineSubsystemSRO::GetIdentityInterface() const
{
	return SROIdentity;
}

IOnlineTitleFilePtr FOnlineSubsystemSRO::GetTitleFileInterface() const
{
	return nullptr;
}

IOnlineEntitlementsPtr FOnlineSubsystemSRO::GetEntitlementsInterface() const
{
	return nullptr;
}

IOnlineEventsPtr FOnlineSubsystemSRO::GetEventsInterface() const
{
	return nullptr;
}

IOnlineAchievementsPtr FOnlineSubsystemSRO::GetAchievementsInterface() const
{
	return nullptr;
}

IOnlineSharingPtr FOnlineSubsystemSRO::GetSharingInterface() const
{
	return nullptr;
}

IOnlineUserPtr FOnlineSubsystemSRO::GetUserInterface() const
{
	return nullptr;
}

IOnlineMessagePtr FOnlineSubsystemSRO::GetMessageInterface() const
{
	return nullptr;
}

IOnlinePresencePtr FOnlineSubsystemSRO::GetPresenceInterface() const
{
	return nullptr;
}

IOnlineChatPtr FOnlineSubsystemSRO::GetChatInterface() const
{
	return nullptr;
}

IOnlineStatsPtr FOnlineSubsystemSRO::GetStatsInterface() const
{
	return nullptr;
}

IOnlineTurnBasedPtr FOnlineSubsystemSRO::GetTurnBasedInterface() const
{
	return nullptr;
}

IOnlineTournamentPtr FOnlineSubsystemSRO::GetTournamentInterface() const
{
	return nullptr;
}

FText FOnlineSubsystemSRO::GetOnlineServiceName() const
{
	return NSLOCTEXT("OnlineSubsystemSRO", "OnlineServiceName", "SRO");
}

