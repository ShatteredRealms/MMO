// Copyright Shattered Realms Online 2024


#include "Character/MMOCharacter.h"

#include "AbilitySystem/Attributes/MMOStatSet.h"
#include "Character/MMOCombatComponent.h"
#include "Character/TargetComponent.h"


AMMOCharacter::AMMOCharacter(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	StatSet = CreateDefaultSubobject<UMMOStatSet>(TEXT("StatSet"));
	CombatComponent = CreateDefaultSubobject<UMMOCombatComponent>(TEXT("CombatComponent"));
	
	TargetComponent = CreateDefaultSubobject<UTargetComponent>(TEXT("TargetComponent"));
	TargetComponent->SetupAttachment(RootComponent);
}

void AMMOCharacter::SetCharacterDetails(const FGrpcSroCharacterCharacter& InCharacterDetails)
{
	const FGrpcSroCharacterCharacter& OldCharacterDetails = CharacterDetails;
	CharacterDetails = InCharacterDetails;
	OnCharacterDetailsChanged.Broadcast(CharacterDetails, OldCharacterDetails);
}

void AMMOCharacter::OnAbilitySystemInitialized()
{
	Super::OnAbilitySystemInitialized();
	CombatComponent->InitializeWithAbilitySystem(GetLyraAbilitySystemComponent());
}
