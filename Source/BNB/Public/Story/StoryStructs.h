// Copyright Folding Sky Games LLC 2021 All rights reserved.

#pragma once

#include "CoreMinimal.h"

#include "FoldingSkyStoryStructs.h"
#include "StoryStructs.generated.h"

class UTexture2D;
class USoundCue;
class USceneComponent;
class UFoldingSkyStoryComponent;

USTRUCT(Blueprintable, BlueprintType)
struct BNB_API FStoryPoint
{
	GENERATED_BODY()
public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		FText SpeakerName = FText();
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		FText Text = FText();	
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		TSoftObjectPtr<UTexture2D> Image;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		TSoftObjectPtr<UDialogueWave> Dialogue;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		TArray<FText> Choices;

	FStoryPoint() {}
};

USTRUCT(Blueprintable, BlueprintType)
struct BNB_API FSpeakerParams
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		USceneComponent* Anchor = nullptr;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		FText Name;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		TSoftObjectPtr<UTexture2D> Image;
	FSpeakerParams() {};
	FSpeakerParams(USceneComponent* InAnchor, const FText& InName, const TSoftObjectPtr<UTexture2D>& InImage) {};
};

USTRUCT(Blueprintable, BlueprintType)
struct BNB_API FStoryParams
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		FText Name;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		FText Words;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		TArray<FText> Choices;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		TSoftObjectPtr<UTexture2D> Image;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		TSoftObjectPtr<UDialogueWave> Dialogue;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		FOnStoryChoiceMade NodeCallback;

	FStoryParams() {};
	FStoryParams(const FText& InName, const FText& InText, const TArray<FText>& InChoices, const TSoftObjectPtr<UTexture2D>& InImage) :
		Name(InName), Words(InText), Choices(InChoices), Image(InImage) {};
	FStoryParams(const FSpeakerParams& SpeakerParams, const FFoldingSkyStoryNodeParams& NodeParams) :
		Name(SpeakerParams.Name), Words(NodeParams.Words), Choices(NodeParams.Choices), Image(SpeakerParams.Image), Dialogue(NodeParams.Dialogue), NodeCallback(NodeParams.NodeCallback) {};

};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnStoryContentSet, FStoryPoint, StoryPoint, bool, bShouldAutoResize);
