// Copyright Wil Simpson All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/IUserObjectListEntry.h"
#include "Blueprint/UserWidget.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "DropDownListEntry.generated.h"

class UDropDownData;
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FDropDownSelected, UDropDownData*, SelectedData);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FDropDownClicked, UDropDownData*, ClickedData);

/**
 * 
 */
UCLASS(Blueprintable)
class MODULARWINDOWS_API UDropDownData : public UObject
{
	GENERATED_BODY()	
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<UObject> DataObject;
	
	// What to display in the dropdown
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString Name;

	// Whether this should open another drop down
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bDropDown;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FDropDownSelected OnSelected;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FDropDownClicked OnClicked;

	UPROPERTY(BlueprintReadOnly)
	bool bHovered;
};
/**
 * 
 */
UCLASS(Blueprintable)
class MODULARWINDOWS_API UDropDownListEntry : public UUserWidget, public IUserObjectListEntry 
{
	GENERATED_BODY()

private:
	FSlateBrush OriginalStyle;
	FSlateBrush OriginalHoveredStyle;
	
public:
	virtual void NativeConstruct() override;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<UDropDownData> Data;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(BindWidget))
	TObjectPtr<UTextBlock> EntryText;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(BindWidget))
	TObjectPtr<UButton> Button;
	
	virtual void NativeOnListItemObjectSet(UObject* ListItemObject) override;

	virtual void NativeOnItemSelectionChanged(bool bIsSelected) override;
	
	UFUNCTION()
	void OnClicked();
	
	UFUNCTION()
	void OnSelected();

	UFUNCTION()
	void OnUnhovered();
};
