// Copyright Shattered Realms Online 2024

#pragma once

#include "CoreMinimal.h"
#include "Player/LyraPlayerState.h"
#include "MMOPlayerState.generated.h"

/**
 * 
 */
UCLASS()
class MMOCORERUNTIME_API AMMOPlayerState : public ALyraPlayerState
{
	GENERATED_BODY()
	
public:
	AMMOPlayerState(const FObjectInitializer& ObjectInitializer);

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	UFUNCTION(Server, Reliable, WithValidation)
	void SetSROCharacter(FGrpcSroCharacterCharacter InCharacterDetails);

	UFUNCTION(BlueprintImplementableEvent)
	void OnCharacterDetailsChanged();
	
protected:
	UFUNCTION()
	void OnRep_CharacterDetails();

private:
	// Stat attribute set used by this actor.
	UPROPERTY()
	TObjectPtr<const class UMMOStatSet> StatSet;

	UPROPERTY(ReplicatedUsing=OnRep_CharacterDetails)
	FGrpcSroCharacterCharacter CharacterDetails;
};
