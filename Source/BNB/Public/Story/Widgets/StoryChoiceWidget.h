// Copyright Folding Sky Games LLC 2021 All rights reserved.

#pragma once

#include "CoreMinimal.h"

#include "Blueprint/UserWidget.h"
#include "StoryChoiceWidget.generated.h"

class UBorder;

UCLASS()
class BNB_API UStoryChoiceWidget : public UUserWidget
{
	GENERATED_BODY()
public:
	
	void SetStoryChoiceText(const FText& InText);
	void SetSelectionState(bool bValue);
protected:
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		FText StoryChoiceText = FText();
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		FLinearColor HighlightColor = FLinearColor::White;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		FLinearColor NormalColor = FLinearColor(0.4, 0.4, 0.4, 0.8);
	UFUNCTION(BlueprintPure, BlueprintImplementableEvent)
		UBorder* GetColorBorder();
};