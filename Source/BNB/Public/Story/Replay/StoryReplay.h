// Copyright Folding Sky Games LLC 2021 All rights reserved.

#pragma once

#include "CoreMinimal.h"

#include "StoryReplayStructs.h"
#include "Components/ActorComponent.h"
#include "StoryReplay.generated.h"

class APlayerController;
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnReplayStory, const FStoryReplaySave&, ReplayData);

UCLASS(BlueprintType)
class BNB_API UStoryReplay : public UActorComponent
{
	GENERATED_BODY()
public:

	UFUNCTION(BlueprintPure, meta = (Category = "Story|Replay"))
		static UStoryReplay* GetStoryReplayHandler(APlayerController* OwningPlayer);

	UFUNCTION(BlueprintCallable, meta = (Category = "Story|Replay"))
		void SaveStoryProgress(FStoryReplaySave SaveData);
	UFUNCTION(BlueprintCallable, meta = (Category = "Story|Replay"))
		void ReplaySavedProgress(bool bForwardOrBack);
	UFUNCTION(BlueprintCallable, meta = (Category = "Story|Replay"))
		void ReplaySavedProgressById(int32 Id);
	UFUNCTION(BlueprintPure, meta = (Category = "Story|Replay"))
		FStoryReplaySave GetStoryProgress(int32 Id);
	UFUNCTION(BlueprintPure, meta = (Category = "Story|Replay"))
		FStorySceneSave GetSceneData(int32 Id);
	UPROPERTY(BlueprintAssignable, meta = (Category = "Story|Replay"))
		FOnReplayStory OnReplayStory;
protected:
	void ReplayStoryProgressInternal(const bool& bForwardOrBack);
	void SaveStoryProgressInternal(const FStoryReplaySave& StoryData);
	UPROPERTY()
		TMap<int32, FStorySceneSave> SavedScenes;
	UPROPERTY()
		TMap<int32, FStoryReplaySave> SavedStoryProgress;
	void ReplayStory(const int32& Index);
	void RemoveStoryChain(const int32& Index);
	int32 GetNextUniqueStoryIndex();
	int32 StoryProgressIndex = -1;
	int32 ReplayIndex = -1;
};