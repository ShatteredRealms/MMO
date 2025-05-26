// Copyright Wil Simpson All Rights Reserved


#include "BaseWindow.h"

#include "Components/NamedSlot.h"

UBaseWindow::UBaseWindow()
{
}

void UBaseWindow::NativeConstruct()
{
	Super::NativeConstruct();

	OnTabSelected.BindUObject(this, &UBaseWindow::FocusTab);
}

UTabWidget* UBaseWindow::CreateTab(const FText& TabName, UUserWidget* Content)
{
	UTabWidget* Tab = CreateWidget<UTabWidget>(GetOwningPlayer(), TabWidgetClass);

	if (!Tab)
	{
		return nullptr;
	}
	
	Tab->TabName->SetText(TabName);
	Tab->OnTabSelected = &OnTabSelected;
	Tabs->AddChildToHorizontalBox(Tab);
	TabMapping.Add(Tab, Content);

	ContentSlot->AddChild(Content);
	Content->SetVisibility(ESlateVisibility::Hidden);

	if (!ActiveTab)
	{
		FocusTab(Tab);
	}

	return Tab;
}

void UBaseWindow::AppendTab(UTabWidget* Tab, UUserWidget* Content)
{
	Tabs->AddChildToHorizontalBox(Tab);
	TabMapping.Add(Tab, Content);
}

void UBaseWindow::FocusTab(UTabWidget* Tab)
{
	for (const TPair<UTabWidget*, UUserWidget*>& tab : TabMapping)
	{
		if (tab.Key == Tab)
		{
			Tab->SetSelected(true);
			ActiveTab = tab.Key;
			tab.Value->SetVisibility(ESlateVisibility::Visible);
		}
		else
		{
			tab.Key->SetSelected(false);
			tab.Value->SetVisibility(ESlateVisibility::Hidden);
		}
	}
}

void UBaseWindow::SetCloseable(bool IsCloseable)
{
	CloseButton->SetVisibility(IsCloseable ? ESlateVisibility::Visible : ESlateVisibility::Hidden);
}

FVector2D UBaseWindow::GetMinSize()
{
	// +10 for offset (5 each side)
	// +8 for padding (4 each side)
	auto X = FMath::Max(Tabs->GetDesiredSize().X + 10 + 8, 10.0);
	return {X, 100.0};
}
