// Copyright Folding Sky Games LLC 2021 All rights reserved.

#include "Story/Widgets/AnimatedLetterBlock.h"

#include "Components/TextBlock.h"


void UAnimatedLetterBlock::SetLetter(const TCHAR& Letter)
{
	if(UTextBlock* Block =  GetTextBlock())
	{
		FString LetterAsString = "";
		LetterAsString.AppendChar(Letter);
		const FText& LetterAsText = FText::FromString(LetterAsString);
		Block->SetText(LetterAsText);
	}
}

void UAnimatedLetterBlock::SetLetterAndPlayAnimation(const TCHAR& Letter)
{
	SetLetter(Letter);
	PlayLetterAnimation();
}
void UAnimatedLetterBlock::PlayLetterAnimation()
{
	if (UWidgetAnimation* Anim = GetAnimation())
	{
		PlayAnimation(Anim);
	}
}


