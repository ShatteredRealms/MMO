
#include "UI/Targeting/TargetDetailsWidget.h"

#include "Character/LyraHealthComponent.h"
#include "Character/MMOCharacter.h"
#include "Components/Border.h"
#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"

void UTargetDetailsWidget::Setup(AActor* InOwner)
{
	Owner = InOwner;
	HealthComponent = InOwner->FindComponentByClass<ULyraHealthComponent>();

	if (HealthComponent)
	{
		HealthComponent->OnHealthChanged.AddDynamic(this, &UTargetDetailsWidget::UpdateHealthPercentage);
		HealthProgressBar->SetVisibility(ESlateVisibility::Visible);
	}
	else
	{
		HealthProgressBar->SetVisibility(ESlateVisibility::Hidden);
	}
	
	AMMOCharacter* Character = Cast<AMMOCharacter>(Owner);
	if (Character)
	{
		SetDisplayName(Character->GetCharacterDetails().Name);
		Character->GetOnCharacterDetailsChanged().AddDynamic(this, &UTargetDetailsWidget::OnCharaterDetailsChanged);
	}

	UpdateBorder();
	OnSetupCompleted(Owner);
}

bool UTargetDetailsWidget::SetAttacked(bool bIsAttacked)
{
	check(IsAttackable())
	
	if (bAttacked == bIsAttacked)
	{
		return false;
	}

	bAttacked = bIsAttacked;
	UpdateBorder();
	OnAttackedChangedEvent(bAttacked);

	return true;
}

void UTargetDetailsWidget::SetDisplayName(FString NewDisplayName)
{
	DisplayName = NewDisplayName;
	NameTextBox->SetText(FText::FromString(NewDisplayName));
	OnDisplayNameChangedEvent(DisplayName);
}

bool UTargetDetailsWidget::SetTargeted(bool bNewTargeted)
{
	if (bTargeted == bNewTargeted)
	{
		return false;
	}

	bTargeted = bNewTargeted;
	UpdateBorder();
	OnTargetedChangedEvent(bTargeted);
	
	return true;
}

void UTargetDetailsWidget::UpdateHealthPercentage_Implementation(ULyraHealthComponent* InHealthComponent, float OldValue, float NewValue,
	AActor* Instigator)
{
	HealthProgressBar->SetPercent(NewValue);
}

void UTargetDetailsWidget::UpdateBorder()
{
	FLinearColor Color = TargetedHighlightColor;
	if (bAttacked)
	{
		Color = AttackedHighlightColor;
	}

	Color.A = bTargeted || bAttacked ? 255 : 0;
	
	Border->SetBrushColor(Color);
}

void UTargetDetailsWidget::OnCharaterDetailsChanged(const FGrpcSroCharacterCharacter& NewCharacterDetails,
	const FGrpcSroCharacterCharacter& OldCharacterDetails)
{
	SetDisplayName(NewCharacterDetails.Name);
}
