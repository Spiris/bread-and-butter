// Copyright Folding Sky Games LLC 2021 All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "StatModifier.h"
#include "CharacterClass.generated.h"

USTRUCT(BlueprintType)
struct BNB_API FCharacterClass
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FString Name = "";
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		TMap<FString, FStatModifier> Modifiers;
	int32 GetModifier(const FString& Key);
	FCharacterClass();
};
