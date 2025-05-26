// Copyright Wil Simpson All Rights Reserved


#include "Components/DropDownListEntry.h"

void UDropDownListEntry::NativeConstruct()
{
	Super::NativeConstruct();
	OriginalHoveredStyle = Button->GetStyle().Hovered;
	OriginalStyle = Button->GetStyle().Normal;
}

void UDropDownListEntry::NativeOnListItemObjectSet(UObject* ListItemObject)
{
	Data = Cast<UDropDownData>(ListItemObject);
	if (!Data)
	{
		return;
	}
	
	Button->OnClicked.AddDynamic(this, &UDropDownListEntry::OnClicked);
	Button->OnHovered.AddDynamic(this, &UDropDownListEntry::OnSelected);
	Button->OnUnhovered.AddDynamic(this, &UDropDownListEntry::OnUnhovered);
	FButtonStyle style = Button->GetStyle();
	style.Normal = OriginalStyle;
	Button->SetStyle(style);
}

void UDropDownListEntry::NativeOnItemSelectionChanged(bool bIsSelected)
{
	IUserObjectListEntry::NativeOnItemSelectionChanged(bIsSelected);

	if (bIsSelected)
	{

		FButtonStyle style = Button->GetStyle();
		style.Normal = OriginalHoveredStyle;
		Button->SetStyle(style);
	}
	else
	{
		FButtonStyle style = Button->GetStyle();
		style.Normal = OriginalStyle;
		Button->SetStyle(style);
	}
}

void UDropDownListEntry::OnClicked()
{
	NativeOnItemSelectionChanged(true);
	Data->OnClicked.Broadcast(Data);
}

void UDropDownListEntry::OnSelected()
{
	NativeOnItemSelectionChanged(true);
	Data->OnSelected.Broadcast(Data);
	Data->bHovered = true;
}

void UDropDownListEntry::OnUnhovered()
{
	Data->bHovered = false;
}
