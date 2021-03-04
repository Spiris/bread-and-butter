// Copyright Folding Sky Games LLC 2021 All rights reserved.

#include "FoldingSkyStoryGraphSchema.h"
#include "EdGraphSchema_K2_Actions.h"
#include "Kismet2/BlueprintEditorUtils.h"

UFoldingSkyStoryGraphSchema::UFoldingSkyStoryGraphSchema(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer) { }

UK2Node_VariableGet* UFoldingSkyStoryGraphSchema::SpawnVariableGetNode(const FVector2D GraphPosition, class UEdGraph* ParentGraph, FName VariableName, UStruct* Source) const
{
	return Super::SpawnVariableGetNode(GraphPosition, ParentGraph, VariableName, Source);
}

UK2Node_VariableSet* UFoldingSkyStoryGraphSchema::SpawnVariableSetNode(const FVector2D GraphPosition, class UEdGraph* ParentGraph, FName VariableName, UStruct* Source) const
{
	return Super::SpawnVariableSetNode(GraphPosition, ParentGraph, VariableName, Source);
}