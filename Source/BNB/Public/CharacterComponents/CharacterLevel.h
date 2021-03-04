// Copyright Folding Sky Games LLC 2021 All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "CharacterLevel.generated.h"

USTRUCT(BlueprintType)
struct BNB_API FCharacterLevel
{
	GENERATED_BODY()
public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		int32 Value;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		int32 TotalExperience;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		int32 CurrentExperience;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		int32 NextLevel;
};
