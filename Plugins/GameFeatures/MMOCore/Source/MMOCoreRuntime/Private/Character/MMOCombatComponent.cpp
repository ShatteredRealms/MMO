// Copyright Shattered Realms Online 2024

#include "Character/MMOCombatComponent.h"

#include "MMOCoreRuntimeModule.h"
#include "MMOGameplayTags.h"
#include "AbilitySystem/LyraAbilitySystemComponent.h"
#include "Character/TargetComponent.h"
#include "Net/UnrealNetwork.h"


UMMOCombatComponent::UMMOCombatComponent(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	PrimaryComponentTick.bStartWithTickEnabled = false;
	PrimaryComponentTick.bCanEverTick = false;
	
	SetIsReplicatedByDefault(true);

	AbilitySystemComponent = nullptr;
	CombatTarget = nullptr;
	Target = nullptr;
}

void UMMOCombatComponent::InitializeWithAbilitySystem(ULyraAbilitySystemComponent* InASC)
{
	AActor* Owner = GetOwner();
	check(Owner);

	if (!ensureMsgf(!AbilitySystemComponent, TEXT("MMOCombatComponent: Combat component for owner [%s] has already been initialized with an ability system."), *GetNameSafe(Owner)))
	{
		UE_LOG(LogMMO, Error, TEXT("MMOCombatComponent: Combat component for owner [%s] has already been initialized with an ability system."), *GetNameSafe(Owner));
		return;
	}
	
	AbilitySystemComponent = InASC;
	if (!ensureMsgf(AbilitySystemComponent, TEXT("MMOCombatComponent: Cannot initialize combat component for owner [%s] with NULL ability system."), *GetNameSafe(Owner)))
	{
		return;
	}

	ClearGameplayTags();
	OnCombatTargetChanged.Broadcast(this, nullptr, CombatTarget, nullptr);
	OnTargetChanged.Broadcast(this, nullptr, Target, nullptr);
}

bool UMMOCombatComponent::StartCombat(const AActor* Instigator)
{
	if (Target == nullptr)
	{
		return false;
	}
	
	SetCombatTarget(Target, Instigator);
	return true;
}

void UMMOCombatComponent::StopCombat(const AActor* Instigator)
{
	SetCombatTarget(nullptr, Instigator);
}

#define CHECK_GET_TARGETCOMPONENT(ActorName, TargetComponentName) \
	if (ActorName == nullptr) return; \
	UTargetComponent* TargetComponentName = ActorName->FindComponentByClass<UTargetComponent>(); \
	if (TargetComponent == nullptr) return; \

void UMMOCombatComponent::SetActorTargeted(const AActor* Actor, bool bTargeted)
{
	CHECK_GET_TARGETCOMPONENT(Actor, TargetComponent);
	TargetComponent->SetTargeted(bTargeted);
}

void UMMOCombatComponent::SetActorAttacked(const AActor* Actor, bool bAttacked)
{
	CHECK_GET_TARGETCOMPONENT(Actor, TargetComponent);
	TargetComponent->SetAttacked(bAttacked);
}

void UMMOCombatComponent::SetCombatTarget(AActor* InTarget, const AActor* Instigator)
{
	if (CombatTarget != InTarget)
	{
		AActor* OldTarget = CombatTarget;
		CombatTarget = InTarget;
		SetActorAttacked(OldTarget, false);
		SetActorAttacked(CombatTarget, true);

		OnCombatTargetChanged.Broadcast(this, OldTarget, CombatTarget, Instigator);
		if (AbilitySystemComponent)
		{
			AbilitySystemComponent->SetLooseGameplayTagCount(MMOGameplayTags::Status_Combat_Attacking, InTarget == nullptr ? 0 : 1);
		}
	}
}

void UMMOCombatComponent::OnRep_CombatTarget(AActor* OldTarget)
{
	SetCombatTarget(CombatTarget, GetOwner());
}

void UMMOCombatComponent::OnRep_Target(AActor* OldTarget)
{
	SetTarget(Target, GetOwner());
}

void UMMOCombatComponent::SetTarget(AActor* InTarget, const AActor* Instigator)
{
	if (Target != InTarget)
	{
		AActor* OldTarget = Target;
		Target = InTarget;
		SetActorTargeted(OldTarget, false);
		SetActorTargeted(Target, true);

		OnTargetChanged.Broadcast(this, OldTarget, Target, Instigator);
	}
}

void UMMOCombatComponent::ClearGameplayTags()
{
	if (AbilitySystemComponent)
	{
		AbilitySystemComponent->SetLooseGameplayTagCount(MMOGameplayTags::Status_Combat_Attacking, 0);
	}
}

void UMMOCombatComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(UMMOCombatComponent, CombatTarget);
	DOREPLIFETIME_CONDITION(UMMOCombatComponent, Target, COND_OwnerOnly);
}
