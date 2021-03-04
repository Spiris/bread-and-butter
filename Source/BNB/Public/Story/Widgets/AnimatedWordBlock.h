// Copyright Folding Sky Games LLC 2021 All rights reserved.

#pragma once

#include "CoreMinimal.h"

#include "Blueprint/UserWidget.h"
#include "AnimatedWordBlock.generated.h"

class UHorizontalBox;
class UWidgetAnimation;
class UAnimatedLetterBlock;

UCLASS(meta = (DisplayName = "AnimatedWord"))
class BNB_API UAnimatedWordBlock : public UUserWidget
{
	GENERATED_BODY()
public:
	void SetWord(const FString& Word, TSubclassOf<UAnimatedLetterBlock> AnimatedLetterClass = nullptr);
	void PlayWordAnimation();
	void PlayLetterAnimation(const int32& LetterIndex);
	UAnimatedLetterBlock* GetLetter(const int32& LetterIndex);
	TArray<UAnimatedLetterBlock*> GetLetters();

	UFUNCTION(BlueprintPure, BlueprintImplementableEvent)
		UHorizontalBox* GetWordContainer();
	UFUNCTION(BlueprintPure, BlueprintImplementableEvent)
		UWidgetAnimation* GetWordAnimation();
};