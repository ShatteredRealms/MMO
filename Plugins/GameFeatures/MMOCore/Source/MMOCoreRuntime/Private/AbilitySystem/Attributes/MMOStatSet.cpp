// Copyright Epic Games, Inc. All Rights Reserved.

#include "AbilitySystem/Attributes/MMOStatSet.h"

#include "Net/UnrealNetwork.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(MMOStatSet)

class FLifetimeProperty;

UMMOStatSet::UMMOStatSet()
	: Strength(1.0f)
	, Stamina(1.0f)
	, Agility(1.0f)
	, Perception(1.0f)
	, Intellect(1.0f)
	, Willpower(1.0f)
	, Endurance(1.0f)
	, EnergyReserve(1.0f)
	, CloseCombatEvasion(1.0f)
	, RangedCombatEvasion(1.0f)
	, Deflection(1.0f)
	, MartialArts(1.0f)
	, OneHanded(1.0f)
	, TwoHanded(1.0f)
	, Piercing(1.0f)
	, WeaponSpeed(1.0f)
	, MeleeSpeed(1.0f)
	, SneakAttack(1.0f)
	, QuickStrike(1.0f)
	, PowerStrike(1.0f)
	, RapidBlows(1.0f)
	, Bow(1.0f)
	, Pistol(1.0f)
	, Rifle(1.0f)
	, Sniper(1.0f)
	, Shotgun(1.0f)
	, Throwing(1.0f)
	, Ranged(1.0f)
	, SteadyShot(1.0f)
	, QuickShot(1.0f)
	, PowerShot(1.0f)
	, RapidFire(1.0f)
	, SilentShot(1.0f)
	, Creation(1.0f)
	, Metamorphosis(1.0f)
	, Remedy(1.0f)
	, Rejuvenation(1.0f)
	, SpaceTime(1.0f)
	, Destruction(1.0f)
	, EnergySpeed(1.0f)
	, Alchemy(1.0f)
	, Programming(1.0f)
	, Construction(1.0f)
	, Engineering(1.0f)
	, FirstAid(1.0f)
	, Surgery(1.0f)
	, Speed(1.0f)
	, Flying(1.0f)
	, Stealth(1.0f)
{
}

void UMMOStatSet::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME_CONDITION_NOTIFY(UMMOStatSet, Strength, COND_OwnerOnly, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UMMOStatSet, Stamina, COND_OwnerOnly, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UMMOStatSet, Agility, COND_OwnerOnly, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UMMOStatSet, Perception, COND_OwnerOnly, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UMMOStatSet, Intellect, COND_OwnerOnly, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UMMOStatSet, Willpower, COND_OwnerOnly, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UMMOStatSet, Endurance, COND_OwnerOnly, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UMMOStatSet, EnergyReserve, COND_OwnerOnly, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UMMOStatSet, CloseCombatEvasion, COND_OwnerOnly, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UMMOStatSet, RangedCombatEvasion, COND_OwnerOnly, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UMMOStatSet, Deflection, COND_OwnerOnly, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UMMOStatSet, MartialArts, COND_OwnerOnly, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UMMOStatSet, OneHanded, COND_OwnerOnly, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UMMOStatSet, TwoHanded, COND_OwnerOnly, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UMMOStatSet, Piercing, COND_OwnerOnly, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UMMOStatSet, WeaponSpeed, COND_OwnerOnly, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UMMOStatSet, MeleeSpeed, COND_OwnerOnly, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UMMOStatSet, SneakAttack, COND_OwnerOnly, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UMMOStatSet, QuickStrike, COND_OwnerOnly, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UMMOStatSet, PowerStrike, COND_OwnerOnly, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UMMOStatSet, RapidBlows, COND_OwnerOnly, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UMMOStatSet, Bow, COND_OwnerOnly, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UMMOStatSet, Pistol, COND_OwnerOnly, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UMMOStatSet, Rifle, COND_OwnerOnly, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UMMOStatSet, Shotgun, COND_OwnerOnly, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UMMOStatSet, Sniper, COND_OwnerOnly, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UMMOStatSet, Throwing, COND_OwnerOnly, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UMMOStatSet, Ranged, COND_OwnerOnly, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UMMOStatSet, SteadyShot, COND_OwnerOnly, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UMMOStatSet, QuickShot, COND_OwnerOnly, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UMMOStatSet, PowerShot, COND_OwnerOnly, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UMMOStatSet, RapidFire, COND_OwnerOnly, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UMMOStatSet, SilentShot, COND_OwnerOnly, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UMMOStatSet, Creation, COND_OwnerOnly, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UMMOStatSet, Metamorphosis, COND_OwnerOnly, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UMMOStatSet, Remedy, COND_OwnerOnly, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UMMOStatSet, Rejuvenation, COND_OwnerOnly, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UMMOStatSet, SpaceTime, COND_OwnerOnly, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UMMOStatSet, Destruction, COND_OwnerOnly, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UMMOStatSet, EnergySpeed, COND_OwnerOnly, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UMMOStatSet, Alchemy, COND_OwnerOnly, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UMMOStatSet, Programming, COND_OwnerOnly, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UMMOStatSet, Construction, COND_OwnerOnly, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UMMOStatSet, Engineering, COND_OwnerOnly, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UMMOStatSet, FirstAid, COND_OwnerOnly, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UMMOStatSet, Surgery, COND_OwnerOnly, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UMMOStatSet, Speed, COND_OwnerOnly, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UMMOStatSet, Flying, COND_OwnerOnly, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UMMOStatSet, Stealth, COND_OwnerOnly, REPNOTIFY_Always);
}

void UMMOStatSet::OnRep_Strength(const FGameplayAttributeData& OldValue)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UMMOStatSet, Strength, OldValue);
}

void UMMOStatSet::OnRep_Stamina(const FGameplayAttributeData& OldValue)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UMMOStatSet, Stamina, OldValue);
}

void UMMOStatSet::OnRep_Agility(const FGameplayAttributeData& OldValue)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UMMOStatSet, Agility, OldValue);
}

void UMMOStatSet::OnRep_Perception(const FGameplayAttributeData& OldValue)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UMMOStatSet, Perception, OldValue);
}

void UMMOStatSet::OnRep_Intellect(const FGameplayAttributeData& OldValue)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UMMOStatSet, Intellect, OldValue);
}

void UMMOStatSet::OnRep_Willpower(const FGameplayAttributeData& OldValue)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UMMOStatSet, Willpower, OldValue);
}

void UMMOStatSet::OnRep_Endurance(const FGameplayAttributeData& OldValue)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UMMOStatSet, Endurance, OldValue);
}

void UMMOStatSet::OnRep_EnergyReserve(const FGameplayAttributeData& OldValue)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UMMOStatSet, EnergyReserve, OldValue);
}

void UMMOStatSet::OnRep_CloseCombatEvasion(const FGameplayAttributeData& OldValue)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UMMOStatSet, CloseCombatEvasion, OldValue);
}

void UMMOStatSet::OnRep_RangedCombatEvasion(const FGameplayAttributeData& OldValue)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UMMOStatSet, RangedCombatEvasion, OldValue);
}

void UMMOStatSet::OnRep_Deflection(const FGameplayAttributeData& OldValue)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UMMOStatSet, Deflection, OldValue);
}

void UMMOStatSet::OnRep_MartialArts(const FGameplayAttributeData& OldValue)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UMMOStatSet, MartialArts, OldValue);
}

void UMMOStatSet::OnRep_OneHanded(const FGameplayAttributeData& OldValue)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UMMOStatSet, OneHanded, OldValue);
}

void UMMOStatSet::OnRep_TwoHanded(const FGameplayAttributeData& OldValue)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UMMOStatSet, TwoHanded, OldValue);
}

void UMMOStatSet::OnRep_Piercing(const FGameplayAttributeData& OldValue)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UMMOStatSet, Piercing, OldValue);
}

void UMMOStatSet::OnRep_WeaponSpeed(const FGameplayAttributeData& OldValue)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UMMOStatSet, WeaponSpeed, OldValue);
}

void UMMOStatSet::OnRep_MeleeSpeed(const FGameplayAttributeData& OldValue)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UMMOStatSet, MeleeSpeed, OldValue);
}

void UMMOStatSet::OnRep_SneakAttack(const FGameplayAttributeData& OldValue)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UMMOStatSet, SneakAttack, OldValue);
}

void UMMOStatSet::OnRep_QuickStrike(const FGameplayAttributeData& OldValue)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UMMOStatSet, QuickStrike, OldValue);
}

void UMMOStatSet::OnRep_PowerStrike(const FGameplayAttributeData& OldValue)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UMMOStatSet, PowerStrike, OldValue);
}

void UMMOStatSet::OnRep_RapidBlows(const FGameplayAttributeData& OldValue)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UMMOStatSet, RapidBlows, OldValue);
}

void UMMOStatSet::OnRep_Bow(const FGameplayAttributeData& OldValue)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UMMOStatSet, Bow, OldValue);
}

void UMMOStatSet::OnRep_Pistol(const FGameplayAttributeData& OldValue)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UMMOStatSet, Pistol, OldValue);
}

void UMMOStatSet::OnRep_Rifle(const FGameplayAttributeData& OldValue)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UMMOStatSet, Rifle, OldValue);
}

void UMMOStatSet::OnRep_Sniper(const FGameplayAttributeData& OldValue)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UMMOStatSet, Sniper, OldValue);
}

void UMMOStatSet::OnRep_Shotgun(const FGameplayAttributeData& OldValue)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UMMOStatSet, Shotgun, OldValue);
}

void UMMOStatSet::OnRep_Throwing(const FGameplayAttributeData& OldValue)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UMMOStatSet, Throwing, OldValue);
}

void UMMOStatSet::OnRep_Ranged(const FGameplayAttributeData& OldValue)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UMMOStatSet, Ranged, OldValue);
}

void UMMOStatSet::OnRep_SteadyShot(const FGameplayAttributeData& OldValue)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UMMOStatSet, SteadyShot, OldValue);
}

void UMMOStatSet::OnRep_QuickShot(const FGameplayAttributeData& OldValue)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UMMOStatSet, QuickShot, OldValue);
}

void UMMOStatSet::OnRep_PowerShot(const FGameplayAttributeData& OldValue)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UMMOStatSet, PowerShot, OldValue);
}

void UMMOStatSet::OnRep_RapidFire(const FGameplayAttributeData& OldValue)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UMMOStatSet, RapidFire, OldValue);
}

void UMMOStatSet::OnRep_SilentShot(const FGameplayAttributeData& OldValue)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UMMOStatSet, SilentShot, OldValue);
}

void UMMOStatSet::OnRep_Creation(const FGameplayAttributeData& OldValue)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UMMOStatSet, Creation, OldValue);
}

void UMMOStatSet::OnRep_Metamorphosis(const FGameplayAttributeData& OldValue)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UMMOStatSet, Metamorphosis, OldValue);
}

void UMMOStatSet::OnRep_Remedy(const FGameplayAttributeData& OldValue)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UMMOStatSet, Remedy, OldValue);
}

void UMMOStatSet::OnRep_Rejuvenation(const FGameplayAttributeData& OldValue)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UMMOStatSet, Rejuvenation, OldValue);
}

void UMMOStatSet::OnRep_SpaceTime(const FGameplayAttributeData& OldValue)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UMMOStatSet, SpaceTime, OldValue);
}

void UMMOStatSet::OnRep_Destruction(const FGameplayAttributeData& OldValue)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UMMOStatSet, Destruction, OldValue);
}

void UMMOStatSet::OnRep_EnergySpeed(const FGameplayAttributeData& OldValue)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UMMOStatSet, EnergySpeed, OldValue);
}

void UMMOStatSet::OnRep_Alchemy(const FGameplayAttributeData& OldValue)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UMMOStatSet, Alchemy, OldValue);
}

void UMMOStatSet::OnRep_Programming(const FGameplayAttributeData& OldValue)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UMMOStatSet, Programming, OldValue);
}

void UMMOStatSet::OnRep_Construction(const FGameplayAttributeData& OldValue)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UMMOStatSet, Construction, OldValue);
}

void UMMOStatSet::OnRep_Engineering(const FGameplayAttributeData& OldValue)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UMMOStatSet, Engineering, OldValue);
}

void UMMOStatSet::OnRep_FirstAid(const FGameplayAttributeData& OldValue)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UMMOStatSet, FirstAid, OldValue);
}

void UMMOStatSet::OnRep_Surgery(const FGameplayAttributeData& OldValue)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UMMOStatSet, Surgery, OldValue);
}

void UMMOStatSet::OnRep_Speed(const FGameplayAttributeData& OldValue)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UMMOStatSet, Speed, OldValue);
}

void UMMOStatSet::OnRep_Flying(const FGameplayAttributeData& OldValue)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UMMOStatSet, Flying, OldValue);
}

void UMMOStatSet::OnRep_Stealth(const FGameplayAttributeData& OldValue)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UMMOStatSet, Stealth, OldValue);
}

