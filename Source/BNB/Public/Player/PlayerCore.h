// Copyright Folding Sky Games LLC 2021 All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Engine/LocalPlayer.h"
#include "PlayerCore.generated.h"

/**
 * 
 */
UCLASS()
class BNB_API UPlayerCore : public ULocalPlayer
{
	GENERATED_BODY()
public:
	UPlayerCore();
	virtual FString GetNickname() const override;
	int32 PlayerId;
};
