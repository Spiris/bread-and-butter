// Copyright Folding Sky Games LLC 2021 All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "AbilityScore.generated.h"

USTRUCT(BlueprintType)
struct BNB_API FAbilityScore
{
	GENERATED_BODY()
public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		int32 Strength = 0;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		int32 Dexterity = 0;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		int32 Magic = 0;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		int32 Vitality = 0;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		int32 Agility = 0;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		int32 Focus = 0;
	FAbilityScore() {}
};
