// Copyright Folding Sky Games LLC 2021 All rights reserved.

#include "K2Node_FoldingSkyStoryText.h"

UK2Node_FoldingSkyStoryOneWayText::UK2Node_FoldingSkyStoryOneWayText(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	NodeType = EFoldingSkyStoryNodeType::TextNode;
	SetNumChoices(0);
}
UK2Node_FoldingSkyStoryTwoWayText::UK2Node_FoldingSkyStoryTwoWayText(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	NodeType = EFoldingSkyStoryNodeType::TextNode;
	SetNumChoices(-1);
}
UK2Node_FoldingSkyStoryWithChoicesText::UK2Node_FoldingSkyStoryWithChoicesText(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	NodeType = EFoldingSkyStoryNodeType::TextNode;
	SetNumChoices(1);
}