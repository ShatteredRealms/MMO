// Copyright Shattered Realms Online 2024


#include "Player/MMOPlayerState.h"

#include "CommonUserSubsystem.h"
#include "MMOCoreRuntimeModule.h"
#include "AbilitySystem/Attributes/MMOStatSet.h"
#include "Character/MMOCharacter.h"
#include "Net/UnrealNetwork.h"


AMMOPlayerState::AMMOPlayerState(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	StatSet = CreateDefaultSubobject<UMMOStatSet>(TEXT("StatSet"));
}

void AMMOPlayerState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(AMMOPlayerState, CharacterDetails);
}

void AMMOPlayerState::OnRep_CharacterDetails()
{
	AMMOCharacter* Character = Cast<AMMOCharacter>(GetPlayerController()->GetCharacter());
	if (Character)
	{
		Character->SetCharacterDetails(CharacterDetails);
	}
	else
	{
		UE_LOG(LogMMO, Error, TEXT("AMMOPlayerState::OnRep_CharacterDetails(): PlayerController [%s] has no character to set details on!"), *GetNameSafe(GetPlayerController()));
	}
}
void AMMOPlayerState::SetSROCharacter_Implementation(FGrpcSroCharacterCharacter InCharacterDetails)
{
	CharacterDetails = InCharacterDetails;
	SetPlayerName(CharacterDetails.Name);
	CastChecked<AMMOCharacter>(GetPlayerController()->GetCharacter())->SetCharacterDetails(CharacterDetails);
}


bool AMMOPlayerState::SetSROCharacter_Validate(FGrpcSroCharacterCharacter InCharacterDetails)
{
	UCommonUserSubsystem* CUS = GetGameInstance()->GetSubsystem<UCommonUserSubsystem>();
	if (!CUS)
	{
		return false;
	}

	if (GetUniqueId() == nullptr)
	{
		return false;
	}

	const auto UniqueNetIdPtr = GetUniqueId().GetUniqueNetId();
	if (!UniqueNetIdPtr || !UniqueNetIdPtr.IsValid())
	{
		return false;
	}

	return UniqueNetIdPtr->ToString() == InCharacterDetails.OwnerId;
}
