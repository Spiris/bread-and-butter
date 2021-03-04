// Copyright Folding Sky Games LLC 2021 All rights reserved.

#pragma once

#include "CoreMinimal.h"

#include "Components/TextBlock.h"
#include "StoryTextBlock.generated.h"


UCLASS()
class BNB_API UStoryTextBlock : public UTextBlock
{
	GENERATED_BODY()
public:
	FVector2D GetDesiredSize();
};