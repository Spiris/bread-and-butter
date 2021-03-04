// Copyright Folding Sky Games LLC 2021 All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "AbilityScore.h"
#include "Theme.generated.h"

USTRUCT(BlueprintType)
struct BNB_API FTheme
{
	GENERATED_BODY()
public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		FString Name;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		FAbilityScore AbilityModifier;
};
