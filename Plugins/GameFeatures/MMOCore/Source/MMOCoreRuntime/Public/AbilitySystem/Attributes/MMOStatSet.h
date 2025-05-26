#pragma once

#include "AbilitySystemComponent.h"
#include "AbilitySystem/Attributes/LyraAttributeSet.h"

#include "MMOStatSet.generated.h"

class UObject;
struct FFrame;


/**
 * UMMOStatSet
 */
UCLASS(BlueprintType)
class UMMOStatSet : public ULyraAttributeSet
{
	GENERATED_BODY()

public:

	UMMOStatSet();

	ATTRIBUTE_ACCESSORS(UMMOStatSet, Strength);
	ATTRIBUTE_ACCESSORS(UMMOStatSet, Stamina);
	ATTRIBUTE_ACCESSORS(UMMOStatSet, Agility);
	ATTRIBUTE_ACCESSORS(UMMOStatSet, Perception);
	ATTRIBUTE_ACCESSORS(UMMOStatSet, Intellect);
	ATTRIBUTE_ACCESSORS(UMMOStatSet, Willpower);
	ATTRIBUTE_ACCESSORS(UMMOStatSet, Endurance);
	ATTRIBUTE_ACCESSORS(UMMOStatSet, EnergyReserve);
	ATTRIBUTE_ACCESSORS(UMMOStatSet, CloseCombatEvasion);
	ATTRIBUTE_ACCESSORS(UMMOStatSet, RangedCombatEvasion);
	ATTRIBUTE_ACCESSORS(UMMOStatSet, Deflection);
	ATTRIBUTE_ACCESSORS(UMMOStatSet, MartialArts);
	ATTRIBUTE_ACCESSORS(UMMOStatSet, OneHanded);
	ATTRIBUTE_ACCESSORS(UMMOStatSet, TwoHanded);
	ATTRIBUTE_ACCESSORS(UMMOStatSet, Piercing);
	ATTRIBUTE_ACCESSORS(UMMOStatSet, WeaponSpeed);
	ATTRIBUTE_ACCESSORS(UMMOStatSet, MeleeSpeed);
	ATTRIBUTE_ACCESSORS(UMMOStatSet, SneakAttack);
	ATTRIBUTE_ACCESSORS(UMMOStatSet, QuickStrike);
	ATTRIBUTE_ACCESSORS(UMMOStatSet, PowerStrike);
	ATTRIBUTE_ACCESSORS(UMMOStatSet, RapidBlows);
	ATTRIBUTE_ACCESSORS(UMMOStatSet, Bow);
	ATTRIBUTE_ACCESSORS(UMMOStatSet, Pistol);
	ATTRIBUTE_ACCESSORS(UMMOStatSet, Rifle);
	ATTRIBUTE_ACCESSORS(UMMOStatSet, Sniper);
	ATTRIBUTE_ACCESSORS(UMMOStatSet, Shotgun);
	ATTRIBUTE_ACCESSORS(UMMOStatSet, Throwing);
	ATTRIBUTE_ACCESSORS(UMMOStatSet, Ranged);
	ATTRIBUTE_ACCESSORS(UMMOStatSet, SteadyShot);
	ATTRIBUTE_ACCESSORS(UMMOStatSet, QuickShot);
	ATTRIBUTE_ACCESSORS(UMMOStatSet, PowerShot);
	ATTRIBUTE_ACCESSORS(UMMOStatSet, RapidFire);
	ATTRIBUTE_ACCESSORS(UMMOStatSet, SilentShot);
	ATTRIBUTE_ACCESSORS(UMMOStatSet, Creation);
	ATTRIBUTE_ACCESSORS(UMMOStatSet, Metamorphosis);
	ATTRIBUTE_ACCESSORS(UMMOStatSet, Remedy);
	ATTRIBUTE_ACCESSORS(UMMOStatSet, Rejuvenation);
	ATTRIBUTE_ACCESSORS(UMMOStatSet, SpaceTime);
	ATTRIBUTE_ACCESSORS(UMMOStatSet, Destruction);
	ATTRIBUTE_ACCESSORS(UMMOStatSet, EnergySpeed);
	ATTRIBUTE_ACCESSORS(UMMOStatSet, Alchemy);
	ATTRIBUTE_ACCESSORS(UMMOStatSet, Programming);
	ATTRIBUTE_ACCESSORS(UMMOStatSet, Construction);
	ATTRIBUTE_ACCESSORS(UMMOStatSet, Engineering);
	ATTRIBUTE_ACCESSORS(UMMOStatSet, FirstAid);
	ATTRIBUTE_ACCESSORS(UMMOStatSet, Surgery);
	ATTRIBUTE_ACCESSORS(UMMOStatSet, Speed);
	ATTRIBUTE_ACCESSORS(UMMOStatSet, Flying);
	ATTRIBUTE_ACCESSORS(UMMOStatSet, Stealth);

protected:

	UFUNCTION()
	void OnRep_Strength(const FGameplayAttributeData& OldValue);

	UFUNCTION()
	void OnRep_Stamina(const FGameplayAttributeData& OldValue);
	UFUNCTION()
	void OnRep_Agility(const FGameplayAttributeData& OldValue);
	UFUNCTION()
	void OnRep_Perception(const FGameplayAttributeData& OldValue);
	UFUNCTION()
	void OnRep_Intellect(const FGameplayAttributeData& OldValue);
	UFUNCTION()
	void OnRep_Willpower(const FGameplayAttributeData& OldValue);
	UFUNCTION()
	void OnRep_Endurance(const FGameplayAttributeData& OldValue);
	UFUNCTION()
	void OnRep_EnergyReserve(const FGameplayAttributeData& OldValue);
	UFUNCTION()
	void OnRep_CloseCombatEvasion(const FGameplayAttributeData& OldValue);
	UFUNCTION()
	void OnRep_RangedCombatEvasion(const FGameplayAttributeData& OldValue);
	UFUNCTION()
	void OnRep_Deflection(const FGameplayAttributeData& OldValue);
	UFUNCTION()
	void OnRep_MartialArts(const FGameplayAttributeData& OldValue);
	UFUNCTION()
	void OnRep_OneHanded(const FGameplayAttributeData& OldValue);
	UFUNCTION()
	void OnRep_TwoHanded(const FGameplayAttributeData& OldValue);
	UFUNCTION()
	void OnRep_Piercing(const FGameplayAttributeData& OldValue);
	UFUNCTION()
	void OnRep_WeaponSpeed(const FGameplayAttributeData& OldValue);
	UFUNCTION()
	void OnRep_MeleeSpeed(const FGameplayAttributeData& OldValue);
	UFUNCTION()
	void OnRep_SneakAttack(const FGameplayAttributeData& OldValue);
	UFUNCTION()
	void OnRep_QuickStrike(const FGameplayAttributeData& OldValue);
	UFUNCTION()
	void OnRep_PowerStrike(const FGameplayAttributeData& OldValue);
	UFUNCTION()
	void OnRep_RapidBlows(const FGameplayAttributeData& OldValue);
	UFUNCTION()
	void OnRep_Bow(const FGameplayAttributeData& OldValue);
	UFUNCTION()
	void OnRep_Pistol(const FGameplayAttributeData& OldValue);
	UFUNCTION()
	void OnRep_Rifle(const FGameplayAttributeData& OldValue);
	UFUNCTION()
	void OnRep_Sniper(const FGameplayAttributeData& OldValue);
	UFUNCTION()
	void OnRep_Shotgun(const FGameplayAttributeData& OldValue);
	UFUNCTION()
	void OnRep_Throwing(const FGameplayAttributeData& OldValue);
	UFUNCTION()
	void OnRep_Ranged(const FGameplayAttributeData& OldValue);
	UFUNCTION()
	void OnRep_SteadyShot(const FGameplayAttributeData& OldValue);
	UFUNCTION()
	void OnRep_QuickShot(const FGameplayAttributeData& OldValue);
	UFUNCTION()
	void OnRep_PowerShot(const FGameplayAttributeData& OldValue);
	UFUNCTION()
	void OnRep_RapidFire(const FGameplayAttributeData& OldValue);
	UFUNCTION()
	void OnRep_SilentShot(const FGameplayAttributeData& OldValue);
	UFUNCTION()
	void OnRep_Creation(const FGameplayAttributeData& OldValue);
	UFUNCTION()
	void OnRep_Metamorphosis(const FGameplayAttributeData& OldValue);
	UFUNCTION()
	void OnRep_Remedy(const FGameplayAttributeData& OldValue);
	UFUNCTION()
	void OnRep_Rejuvenation(const FGameplayAttributeData& OldValue);
	UFUNCTION()
	void OnRep_SpaceTime(const FGameplayAttributeData& OldValue);
	UFUNCTION()
	void OnRep_Destruction(const FGameplayAttributeData& OldValue);
	UFUNCTION()
	void OnRep_EnergySpeed(const FGameplayAttributeData& OldValue);
	UFUNCTION()
	void OnRep_Alchemy(const FGameplayAttributeData& OldValue);
	UFUNCTION()
	void OnRep_Programming(const FGameplayAttributeData& OldValue);
	UFUNCTION()
	void OnRep_Construction(const FGameplayAttributeData& OldValue);
	UFUNCTION()
	void OnRep_Engineering(const FGameplayAttributeData& OldValue);
	UFUNCTION()
	void OnRep_FirstAid(const FGameplayAttributeData& OldValue);
	UFUNCTION()
	void OnRep_Surgery(const FGameplayAttributeData& OldValue);
	UFUNCTION()
	void OnRep_Speed(const FGameplayAttributeData& OldValue);
	UFUNCTION()
	void OnRep_Flying(const FGameplayAttributeData& OldValue);
	UFUNCTION()
	void OnRep_Stealth(const FGameplayAttributeData& OldValue);

private:

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_Strength, Category = "MMO|Stat", Meta = (AllowPrivateAccess = true))
	FGameplayAttributeData Strength;

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_Stamina, Category = "MMO|Stat", Meta = (AllowPrivateAccess = true))
	FGameplayAttributeData Stamina;

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_Agility, Category = "MMO|Stat", Meta = (AllowPrivateAccess = true))
	FGameplayAttributeData Agility;

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_Perception, Category = "MMO|Stat", Meta = (AllowPrivateAccess = true))
	FGameplayAttributeData Perception;

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_Intellect, Category = "MMO|Stat", Meta = (AllowPrivateAccess = true))
	FGameplayAttributeData Intellect;

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_Willpower, Category = "MMO|Stat", Meta = (AllowPrivateAccess = true))
	FGameplayAttributeData Willpower;

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_Endurance, Category = "MMO|Stat", Meta = (AllowPrivateAccess = true))
	FGameplayAttributeData Endurance;

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_EnergyReserve, Category = "MMO|Stat", Meta = (AllowPrivateAccess = true))
	FGameplayAttributeData EnergyReserve;

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_CloseCombatEvasion, Category = "MMO|Stat", Meta = (AllowPrivateAccess = true))
	FGameplayAttributeData CloseCombatEvasion;

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_RangedCombatEvasion, Category = "MMO|Stat", Meta = (AllowPrivateAccess = true))
	FGameplayAttributeData RangedCombatEvasion;

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_Deflection, Category = "MMO|Stat", Meta = (AllowPrivateAccess = true))
	FGameplayAttributeData Deflection;

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_MartialArts, Category = "MMO|Stat", Meta = (AllowPrivateAccess = true))
	FGameplayAttributeData MartialArts;

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_OneHanded, Category = "MMO|Stat", Meta = (AllowPrivateAccess = true))
	FGameplayAttributeData OneHanded;

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_TwoHanded, Category = "MMO|Stat", Meta = (AllowPrivateAccess = true))
	FGameplayAttributeData TwoHanded;

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_Piercing, Category = "MMO|Stat", Meta = (AllowPrivateAccess = true))
	FGameplayAttributeData Piercing;

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_WeaponSpeed, Category = "MMO|Stat", Meta = (AllowPrivateAccess = true))
	FGameplayAttributeData WeaponSpeed;

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_MeleeSpeed, Category = "MMO|Stat", Meta = (AllowPrivateAccess = true))
	FGameplayAttributeData MeleeSpeed;

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_SneakAttack, Category = "MMO|Stat", Meta = (AllowPrivateAccess = true))
	FGameplayAttributeData SneakAttack;

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_QuickStrike, Category = "MMO|Stat", Meta = (AllowPrivateAccess = true))
	FGameplayAttributeData QuickStrike;

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_PowerStrike, Category = "MMO|Stat", Meta = (AllowPrivateAccess = true))
	FGameplayAttributeData PowerStrike;

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_RapidBlows, Category = "MMO|Stat", Meta = (AllowPrivateAccess = true))
	FGameplayAttributeData RapidBlows;

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_Bow, Category = "MMO|Stat", Meta = (AllowPrivateAccess = true))
	FGameplayAttributeData Bow;

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_Pistol, Category = "MMO|Stat", Meta = (AllowPrivateAccess = true))
	FGameplayAttributeData Pistol;

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_Rifle, Category = "MMO|Stat", Meta = (AllowPrivateAccess = true))
	FGameplayAttributeData Rifle;

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_Sniper, Category = "MMO|Stat", Meta = (AllowPrivateAccess = true))
	FGameplayAttributeData Sniper;

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_Shotgun, Category = "MMO|Stat", Meta = (AllowPrivateAccess = true))
	FGameplayAttributeData Shotgun;

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_Throwing, Category = "MMO|Stat", Meta = (AllowPrivateAccess = true))
	FGameplayAttributeData Throwing;

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_Ranged, Category = "MMO|Stat", Meta = (AllowPrivateAccess = true))
	FGameplayAttributeData Ranged;

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_SteadyShot, Category = "MMO|Stat", Meta = (AllowPrivateAccess = true))
	FGameplayAttributeData SteadyShot;

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_QuickShot, Category = "MMO|Stat", Meta = (AllowPrivateAccess = true))
	FGameplayAttributeData QuickShot;

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_PowerShot, Category = "MMO|Stat", Meta = (AllowPrivateAccess = true))
	FGameplayAttributeData PowerShot;

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_RapidFire, Category = "MMO|Stat", Meta = (AllowPrivateAccess = true))
	FGameplayAttributeData RapidFire;

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_SilentShot, Category = "MMO|Stat", Meta = (AllowPrivateAccess = true))
	FGameplayAttributeData SilentShot;

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_Creation, Category = "MMO|Stat", Meta = (AllowPrivateAccess = true))
	FGameplayAttributeData Creation;

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_Metamorphosis, Category = "MMO|Stat", Meta = (AllowPrivateAccess = true))
	FGameplayAttributeData Metamorphosis;

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_Remedy, Category = "MMO|Stat", Meta = (AllowPrivateAccess = true))
	FGameplayAttributeData Remedy;

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_Rejuvenation, Category = "MMO|Stat", Meta = (AllowPrivateAccess = true))
	FGameplayAttributeData Rejuvenation;

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_SpaceTime, Category = "MMO|Stat", Meta = (AllowPrivateAccess = true))
	FGameplayAttributeData SpaceTime;

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_Destruction, Category = "MMO|Stat", Meta = (AllowPrivateAccess = true))
	FGameplayAttributeData Destruction;

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_EnergySpeed, Category = "MMO|Stat", Meta = (AllowPrivateAccess = true))
	FGameplayAttributeData EnergySpeed;

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_Alchemy, Category = "MMO|Stat", Meta = (AllowPrivateAccess = true))
	FGameplayAttributeData Alchemy;

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_Programming, Category = "MMO|Stat", Meta = (AllowPrivateAccess = true))
	FGameplayAttributeData Programming;

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_Construction, Category = "MMO|Stat", Meta = (AllowPrivateAccess = true))
	FGameplayAttributeData Construction;

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_Engineering, Category = "MMO|Stat", Meta = (AllowPrivateAccess = true))
	FGameplayAttributeData Engineering;

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_FirstAid, Category = "MMO|Stat", Meta = (AllowPrivateAccess = true))
	FGameplayAttributeData FirstAid;

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_Surgery, Category = "MMO|Stat", Meta = (AllowPrivateAccess = true))
	FGameplayAttributeData Surgery;

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_Speed, Category = "MMO|Stat", Meta = (AllowPrivateAccess = true))
	FGameplayAttributeData Speed;

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_Flying, Category = "MMO|Stat", Meta = (AllowPrivateAccess = true))
	FGameplayAttributeData Flying;

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_Stealth, Category = "MMO|Stat", Meta = (AllowPrivateAccess = true))
	FGameplayAttributeData Stealth;
};
