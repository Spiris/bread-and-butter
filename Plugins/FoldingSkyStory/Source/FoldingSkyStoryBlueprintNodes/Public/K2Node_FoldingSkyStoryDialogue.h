// Copyright Folding Sky Games LLC 2021 All rights reserved.

#pragma once

#include "CoreMinimal.h"

#include "K2Node_FoldingSkyStory.h"
#include "K2Node_FoldingSkyStoryDialogue.generated.h"

UCLASS()
class FOLDINGSKYSTORYBLUEPRINTNODES_API UK2Node_FoldingSkyStoryOneWayDialogue : public UK2Node_FoldingSkyStoryTyped
{
	GENERATED_UCLASS_BODY()
};

UCLASS()
class FOLDINGSKYSTORYBLUEPRINTNODES_API UK2Node_FoldingSkyStoryTwoWayDialogue : public UK2Node_FoldingSkyStoryTyped
{
	GENERATED_UCLASS_BODY()
};
UCLASS()
class FOLDINGSKYSTORYBLUEPRINTNODES_API UK2Node_FoldingSkyStoryWithChoicesDialogue : public UK2Node_FoldingSkyStoryWithChoices
{
	GENERATED_UCLASS_BODY()
};