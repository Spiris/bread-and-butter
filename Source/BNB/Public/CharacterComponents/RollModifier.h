// Copyright Folding Sky Games LLC 2021 All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "StatModifier.h"
#include "Math/RandomStream.h"
#include "RollModifier.generated.h"

USTRUCT(BlueprintType)
struct BNB_API FRollModifier
{
	GENERATED_BODY()
public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		int32 Melee = 0;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		int32 Ranged = 0;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		int32 Spell = 0;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		int32 MeleeHit = 0;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		int32 RangedHit = 0;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		int32 SpellHit = 0;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		int32 Dodge = 0;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		int32 Defend = 0;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		int32 Resist = 0;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		TArray<FStatModifier> MeleeModifiers;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		TArray<FStatModifier> RangedModifiers;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		TArray<FStatModifier> SpellModifiers;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		TArray<FStatModifier> MeleeHitModifiers;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		TArray<FStatModifier> RangedHitModifiers;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		TArray<FStatModifier> SpellHitModifiers;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		TArray<FStatModifier> DodgeModifiers;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		TArray<FStatModifier> DefendModifiers;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		TArray<FStatModifier> ResistModifiers;

	FRandomStream Random;
	
	int32 MeleeRoll();
	int32 RangedRoll();
	int32 SpellRoll();

	int32 MeleeHitRoll();
	int32 RangedHitRoll();
	int32 SpellHitRoll();

	int32 DodgeRoll();
	int32 DefendRoll();
	int32 ResistRoll();
	FRollModifier();
};
