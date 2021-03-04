// Copyright Folding Sky Games LLC 2021 All rights reserved.

#include "GameStateCore.h"

#include "Flow/FlowManagerComponent.h"

AGameStateCore::AGameStateCore()
	: Super()
{
	FlowManager = CreateDefaultSubobject<UFlowManagerComponent>("Flow Manager");
}