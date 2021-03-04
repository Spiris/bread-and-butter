// Copyright Folding Sky Games LLC 2021 All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "CurrentStatus.generated.h"

USTRUCT(BlueprintType)
struct BNB_API FCharacterStatus
{
	GENERATED_BODY()
public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		int32 HitPoints = 0;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		int32 Barrier = 0;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		float PercentHitPoints = 0;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		float PercentBarrier = 0;
	void CalcPercents(const float& MaxHitPoints, const float& MaxBarrier);
	bool IsDead();
	FCharacterStatus() {}
};
