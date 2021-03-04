// Copyright Folding Sky Games LLC 2021 All rights reserved.

#include "Story/Widgets/StoryWrapBox.h"
#include "Story/Widgets/AnimatedWordBlock.h"
#include "Story/Widgets/AnimatedLetterBlock.h"

#include "Widgets/Layout/SWrapBox.h"

FVector2D UStoryWrapBox::GetDesiredSize()
{
	FVector2D DesiredSize = FVector2D::ZeroVector;
	if (!MyWrapBox.IsValid())
	{		
		RebuildWidget();
		SynchronizeProperties();
	}
	if (MyWrapBox.IsValid())
	{
		DesiredSize = MyWrapBox->ComputeDesiredSize(1.0f); // this does nothing, good show.
	}
	return DesiredSize;
}



void UStoryWrapBox::WriteAnimated(const FString& InString)
{
	CachedStoryWords.Empty();
	ClearChildren();
	InString.ParseIntoArrayWS(CachedStoryWords);
	const int32& LastWordIndex = CachedStoryWords.Num() - 1;
	for (int32 i = 0; i < LastWordIndex; i++)
	{
		AddWordBlock(CachedStoryWords[i] + TEXT(" "));
	}
	if (CachedStoryWords.IsValidIndex(LastWordIndex))
	{
		AddWordBlock(CachedStoryWords[LastWordIndex]);
	}
	AnimateLetter(0, 0);
}
void UStoryWrapBox::AddWordBlock(const FString& NewWordString)
{
	UAnimatedWordBlock* WordBlock = CreateWidget<UAnimatedWordBlock>(this, AnimatedWordClass);
	WordBlock->SetWord(NewWordString, AnimatedLetterClass);
	AddChild(WordBlock);
}
void UStoryWrapBox::AnimateLetter(const int32 WordIndex, const int32 LetterIndex)
{
	int32 NextWordIndex = WordIndex;
	int32 NextLetterIndex = LetterIndex + 1;
	if (UAnimatedWordBlock* WordBlock = Cast<UAnimatedWordBlock>(GetChildAt(WordIndex)))
	{
		if (UAnimatedLetterBlock* Letter = WordBlock->GetLetter(LetterIndex))
		{
			Letter->PlayLetterAnimation();
		}
		else
		{
			NextWordIndex += 1;
			NextLetterIndex = 0;
		}
		FTimerDelegate tdel = FTimerDelegate::CreateUObject(this,
			&UStoryWrapBox::AnimateLetter, NextWordIndex, NextLetterIndex);
		GetWorld()->GetTimerManager().SetTimer(AnimatedLetterHandle, tdel, AnimatedLetterDelay, false);
		return; // early out to or else the timer will get cleared
	}
	if (WordIndex > 0)
	{
		GetWorld()->GetTimerManager().ClearTimer(AnimatedLetterHandle);
	}
}
