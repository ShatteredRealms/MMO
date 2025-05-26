// Copyright Shattered Realms Online 2024


#include "GameModes/MMOGameMode.h"

#include "MMOCoreRuntimeModule.h"
#include "Character/MMOCharacter.h"
#include "Player/MMOPlayerController.h"
#include "Player/MMOPlayerState.h"

AMMOGameMode::AMMOGameMode(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	PlayerControllerClass = AMMOPlayerController::StaticClass();
	PlayerStateClass = AMMOPlayerState::StaticClass();
	DefaultPawnClass = AMMOCharacter::StaticClass();
}


void AMMOGameMode::HandleStartingNewPlayer_Implementation(APlayerController* NewPlayer)
{
	Super::HandleStartingNewPlayer_Implementation(NewPlayer);
	if (IsExperienceLoaded())
	{
		auto PlayerState = NewPlayer->GetPlayerState<AMMOPlayerState>();
		const auto Data = CachedCharacterData.Find(NewPlayer->NetConnection->PlayerId);
		if (PlayerState && Data)
		{
			UE_LOG(LogMMO, Log, TEXT("HandleStartingNewPlayer_Implementation: Setting SROCharacter for player %s"), *GetNameSafe(NewPlayer));
			PlayerState->SetSROCharacter(*Data);
		}
		else
		{
			UE_LOG(LogMMO, Error, TEXT("HandleStartingNewPlayer_Implementation: Failed to find SROCharacter for player %s"), *GetNameSafe(NewPlayer));
		}
	}
}
