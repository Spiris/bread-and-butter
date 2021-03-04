// Copyright Folding Sky Games LLC 2021 All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "StatModifier.generated.h"

USTRUCT(BlueprintType)
struct BNB_API FStatModifier
{
	GENERATED_BODY()
public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		FString Key = "";
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		FString Source = "";
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		int32 Value = 0;
	FStatModifier() {}
};
