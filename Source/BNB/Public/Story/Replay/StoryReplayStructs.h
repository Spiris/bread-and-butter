// Copyright Folding Sky Games LLC 2021 All rights reserved.

#pragma once

#include "CoreMinimal.h"

#include "Story/StoryStructs.h"
#include "StoryReplayStructs.generated.h"

class UObject;
class USceneComponent;
class UFoldingSkyStoryComponent;
class APlayerController;

USTRUCT(Blueprintable, BlueprintType)
struct BNB_API FStorySceneSave
{
	GENERATED_BODY()
public:
	UPROPERTY(BlueprintReadWrite)
		USceneComponent* Anchor = nullptr;
	UPROPERTY(BlueprintReadWrite)
		APlayerController* Player = nullptr;
	UPROPERTY(BlueprintReadWrite)
		UFoldingSkyStoryComponent* Story = nullptr;
};
USTRUCT(Blueprintable, BlueprintType)
struct BNB_API FStoryReplaySave
{
	GENERATED_BODY()
public:
	UPROPERTY(BlueprintReadWrite)
		FOnStoryChoiceMade ChoiceCallback;
	UPROPERTY()
		UObject* NodeHardRef = nullptr;	
	UPROPERTY(BlueprintReadWrite)
		USceneComponent* Anchor = nullptr;
	UPROPERTY(BlueprintReadWrite)
		APlayerController* Player = nullptr;
	UPROPERTY(BlueprintReadWrite)
		UFoldingSkyStoryComponent* Story = nullptr;
	UPROPERTY(BlueprintReadWrite)
		FStoryPoint Point;
	UPROPERTY()
		int32 Index = -1;
	UPROPERTY()
		int32 PreviousIndex = -1;
	UPROPERTY()
		int32 NextIndex = -1;
	UPROPERTY()
		int32 SceneId = -1;
};