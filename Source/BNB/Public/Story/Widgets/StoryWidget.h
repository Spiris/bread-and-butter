// Copyright Folding Sky Games LLC 2021 All rights reserved.

#pragma once

#include "CoreMinimal.h"

#include "Story/StoryStructs.h"
#include "Blueprint/UserWidget.h"
#include "StoryWidget.generated.h"

class UFont;
class UImage;
class UScrollBox;
class UStoryChoiceWidget;
class UStoryTextBlock;
class UStoryWrapBox;

UCLASS()
class BNB_API UStoryWidget : public UUserWidget
{
	GENERATED_BODY()
public:
	virtual void NativeConstruct() override;
	UFUNCTION(BlueprintCallable)
		void InitPosition(const FVector& AnchorLocation);
	UFUNCTION(BlueprintCallable)
		void UpdatePosition(const FVector& AnchorLocation);
	UFUNCTION(BlueprintCallable)
		void SetStoryContent(const FStoryPoint& StoryPoint, bool bAutoResize);
	UFUNCTION(BlueprintCallable)
		void ChangeChoice(int32 Modifier);
	UFUNCTION(BlueprintCallable)
		void SelectChoice(int32 NewChoice);
	
	UFUNCTION(BlueprintPure)
		int32 GetCurrentChoice() const { return SelectedChoice; }
	UFUNCTION(BlueprintPure)
		FText GetCurrentChoiceText() const;
protected:
	FVector2D ViewportPosition = FVector2D::ZeroVector;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		FText NameText = FText();
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		FText StoryText = FText();
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		TSoftObjectPtr<UTexture2D> StoryImage;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		TSoftObjectPtr<UTexture2D> DefaultUnknownImage;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		TSoftObjectPtr<USoundCue> StorySound;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		TArray<FText> StoryChoices;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		TSubclassOf<UStoryChoiceWidget> StoryChoiceClass;
	UPROPERTY(BlueprintReadWrite)
		TArray<UStoryChoiceWidget*> StoryChoiceWidgets;
	UPROPERTY(BlueprintReadWrite)
		int32 SelectedChoice = -1;
	
	void SetStoryText(const FText& InText, bool bAutoResize);
	
	// Used to resize widget as a whole including border size	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float Buffer = 64.f;
	
	UFUNCTION(BlueprintPure, BlueprintImplementableEvent)
		UStoryTextBlock* GetTextContainer();
	UFUNCTION(BlueprintPure, BlueprintImplementableEvent)
		UScrollBox* GetChoiceContainer();
	UFUNCTION(BlueprintPure, BlueprintImplementableEvent)
		UImage* GetPortraitContainer();
	UFUNCTION(BlueprintPure, BlueprintImplementableEvent)
		UStoryWrapBox* GetWrapBox();
	

	void SetScreenPosition(const FVector2D& Target, const FVector2D& TailTarget);
	void SetChoices(const TArray<FText>& Choices);
	void SetPortrait(TSoftObjectPtr<UTexture2D> Image);
	FVector2D GetSizeFromText();
	int32 WrapIntInclusive(int32 Value, int32 Min, int32 Max) const;

	// Tail Handling Code
	UPROPERTY(BlueprintReadOnly)
		TArray<FVector2D> TailBorderVecs;
	UPROPERTY(BlueprintReadOnly)
		TArray<FVector2D> TailInsideVecs;
	UPROPERTY(BlueprintReadOnly)
		FVector2D TailTip;
};