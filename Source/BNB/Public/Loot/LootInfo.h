// Copyright Folding Sky Games LLC 2021 All rights reserved.

#pragma once

#include "CoreMinimal.h"

#include "Engine/DataTable.h"
#include "LootInfo.generated.h"

UENUM(BlueprintType)
enum class ELootQuality : uint8
{
	Poor,
	Good,
	High,
	Master,
	Rare,
};
USTRUCT(Blueprintable)
struct BNB_API FLootChance 
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int32 ProbabilityOfDrop = 0;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FDataTableRowHandle LootHandle;
	FLootChance() {}
};

USTRUCT(Blueprintable, BlueprintType)
struct BNB_API FLootTableRow : public FTableRowBase
{
	GENERATED_BODY()
public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		FString Name = "Default";
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		ELootQuality Quality = ELootQuality::Poor;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		FString Description = "Default";
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		FString FlavorText = "Default";
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		TSoftObjectPtr<class UTexture2D> Image;
};