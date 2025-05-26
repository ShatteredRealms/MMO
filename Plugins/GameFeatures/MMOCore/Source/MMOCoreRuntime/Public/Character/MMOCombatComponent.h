// Copyright Shattered Realms Online 2024

#pragma once

#include "CoreMinimal.h"
#include "Components/GameFrameworkComponent.h"
#include "MMOCombatComponent.generated.h"

class UTargetComponent;
class ULyraAbilitySystemComponent;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_FourParams(FMMOCombat_TargetChanged, UMMOCombatComponent*, CombatComponent, AActor*, OldTarget, AActor*, NewTarget,
                                              const AActor*, Instigator);
/**
 * 
 */
UCLASS()
class MMOCORERUNTIME_API UMMOCombatComponent : public UGameFrameworkComponent
{
	GENERATED_BODY()
	
public:
	UMMOCombatComponent(const FObjectInitializer& ObjectInitializer);

	UFUNCTION(BlueprintPure, Category = "MMO | Combat")
	static UMMOCombatComponent* FindCombatComponent(const AActor* Actor) { return (Actor ? Actor->FindComponentByClass<UMMOCombatComponent>() : nullptr); }

	UFUNCTION(BlueprintPure, Category = "MMO | Combat")
	static bool IsTargetable(const AActor* Actor)
	{
		return Actor && Actor->GetComponentByClass<UTargetComponent>();
	}

	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;

	UFUNCTION(BlueprintCallable, Category = "MMO | Combat")
	void InitializeWithAbilitySystem(ULyraAbilitySystemComponent* InASC);

	UFUNCTION(BlueprintCallable, Category = "MMO | Combat")
	bool IsInCombat() const { return CombatTarget != nullptr; }

	UFUNCTION(BlueprintCallable, Category = "MMO | Combat")
	AActor* GetAttackTarget() const { return CombatTarget; }

	UFUNCTION(BlueprintCallable, Category = "MMO | Combat")
	AActor* GetTarget() const { return Target; }

	UFUNCTION(BlueprintCallable, Category = "MMO | Combat")
	void SetTarget(AActor* InTarget, const AActor* Instigator);

	UFUNCTION(BlueprintCallable, Category = "MMO | Combat")
	bool StartCombat(const AActor* Instigator);
	
	UFUNCTION(BlueprintCallable, Category = "MMO | Combat")
	void StopCombat(const AActor* Instigator);
	
private:
	void SetActorTargeted(const AActor* Actor, bool bTargeted);
	void SetActorAttacked(const AActor* Actor, bool bAttacked);

public:
	UPROPERTY(BlueprintAssignable, Category = "MMO | Combat")
	FMMOCombat_TargetChanged OnCombatTargetChanged;
	
	UPROPERTY(BlueprintAssignable, Category = "MMO | Combat")
	FMMOCombat_TargetChanged OnTargetChanged;

protected:
	void ClearGameplayTags();
	void SetCombatTarget(AActor* InTarget, const AActor* Instigator);

	UFUNCTION()
	void OnRep_CombatTarget(AActor* OldTarget);

	UFUNCTION()
	void OnRep_Target(AActor* OldTarget);
	
protected:
	// Ability system used by this component.
	UPROPERTY()
	TObjectPtr<ULyraAbilitySystemComponent> AbilitySystemComponent;
	
	// Target that the character is currently attacking.
	UPROPERTY(ReplicatedUsing=OnRep_CombatTarget)
	TObjectPtr<AActor> CombatTarget;

	// Target that the character is currently targeting.
	UPROPERTY(ReplicatedUsing=OnRep_Target)
	TObjectPtr<AActor> Target;
};

