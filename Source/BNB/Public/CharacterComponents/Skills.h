// Copyright Folding Sky Games LLC 2021 All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Skills.generated.h"

USTRUCT(BlueprintType)
struct BNB_API FSkills
{
	GENERATED_BODY()
public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		int32 Acrobatics;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		int32 Athletics;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		int32 Bluff;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		int32 Computers;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		int32 Culture;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		int32 Diplomacy;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		int32 Disguise;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		int32 Engineering;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		int32 Intimidate;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		int32 LifeScience;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		int32 Medicine;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		int32 Mysticism;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		int32 Perception;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		int32 PhysicalScience;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		int32 Piloting;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		TMap<FString, int32> Professions;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		int32 SenseMotive;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		int32 SleightOfHand;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		int32 Stealth;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		int32 Survival;
};
