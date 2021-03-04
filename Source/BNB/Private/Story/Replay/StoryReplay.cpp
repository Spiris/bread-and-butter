// Copyright Folding Sky Games LLC 2021 All rights reserved.

#include "Story/Replay/StoryReplay.h"

#include "Story/StoryBlueprintFunctionLibrary.h"
#include "GameFramework/PlayerController.h"
#include "FoldingSkyStoryComponent.h"


UStoryReplay* UStoryReplay::GetStoryReplayHandler(APlayerController* OwningPlayer)
{
	UStoryReplay* comp = OwningPlayer->FindComponentByClass<UStoryReplay>();
	if (!comp)
	{
		comp = Cast<UStoryReplay>(OwningPlayer->AddComponentByClass(UStoryReplay::StaticClass(), false, FTransform::Identity, false));		
	}
	return comp;
}

FStoryReplaySave UStoryReplay::GetStoryProgress(int32 Id) 
{
	if (FStoryReplaySave* s = SavedStoryProgress.Find(Id))
	{
		return *s;
	}
	return FStoryReplaySave();
}

FStorySceneSave UStoryReplay::GetSceneData(int32 Id)
{
	if (FStorySceneSave* s = SavedScenes.Find(Id))
	{
		return *s;
	}
	return FStorySceneSave();
}
void UStoryReplay::SaveStoryProgress(FStoryReplaySave SaveData)
{		
	SaveData.NodeHardRef = SaveData.ChoiceCallback.GetUObject();
	SaveStoryProgressInternal(SaveData);
}

void UStoryReplay::SaveStoryProgressInternal(const FStoryReplaySave& StoryData)
{
	int32 NewStoryIndex = StoryData.Index;
	FStoryReplaySave* NewSave = SavedStoryProgress.Find(NewStoryIndex);
	int32 PreviousSaveIndex = ReplayIndex;
	if (NewSave)
	{
		PreviousSaveIndex = NewSave->PreviousIndex;
	}
	else
	{
		NewStoryIndex = GetNextUniqueStoryIndex();
		NewSave = &SavedStoryProgress.Add(NewStoryIndex, StoryData);
	}
	if (FStoryReplaySave* PreviousSave = SavedStoryProgress.Find(PreviousSaveIndex))
	{
		if (PreviousSave->NextIndex != NewStoryIndex)
		{
			RemoveStoryChain(PreviousSave->NextIndex);
		}
		PreviousSave->NextIndex = NewStoryIndex;
		NewSave->PreviousIndex = PreviousSave->Index;
	}
	ReplayIndex = NewSave->Index = NewStoryIndex;
}

void UStoryReplay::ReplaySavedProgress(bool bForwardOrBack)
{
	ReplayStoryProgressInternal(bForwardOrBack);
}
void UStoryReplay::ReplayStoryProgressInternal(const bool& bForwardOrBack)
{
	if (FStoryReplaySave* Saved = SavedStoryProgress.Find(ReplayIndex))
	{
		const int32& Index = bForwardOrBack ? Saved->NextIndex : Saved->PreviousIndex;
		ReplayStory(Index);
	}
	else
	{
		ReplayStory(ReplayIndex);
	}
}

void UStoryReplay::ReplaySavedProgressById(int32 Id)
{
	ReplayStory(Id);
}

void UStoryReplay::ReplayStory(const int32& Index)
{
	if (FStoryReplaySave* Saved = SavedStoryProgress.Find(Index))
	{
		const FStoryReplaySave& SavedStory = *Saved;		
		OnReplayStory.Broadcast(SavedStory);
		ReplayIndex = Index;
	}
}

void UStoryReplay::RemoveStoryChain(const int32& Index)
{
	FStoryReplaySave NextSave;
	if (SavedStoryProgress.RemoveAndCopyValue(Index, NextSave))
	{
		NextSave.ChoiceCallback.Unbind();
		RemoveStoryChain(NextSave.NextIndex);
	}
}

int32 UStoryReplay::GetNextUniqueStoryIndex()
{
	StoryProgressIndex++;
	return StoryProgressIndex;
}