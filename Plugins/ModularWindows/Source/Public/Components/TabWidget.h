// Copyright Wil Simpson All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "TabWidget.generated.h"

class MODULARWINDOWS_API UTabWidget;
DECLARE_DELEGATE_OneParam(FTabSelectedDelegate, UTabWidget*)

/**
 * 
 */
UCLASS(Blueprintable)
class MODULARWINDOWS_API UTabWidget : public UUserWidget
{
	GENERATED_BODY()

	virtual void NativeConstruct() override;

	UPROPERTY()
	bool bSelected;
	
public:
	FTabSelectedDelegate* OnTabSelected;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Chat", meta = (BindWidget))
	TObjectPtr<UButton> TabButton;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Chat", meta = (BindWidget))
	TObjectPtr<UTextBlock> TabName;

	/** The style used when the tab is not selected */
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FButtonStyle InactiveStyle;

	/** The style used when the tab is selected */
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FButtonStyle ActiveStyle;

	/** Sets the style and whether the button is selected */
	UFUNCTION(BlueprintCallable)
	void SetSelected(bool bNewSelection);

	/**
	 * Whether the button is selected or not
	 * @returns true if the button is selected
	 */
	UFUNCTION(BlueprintCallable)
	bool IsSelected() { return bSelected; }

	UFUNCTION()
	void OnTabClicked();
};
