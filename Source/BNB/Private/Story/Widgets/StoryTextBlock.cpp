// Copyright Folding Sky Games LLC 2021 All rights reserved.

#include "Story/Widgets/StoryTextBlock.h"
#include "Widgets/Text/STextBlock.h"

FVector2D UStoryTextBlock::GetDesiredSize()
{
	FVector2D DesiredSize = FVector2D::ZeroVector;
	if (!MyTextBlock.IsValid())
	{
		RebuildWidget();
		SynchronizeProperties();
	}
	if (MyTextBlock.IsValid())
	{
		DesiredSize = MyTextBlock->ComputeDesiredSize(1.0f);
	}
	return DesiredSize;
}