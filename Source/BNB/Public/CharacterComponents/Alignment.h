// Copyright Folding Sky Games LLC 2021 All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Alignment.generated.h"

//UENUM(BlueprintType)
//enum EAlignment : public uint8
//{
//	LawfulGood,
//	LawfulNeutral,
//	LawfulEvil,
//	NeutralGood,
//	Neutral,
//	NeutralEvil,
//	ChaoticGood,
//	ChaoticNeutral,
//	ChaoticEvil
//};
USTRUCT(BlueprintType)
struct BNB_API FAlignment
{
	GENERATED_BODY()
public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		int32 Value = 0;
};
