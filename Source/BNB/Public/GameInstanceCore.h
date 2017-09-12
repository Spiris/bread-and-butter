// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Engine/GameInstance.h"
#include "GameInstanceCore.generated.h"
class UWorldNexus;
UCLASS()
class BNB_API UGameInstanceCore : public UGameInstance
{
	GENERATED_BODY()
public:
	UGameInstanceCore();
	virtual void Init() override;
	virtual void Shutdown() override;
	UWorldNexus* GetWorldNexus();
protected:
	UPROPERTY()
		UWorldNexus* Nexus;
};
