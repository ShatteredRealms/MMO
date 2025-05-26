// Copyright Wil Simpson All Rights Reserved

#include "Character/TargetComponent.h"

#include "UI/Targeting/TargetDetailsWidget.h"

UTargetComponent::UTargetComponent(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	SetUsingAbsoluteRotation(true);
	SetUsingAbsoluteLocation(false);
	SetUsingAbsoluteScale(true);
	
	SetCanEverAffectNavigation(false);
	SetWindowFocusable(false);
}

void UTargetComponent::BeginPlay()
{
	Super::BeginPlay();

	// Not needed on the server
	if (GetOwner()->HasAuthority() && GEngine->GetNetMode(GetWorld()) != NM_Standalone)
	{
		return;
	}
	
	checkf(GetWidgetClass(), TEXT("WidgetClass must be set on TargetComponent!"))
	checkf(GetWidgetClass()->IsChildOf(UTargetDetailsWidget::StaticClass()), TEXT("WidgetClass must be a child of UTargetDetailsWidget!"));;
	GetDetailsUserWidget()->Setup(GetOwner());
}

bool UTargetComponent::SetTargeted(bool bIsTargeted)
{
	return GetDetailsUserWidget()->SetTargeted(bIsTargeted);
}

bool UTargetComponent::IsTargeted()
{
	return GetDetailsUserWidget()->IsTargeted();
}

bool UTargetComponent::SetAttacked(bool bIsAttacked)
{
	return GetDetailsUserWidget()->SetAttacked(bIsAttacked);
}

bool UTargetComponent::IsAttacked()
{
	return GetDetailsUserWidget()->IsAttacked();
}

bool UTargetComponent::IsAttackable()
{
	return GetDetailsUserWidget()->IsAttackable();
}

void UTargetComponent::SetDisplayName(FString NewName)
{
	GetDetailsUserWidget()->SetDisplayName(NewName);
}

UTargetDetailsWidget* UTargetComponent::GetDetailsUserWidget()
{
	return Cast<UTargetDetailsWidget>(GetUserWidgetObject());
}