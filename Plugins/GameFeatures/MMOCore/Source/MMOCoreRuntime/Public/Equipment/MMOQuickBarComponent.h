// Copyright Shattered Realms Online 2024

#pragma once

#include "Components/ControllerComponent.h"
#include "Equipment/LyraEquipmentManagerComponent.h"
#include "Inventory/LyraInventoryItemInstance.h"

#include "MMOQuickBarComponent.generated.h"

/**
 * 
 */
UCLASS(Blueprintable, meta=(BlueprintSpawnableComponent))
class MMOCORERUNTIME_API UMMOQuickBarComponent : public UControllerComponent
{
	GENERATED_BODY()
	
public:
	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly, Category="MMO|QuickBar")
	void AddItemToSlot(int32 SlotIndex, ULyraInventoryItemInstance* Item);

	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly, Category="MMO|QuickBar")
	ULyraInventoryItemInstance* RemoveItemFromSlot(int32 SlotIndex);

	virtual void BeginPlay() override;

private:
	void ActivateSlot(int32 SlotIndex);

	ULyraEquipmentManagerComponent* FindEquipmentManager() const;

protected:
	UPROPERTY()
	int32 NumSlots = 10;

private:
	UPROPERTY()
	TArray<TObjectPtr<ULyraInventoryItemInstance>> Slots;
};
