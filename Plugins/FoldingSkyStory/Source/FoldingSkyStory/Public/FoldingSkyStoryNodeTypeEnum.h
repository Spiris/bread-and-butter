// Copyright Folding Sky Games LLC 2021 All rights reserved.

#pragma once

#include "CoreMinimal.h"

#include "FoldingSkyStoryNodeTypeEnum.generated.h"

UENUM()
enum class EFoldingSkyStoryNodeType : uint8
{
	TextNode = 0U,
	DialogueNode = 1U,
	CustomNode = 2U
};