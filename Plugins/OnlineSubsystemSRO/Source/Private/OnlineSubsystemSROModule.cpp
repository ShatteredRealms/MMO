// Copyright Epic Games, Inc. All Rights Reserved.

#include "OnlineSubsystemSROModule.h"
#include "OnlineSubsystemSROPrivate.h"

IMPLEMENT_MODULE(FOnlineSubsystemSROModule, OnlineSubsystemSRO);

/**
 * Class responsible for creating instance(s) of the subsystem
 */
class FOnlineFactorySRO : public IOnlineFactory
{
public:

	FOnlineFactorySRO() {}
	virtual ~FOnlineFactorySRO() {}

	virtual IOnlineSubsystemPtr CreateSubsystem(FName InstanceName)
	{
		FOnlineSubsystemSROPtr OnlineSub = MakeShared<FOnlineSubsystemSRO, ESPMode::ThreadSafe>(InstanceName);
		if (OnlineSub->IsEnabled())
		{
			if(!OnlineSub->Init())
			{
				UE_LOG_ONLINE(Warning, TEXT("SRO API failed to initialize!"));
				OnlineSub->Shutdown();
				OnlineSub = nullptr;
			}
		}
		else
		{
			UE_LOG_ONLINE(Warning, TEXT("SRO API disabled!"));
			OnlineSub->Shutdown();
			OnlineSub = nullptr;
		}

		return OnlineSub;
	}
};

void FOnlineSubsystemSROModule::StartupModule()
{
	UE_LOG_ONLINE(Log, TEXT("SRO Startup!"));

	SROFactory = new FOnlineFactorySRO();

	// Create and register our singleton factory with the main online subsystem for easy access
	FOnlineSubsystemModule& OSS = FModuleManager::GetModuleChecked<FOnlineSubsystemModule>("OnlineSubsystem");
	OSS.RegisterPlatformService(SRO_SUBSYSTEM, SROFactory);
}

void FOnlineSubsystemSROModule::ShutdownModule()
{
	UE_LOG_ONLINE(Log, TEXT("SRO Shutdown!"));

	FOnlineSubsystemModule& OSS = FModuleManager::GetModuleChecked<FOnlineSubsystemModule>("OnlineSubsystem");
	OSS.UnregisterPlatformService(SRO_SUBSYSTEM);

	delete SROFactory;
	SROFactory = nullptr;
}