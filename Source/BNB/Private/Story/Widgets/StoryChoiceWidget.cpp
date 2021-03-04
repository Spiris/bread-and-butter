// Copyright Folding Sky Games LLC 2021 All rights reserved.

#include "Story/Widgets/StoryChoiceWidget.h"

#include "Components/Border.h"

void UStoryChoiceWidget::SetStoryChoiceText(const FText& InText)
{
	StoryChoiceText = InText;
}
void UStoryChoiceWidget::SetSelectionState(bool bValue)
{
	if (UBorder* Border = GetColorBorder())
	{
		Border->SetBrushColor(bValue ? HighlightColor : NormalColor );
	}
}