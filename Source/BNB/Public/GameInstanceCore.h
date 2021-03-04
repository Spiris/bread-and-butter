// Copyright Folding Sky Games LLC 2021 All rights reserved.

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
	FDelegateHandle FirstLoadHandle;
	void PostFirstLoadMap(UWorld* World);
	virtual void Shutdown() override;
	UWorldNexus* GetWorldNexus();
protected:
	UPROPERTY()
		UWorldNexus* Nexus;
};
