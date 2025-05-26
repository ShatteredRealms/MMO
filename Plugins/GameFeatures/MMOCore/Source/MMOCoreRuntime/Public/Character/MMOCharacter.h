// Copyright Shattered Realms Online 2024

#pragma once

#include "CoreMinimal.h"
#include "Character/LyraCharacterWithAbilities.h"
#include "MMOCharacter.generated.h"

class UTargetComponent;
class UMMOCombatComponent;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnCharacterDetailsChanged, const FGrpcSroCharacterCharacter&, NewCharacterDetails, const FGrpcSroCharacterCharacter&, OldCharacterDetails);

/**
 * 
 */
UCLASS()
class MMOCORERUNTIME_API AMMOCharacter : public ALyraCharacterWithAbilities
{
	GENERATED_BODY()

public:
	AMMOCharacter(const FObjectInitializer& ObjectInitializer);

	void SetCharacterDetails(const FGrpcSroCharacterCharacter& InCharacterDetails);

	UFUNCTION(BlueprintPure)
	const FGrpcSroCharacterCharacter& GetCharacterDetails() const { return CharacterDetails; }

	UFUNCTION(BlueprintCallable)
	UMMOCombatComponent* GetCombatComponent() const { return CombatComponent; }

	FOnCharacterDetailsChanged& GetOnCharacterDetailsChanged() { return OnCharacterDetailsChanged; }

protected:
	virtual void OnAbilitySystemInitialized() override;

protected:
	UPROPERTY(EditDefaultsOnly, Category="MMO|PlayerState")
	TObjectPtr<UTargetComponent> TargetComponent;
	
private:
	UPROPERTY(VisibleAnywhere, Category="MMO|PlayerState")
	TObjectPtr<UMMOCombatComponent> CombatComponent;
	
	UPROPERTY()
	TObjectPtr<const class UMMOStatSet> StatSet;

	UPROPERTY()
	FGrpcSroCharacterCharacter CharacterDetails;
	
	UPROPERTY()
	FOnCharacterDetailsChanged OnCharacterDetailsChanged;
};
