// Copyright Wil Simpson All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Components/WidgetComponent.h"
#include "TargetComponent.generated.h"


class ULyraHealthComponent;
class UTargetDetailsWidget;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class MMOCORERUNTIME_API UTargetComponent : public UWidgetComponent 
{
	GENERATED_BODY()
	
public:
	// Sets default values for this component's properties
	UTargetComponent(const FObjectInitializer& ObjectInitializer);

	virtual void BeginPlay() override;
	
	/** Set whether targeted or not. Nothing happens if does not change. */
	UFUNCTION(BlueprintCallable, Category="Targeting")
	bool SetTargeted(bool bIsTargeted);

	/** Whether is targeted. */
	UFUNCTION(BlueprintCallable, Category="Targeting")
	bool IsTargeted();

	UFUNCTION(BlueprintCallable, Category="Targeting")
	bool SetAttacked(bool bIsAttacked);
	
	UFUNCTION(BlueprintCallable, Category="Targeting")
	bool IsAttacked();
	
	UFUNCTION(BlueprintCallable, Category="Targeting")
	bool IsAttackable();

	/** Setter for DisplayName. */
	UFUNCTION(BlueprintCallable, Category="Targeting")
	void SetDisplayName(FString NewName);

	UFUNCTION(BlueprintCallable, Category=Targeting)
	virtual UTargetDetailsWidget* GetDetailsUserWidget();
};
