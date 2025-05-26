// Copyright Shattered Realms Online 2024

#pragma once

#include "CoreMinimal.h"
#include "GameModes/LyraGameMode.h"
#include "MMOGameMode.generated.h"

/**
 * 
 */
UCLASS()
class MMOCORERUNTIME_API AMMOGameMode : public ALyraGameMode
{
	GENERATED_BODY()
	AMMOGameMode(const FObjectInitializer& ObjectInitializer);

	virtual void HandleStartingNewPlayer_Implementation(APlayerController* NewPlayer) override;
};
