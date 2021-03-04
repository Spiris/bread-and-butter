// Copyright Folding Sky Games LLC 2021 All rights reserved.

#pragma once

#include "CoreMinimal.h"

#include "CharacterDamage.generated.h"

class UCharacterComponent;

UENUM(BlueprintType)
enum class ECharacterDamageType : uint8
{
	None,
	Melee,
	Ranged,
	Magical
};
UENUM(BlueprintType)
enum class ECharacterDamageResultType : uint8
{
	None,
	Miss,
	Dodged,
	Deflected,
	Resisted,
	Absorbed,
	HitPoints,
	Barrier,
	Overkill,
	Critical
};

USTRUCT(BlueprintType)
struct BNB_API FCharacterDamage
{
	GENERATED_BODY()
public:
	UPROPERTY(BlueprintReadOnly, Transient)
		UCharacterComponent* Source = nullptr;
	UPROPERTY(BlueprintReadOnly)
		int32 Raw = 0;
	UPROPERTY(BlueprintReadOnly)
		int32 Barrier = 0;
	UPROPERTY(BlueprintReadOnly)
		int32 HitPoints = 0;
	UPROPERTY(BlueprintReadOnly)
		ECharacterDamageType DamageType = ECharacterDamageType::None;
	UPROPERTY(BlueprintReadOnly)
		ECharacterDamageResultType ResultType = ECharacterDamageResultType::None;

	FCharacterDamage() {}
};