// Copyright Shattered Realms Online 2024


#include "Equipment/MMOQuickBarComponent.h"

#include "Equipment/LyraEquipmentManagerComponent.h"
#include "GameFramework/GameplayMessageSubsystem.h"
#include "GameFramework/Pawn.h"
#include "NativeGameplayTags.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(MMOQuickBarComponent)

class ULyraEquipmentDefinition;

// UE_DEFINE_GAMEPLAY_TAG_STATIC(TAG_MMO_QuickBar_Message_SlotsChanged, "MMO.QuickBar.Message.SlotsChanged");

void UMMOQuickBarComponent::AddItemToSlot(int32 SlotIndex, ULyraInventoryItemInstance* Item)
{
	if (Slots.IsValidIndex(SlotIndex) && (Item != nullptr))
	{
		Slots[SlotIndex] = Item;
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Invalid slot index or item is null"));
	}
}

ULyraInventoryItemInstance* UMMOQuickBarComponent::RemoveItemFromSlot(int32 SlotIndex)
{
	ULyraInventoryItemInstance* Item = nullptr;
	if (Slots.IsValidIndex(SlotIndex))
	{
		Item = Slots[SlotIndex];
		if (Item != nullptr)
		{
			Slots[SlotIndex] = nullptr;
		}
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Invalid slot index"));
	}

	return Item;
}

void UMMOQuickBarComponent::BeginPlay()
{
	if (Slots.Num() < NumSlots)
	{
		Slots.AddDefaulted(NumSlots - Slots.Num());
	}
	Super::BeginPlay();
}

void UMMOQuickBarComponent::ActivateSlot(int32 SlotIndex)
{
	// check(Slots.IsValidIndex(SlotIndex));
	//
	// ULyraInventoryItemInstance* Item = Slots[SlotIndex];
	// if (Item == nullptr)
	// {
	// 	UE_LOG(LogTemp, Warning, TEXT("No item in slot %d"), SlotIndex);
	// 	return;
	// }
	// const UInventoryFragment_EquippableItem* EquipInfo = Item->FindFragmentByClass<UInventoryFragment_EquippableItem>();
	// if (EquipInfo == nullptr)
	// {
	// 	return;
	// }
	// ULyraEquipmentManagerComponent* EquipmentManager = FindEquipmentManager();
	// if (EquipmentManager == nullptr)
	// {
	// 	UE_LOG(LogTemp, Warning, TEXT("Equipment manager not found"));
	// 	return;
	// }
	//
	// if (EquipInfo->EquipmentDefinition == nullptr)
	// {
	// 	UE_LOG(LogTemp, Warning, TEXT("No equipment definition found"));
	// 	return;
	// }
	// ULyraEquipmentInstance* EquippedItem = EquipmentManager->EquipItem(EquipInfo->EquipmentDefinition);
}

ULyraEquipmentManagerComponent* UMMOQuickBarComponent::FindEquipmentManager() const
{
	if (AController* OwnerController = Cast<AController>(GetOwner()))
	{
		if (APawn* Pawn = OwnerController->GetPawn())
		{
			// return Pawn->FindComponentByClass<ULyraEquipmentManagerComponent>();
		}
	}
	return nullptr;
}
