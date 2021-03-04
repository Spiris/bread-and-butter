// Copyright Folding Sky Games LLC 2021 All rights reserved.

#include "K2Node_FoldingSkyStoryDialogue.h"

UK2Node_FoldingSkyStoryOneWayDialogue::UK2Node_FoldingSkyStoryOneWayDialogue(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	NodeType = EFoldingSkyStoryNodeType::DialogueNode;
	SetNumChoices(0);
}

UK2Node_FoldingSkyStoryTwoWayDialogue::UK2Node_FoldingSkyStoryTwoWayDialogue(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{	
	NodeType = EFoldingSkyStoryNodeType::DialogueNode;
	SetNumChoices(-1);
}

UK2Node_FoldingSkyStoryWithChoicesDialogue::UK2Node_FoldingSkyStoryWithChoicesDialogue(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	NodeType = EFoldingSkyStoryNodeType::DialogueNode;
	SetNumChoices(1);
}