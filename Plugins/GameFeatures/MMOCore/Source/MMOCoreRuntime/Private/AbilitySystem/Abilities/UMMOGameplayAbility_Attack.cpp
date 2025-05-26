// Copyright Shattered Realms Online 2024


#include "AbilitySystem/Abilities/UMMOGameplayAbility_Attack.h"

#include "MMOCoreRuntimeModule.h"
#include "AbilitySystem/LyraAbilitySystemComponent.h"
#include "Character/MMOCombatComponent.h"

UUMMOGameplayAbility_Attack::UUMMOGameplayAbility_Attack(const FObjectInitializer& ObjectInitializer)
{
}

void UUMMOGameplayAbility_Attack::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
	const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	check(ActorInfo);

	ULyraAbilitySystemComponent* ASC = CastChecked<ULyraAbilitySystemComponent>(ActorInfo->AbilitySystemComponent.Get());
	if (UMMOCombatComponent* CombatComponent = UMMOCombatComponent::FindCombatComponent(GetAvatarActorFromActorInfo()))
	{
		CombatComponent->StartCombat(TriggerEventData->Instigator);
	}
	
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
}

void UUMMOGameplayAbility_Attack::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
	const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	AActor* Owner = GetAvatarActorFromActorInfo();
	if (UMMOCombatComponent* CombatComponent = UMMOCombatComponent::FindCombatComponent(Owner))
	{
		CombatComponent->StopCombat(Owner);
	}
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}

bool UUMMOGameplayAbility_Attack::CanActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
	const FGameplayTagContainer* SourceTags, const FGameplayTagContainer* TargetTags, FGameplayTagContainer* OptionalRelevantTags) const
{
	UMMOCombatComponent* CombatComponent = UMMOCombatComponent::FindCombatComponent(GetAvatarActorFromActorInfo());
	if (!CombatComponent || !CombatComponent->GetTarget())
	{
		UE_LOG(LogMMO, Error, TEXT("UUMMOGameplayAbility_Attack::CanActivateAbility: Combat component not found or no target set."));
		return false;
	}
	return Super::CanActivateAbility(Handle, ActorInfo, SourceTags, TargetTags, OptionalRelevantTags);
}

void UUMMOGameplayAbility_Attack::StartAttacking()
{
}

void UUMMOGameplayAbility_Attack::FinishAttacking()
{
}
