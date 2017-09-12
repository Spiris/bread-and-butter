// Fill out your copyright notice in the Description page of Project Settings.

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
