// Copyright Folding Sky Games LLC 2021 All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Modules/ModuleInterface.h"

class FFoldingSkyStoryModule : public IModuleInterface
{
public:
	FFoldingSkyStoryModule() { }
	virtual void StartupModule() override { };
	virtual void ShutdownModule() override { };
};