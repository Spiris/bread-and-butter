// Copyright Folding Sky Games LLC 2021 All rights reserved.

#include "Flow/FlowManagerComponent.h"
#include "CoreFunctionLibrary.h"
#include "GameStateCore.h"

UFlowManagerComponent::UFlowManagerComponent()
{
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}

UFlowManagerComponent* UFlowManagerComponent::Get(UObject* WorldContextObject)
{
	if (AGameStateCore* GameState = UCoreFunctionLibrary::GetGameStateCore(WorldContextObject))
	{
		return GameState->GetFlowManager();
	}
	return nullptr;
}

void UFlowManagerComponent::BeginPlay()
{
	Super::BeginPlay();

}

void UFlowManagerComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

