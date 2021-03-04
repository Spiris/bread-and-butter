// Copyright Folding Sky Games LLC 2021 All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/ObjectMacros.h"
#include "EdGraphSchema_K2.h"
#include "FoldingSkyStoryGraphSchema.generated.h"

UCLASS(MinimalAPI)
class UFoldingSkyStoryGraphSchema : public UEdGraphSchema_K2
{
	GENERATED_UCLASS_BODY()

	virtual class UK2Node_VariableGet* SpawnVariableGetNode(const FVector2D GraphPosition, class UEdGraph* ParentGraph, FName VariableName, UStruct* Source) const override;
	virtual class UK2Node_VariableSet* SpawnVariableSetNode(const FVector2D GraphPosition, class UEdGraph* ParentGraph, FName VariableName, UStruct* Source) const override;

	virtual bool ShouldAlwaysPurgeOnModification() const override { return true; }
};
