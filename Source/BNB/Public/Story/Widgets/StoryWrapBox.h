// Copyright Folding Sky Games LLC 2021 All rights reserved.

#pragma once

#include "CoreMinimal.h"

#include "Components/WrapBox.h"
#include "StoryWrapBox.generated.h"

class UAnimatedWordBlock;
class UAnimatedLetterBlock;

// A widget component to handle writing animated letters inside a widget wrap box
UCLASS()
class BNB_API UStoryWrapBox : public UWrapBox
{
	GENERATED_BODY()
public:
	void WriteAnimated(const FString& InString);
	FVector2D GetDesiredSize(); // doesnt even function, thanks epic!
protected:
	void AddWordBlock(const FString& NewWordString);
	void AnimateLetter(const int32 WordIndex, const int32 LetterIndex);
	TArray<FString> CachedStoryWords;
	FTimerHandle AnimatedLetterHandle;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		TSubclassOf<UAnimatedWordBlock> AnimatedWordClass;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		TSubclassOf<UAnimatedLetterBlock> AnimatedLetterClass;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		float AnimatedLetterDelay = 0.1f;	
};