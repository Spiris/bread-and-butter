// Copyright Folding Sky Games LLC 2021 All rights reserved.

#include "Story/Widgets/AnimatedWordBlock.h"
#include "Story/Widgets/AnimatedLetterBlock.h"

#include "Components/HorizontalBox.h"


void UAnimatedWordBlock::SetWord(const FString& Word, TSubclassOf<UAnimatedLetterBlock> AnimatedLetterClass)
{
	if (AnimatedLetterClass)
	{
		if (UHorizontalBox* WordContainer = GetWordContainer())
		{
			WordContainer->ClearChildren();
			for (int32 i = 0; i < Word.Len(); i++)
			{
				UAnimatedLetterBlock* NewLetter = CreateWidget<UAnimatedLetterBlock>(this, AnimatedLetterClass);
				NewLetter->SetLetter(Word[i]);
				WordContainer->AddChild(NewLetter);
			}
		}
	}
}

void UAnimatedWordBlock::PlayWordAnimation()
{
	if (UWidgetAnimation* Anim = GetWordAnimation())
	{
		PlayAnimation(Anim);
	}
}
void UAnimatedWordBlock::PlayLetterAnimation(const int32& LetterIndex)
{
	TArray<UAnimatedLetterBlock*> Letters = GetLetters();
	if (Letters.IsValidIndex(LetterIndex))
	{
		Letters[LetterIndex]->PlayLetterAnimation();
	}
}
UAnimatedLetterBlock* UAnimatedWordBlock::GetLetter(const int32& LetterIndex)
{
	TArray<UAnimatedLetterBlock*> Letters = GetLetters();
	if (Letters.IsValidIndex(LetterIndex))
	{
		return Letters[LetterIndex];
	}
	return nullptr;
}

TArray<UAnimatedLetterBlock*> UAnimatedWordBlock::GetLetters()
{
	TArray<UAnimatedLetterBlock*> RetVal;
	if (UHorizontalBox* WordContainer = GetWordContainer())
	{
		const int32 count = WordContainer->GetChildrenCount();
		for (int32 i = 0; i < count; i++)
		{
			if (UAnimatedLetterBlock* LetterBlock = Cast<UAnimatedLetterBlock>(WordContainer->GetChildAt(i)))
			{
				RetVal.Add(LetterBlock);
			}
		}
	}
	return RetVal;
}