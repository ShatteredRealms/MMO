// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Modules/ModuleInterface.h"
#include "OnlineSubsystemSROPackage.h"

/**
 * Online subsystem module class (SRO Implementation)
 * Code related to the loading and handling of the SRO module.
 */
class FOnlineSubsystemSROModule : public IModuleInterface
{
private:

	/** Class responsible for creating instance(s) of the subsystem */
	class FOnlineFactorySRO* SROFactory;

public:	

	// IModuleInterface 

	virtual void StartupModule() override;
	virtual void ShutdownModule() override;
	virtual bool SupportsDynamicReloading() override
	{
		return false;
	}
	virtual bool SupportsAutomaticShutdown() override
	{
		return false;
	}

	// FOnlineSubsystemSROModule

	/**
	 * Constructor
	 */
	FOnlineSubsystemSROModule() :
		SROFactory(nullptr)
	{}

	/**
	 * Destructor
	 */
	virtual ~FOnlineSubsystemSROModule() 
	{}
};


