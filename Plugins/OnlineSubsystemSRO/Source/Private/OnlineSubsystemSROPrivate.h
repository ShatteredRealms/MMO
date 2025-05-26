// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Modules/ModuleManager.h"

#include "Http.h"

#include "OnlineSubsystemSRO.h"
#include "OnlineSubsystemSROModule.h"

/** pre-pended to all SRO logging */
#undef ONLINE_LOG_PREFIX
#define ONLINE_LOG_PREFIX TEXT("SRO: ")
