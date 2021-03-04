// Copyright Folding Sky Games LLC 2021 All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Modules/ModuleInterface.h"

class FFoldingSkyStoryBlueprintNodesModule : public IModuleInterface
{
public:
	FFoldingSkyStoryBlueprintNodesModule() { }
	virtual void StartupModule() override { };
	virtual void ShutdownModule() override { };	
};