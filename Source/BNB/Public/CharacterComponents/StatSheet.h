// Copyright Folding Sky Games LLC 2021 All rights reserved.

#pragma once

#include "CoreMinimal.h"

#include "AbilityScore.h"
#include "CharacterClass.h"
#include "Alignment.h"
#include "CombatStats.h"
#include "RollModifier.h"
#include "StatSheet.generated.h"

USTRUCT(BlueprintType)
struct BNB_API FStatSheet
{
	GENERATED_BODY()
public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		FCombatStats CombatStats;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		FRollModifier RollModifier;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		FAbilityScore AbilityScore;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		TArray<FCharacterClass> Classes;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		TMap<FString, FStatModifier> Modifiers;

	int32 RollMeleeHit();
	int32 RollRangedHit();
	int32 RollSpellHit();
	int32 GetMeleeHitBonus();
	int32 GetRangedHitBonus();
	int32 GetSpellHitBonus();

	int32 RollMeleeDamage(); 
	int32 RollRangedDamage();
	int32 RollSpellDamage();
	int32 GetMeleeDamageBonus();
	int32 GetRangedDamageBonus();
	int32 GetSpellDamageBonus();

	int32 RollMeleeAvoidance();
	int32 RollRangedAvoidance();
	int32 RollSpellAvoidance();

	int32 GetMeleeAvoidance();
	int32 GetRangedAvoidance();
	int32 GetSpellAvoidance();

	int32 GetMeleeDamageMitigation();
	int32 GetRangedDamageMitigation();
	int32 GetSpellDamageMitigation();

	int32 GetModifier(const FString& Key);

	FStatSheet();
};
