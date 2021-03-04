// Copyright Folding Sky Games LLC 2021 All rights reserved.

#pragma once

#include "CoreMinimal.h"

#include "Blueprint/UserWidget.h"
#include "AnimatedLetterBlock.generated.h"

class UTextBlock;
class UWidgetAnimation;


UCLASS(meta = (DisplayName = "AnimatedLetter"))
class BNB_API UAnimatedLetterBlock : public UUserWidget
{
	GENERATED_BODY()
public:
	void SetLetter(const TCHAR& Letter);
	void SetLetterAndPlayAnimation(const TCHAR& Letter);
	void PlayLetterAnimation();
	UFUNCTION(BlueprintPure, BlueprintImplementableEvent)
		UTextBlock* GetTextBlock();
	UFUNCTION(BlueprintPure, BlueprintImplementableEvent)
		UWidgetAnimation* GetAnimation();
};