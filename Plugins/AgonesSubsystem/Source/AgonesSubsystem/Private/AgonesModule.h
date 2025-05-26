// Copyright Epic Games, Inc. All Rights Reserved.

#include "Modules/ModuleManager.h"

AGONESSUBSYSTEM_API DECLARE_LOG_CATEGORY_EXTERN(LogAgones, Log, All);

class FAgonesSubsystemModule : public IModuleInterface
{
public:
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;
};
