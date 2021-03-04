// Copyright Folding Sky Games LLC 2021 All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "AbilityScore.h"
#include "Race.generated.h"

USTRUCT(BlueprintType)
struct BNB_API FRace
{
	GENERATED_BODY()
public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		FString Name;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		FAbilityScore AbilityModifier;
};
