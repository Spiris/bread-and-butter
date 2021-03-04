// Copyright Folding Sky Games LLC 2021 All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "GameStateCore.generated.h"

class UFlowManagerComponent;
UCLASS()
class BNB_API AGameStateCore : public AGameStateBase
{
	GENERATED_BODY()
public:
	AGameStateCore();
	UFlowManagerComponent* GetFlowManager() { return FlowManager; }
protected:
	UPROPERTY()
		UFlowManagerComponent* FlowManager;
};
