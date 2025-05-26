#pragma once

#include "Logging/LogMacros.h"
#include "Modules/ModuleManager.h"

COMMONSRO_API DECLARE_LOG_CATEGORY_EXTERN(LogCommonSRO, Log, All)

class COMMONSRO_API FCommonSROModule : public IModuleInterface
{
public:

	/** IModuleInterface implementation */
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;
};
