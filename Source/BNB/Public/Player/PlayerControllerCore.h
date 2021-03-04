// Copyright Folding Sky Games LLC 2021 All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "PlayerControllerCore.generated.h"
class UPlayerCore;
/**
 * 
 */
UCLASS()
class BNB_API APlayerControllerCore : public APlayerController
{
	GENERATED_BODY()
public:
	virtual void BeginPlay() override;

	// CHARACTER -
public:
	FString GetCharacterName();
	int32 GetCharacterId();
	// PLAYER CORE - 
public:
	UPlayerCore* GetPlayerCore();
	FString GetPlayerNickname();
	int32 GetPlayerId();
private:
	UPlayerCore* PlayerCore;
};
