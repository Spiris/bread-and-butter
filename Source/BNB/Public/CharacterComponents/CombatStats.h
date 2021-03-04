// Copyright Folding Sky Games LLC 2021 All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "CombatStats.generated.h"



USTRUCT(BlueprintType)
struct BNB_API FCombatStats
{
	GENERATED_BODY()
public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		int32 HitPoints = 0;	
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		int32 Barrier = 0;	
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		int32 MoveSpeed = 0;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		int32 ArmorRating = 0;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		int32 SpellArmorRating = 0;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		int32 HitRating = 0;
	FCombatStats() {}
};
