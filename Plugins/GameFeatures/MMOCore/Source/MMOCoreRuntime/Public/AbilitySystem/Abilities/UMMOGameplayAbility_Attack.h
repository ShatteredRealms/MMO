// Copyright Shattered Realms Online 2024

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Abilities/LyraGameplayAbility.h"
#include "UMMOGameplayAbility_Attack.generated.h"

/**
 * 
 */
UCLASS()
class MMOCORERUNTIME_API UUMMOGameplayAbility_Attack : public ULyraGameplayAbility
{
	GENERATED_BODY()

public:
	UUMMOGameplayAbility_Attack(const FObjectInitializer& ObjectInitializer);

protected:
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled) override;
	virtual bool CanActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayTagContainer* SourceTags, const FGameplayTagContainer* TargetTags, FGameplayTagContainer* OptionalRelevantTags) const override;

	UFUNCTION(BlueprintCallable, Category = "MMO | Ability")
	void StartAttacking();

	UFUNCTION(BlueprintCallable, Category = "MMO | Ability")
	void FinishAttacking();
};
