// Copyright Epic Games, Inc. All Rights Reserved.
#include "CommonUserBasicPresence.h"
#include "CommonSessionSubsystem.h"
#include "Engine/GameInstance.h"
#include "Engine/LocalPlayer.h"
#include "CommonUserTypes.h"


#if COMMONUSER_OSSV1
#include "OnlineSubsystemUtils.h"
#include "Interfaces/OnlinePresenceInterface.h"
#else
#include "Online/OnlineServicesEngineUtils.h"
#include "Online/Presence.h"
#endif

DECLARE_LOG_CATEGORY_EXTERN(LogUserBasicPresence, Log, All);
DEFINE_LOG_CATEGORY(LogUserBasicPresence);

UCommonUserBasicPresence::UCommonUserBasicPresence()
{

}

void UCommonUserBasicPresence::Initialize(FSubsystemCollectionBase& Collection)
{
}

void UCommonUserBasicPresence::Deinitialize()
{

}