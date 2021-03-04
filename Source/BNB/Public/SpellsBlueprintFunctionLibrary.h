// Copyright Folding Sky Games LLC 2021 All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "SpellsBlueprintFunctionLibrary.generated.h"

USTRUCT(Blueprintable, BlueprintType)
struct FSpellKeyCombo
{
	GENERATED_BODY()
public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		FName Name;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		TArray<FKey> Incantation;

	FSpellKeyCombo() {}
};

UCLASS()
class BNB_API USpellsBlueprintFunctionLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
public:
	UFUNCTION(BlueprintCallable)
		static TArray<FSpellKeyCombo> FilterInvalidSpells(const FKey& InputKey, const int32& SpellPosition, const TArray<FSpellKeyCombo>& CurrentSpells);
	
};
