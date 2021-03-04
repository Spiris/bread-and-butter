// Copyright Folding Sky Games LLC 2021 All rights reserved.

#pragma once

#include "CoreMinimal.h"

#include "FoldingSkyStoryStructs.h"
#include "Components/ActorComponent.h"
#include "FoldingSkyStoryComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnStoryPosted, const bool, bIsReplayed, const FFoldingSkyStoryNodeParams&, Params);

class APlayerController;
class UPlayerStoryComponent;

UCLASS(Blueprintable)
class FOLDINGSKYSTORY_API UFoldingSkyStoryComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UFoldingSkyStoryComponent();
	/** Fires the OnBeginStoryGraph event, call this to 
	*	notify the blueprint it should begin traversing its story nodes  */
	UFUNCTION(BlueprintCallable)
		void BeginStoryGraph();
	/**	Resends a story node with the node callback delegate, posting the story 
	*	but not indicating it should be saved again, if a save system is in use. */
	UFUNCTION(BlueprintCallable)
		void ResendStory(const FOnStoryChoiceMade& ChoiceCallback, const FFoldingSkyStoryNodeParams& Params);
	/** Accepts a numeric value representing the choice on the blueprint node that is desired
	*	-1 fires the cancellation event
	*	invalid choice indicies will not be processed */
	UFUNCTION(BlueprintCallable)
		void AcceptChoiceValue(int32 Choice);

	/**	Fired when BeginStoryGraph() is called to notify the blueprint
	*	it should start traversing it's story nodes */
	UPROPERTY(BlueprintAssignable)
		FOnStoryActionEvent OnStoryBeginGraph;
	/**	Called EVERY time a story node is reached on the blueprint graph, 
	*	even when resending a previously sent node  */
	UPROPERTY(BlueprintAssignable)
		FOnStoryPosted OnStoryPosted;

	/**	The default text that will display for one way story nodes belonging to this story component  */
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FText OneWayOptionText;
	/**	The default affirmative text that will display for two way story nodes belonging to this story component  */
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FText TwoWayStoryOptionOneText;
	/**	The default negative text that will display for two way story nodes belonging to this story component  */
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FText TwoWayStoryOptionTwoText;

	void SendStory_NodeInternalUseOnly(const FFoldingSkyStoryNodeParams& NodeParams);
	TArray<FText> GetBasicChoices() const;
	TArray<FText> GetBinaryChoices() const;
protected:
	
	/** An event to be bound by the story blueprint graph, signaling the story graphs hould begin traversal of it's nodes */
	UFUNCTION(BlueprintImplementableEvent, Category = Story, meta = (DisplayName = "On Story Begin Graph"))
		void OnStoryBeginGraph_BP();
	/** An event to be bound by the story blueprint graph, fired every time 
	*	a story node is reached or story content has been supplied for posting */
	UFUNCTION(BlueprintImplementableEvent, Category = Story, meta = (DisplayName = "On Story Posted"))
		void OnStoryPosted_BP(const bool bIsReplayed, const FFoldingSkyStoryNodeParams& Params);
	/**	called when the player interacting with this graph presses the 
	*	accept binding this calls to OnStoryGetCurrentChoice_BP by default 
	*	and will provide the choice supplied therein at that time */
	UFUNCTION(BlueprintImplementableEvent, Category = Story, meta = (DisplayName = "On Story Choice Accepted"))
		void OnStoryChoiceAccepted_BP(const int32& Choice, const FText& ChoiceText);

	/**	 calls the OnStoryPosted events and sets the proxy callback */
	void PostStory(const bool bIsReplayed, const FFoldingSkyStoryNodeParams& Params);
	/** Data involving the story node that is currently posted, includes the callback for the blueprint node */
	UPROPERTY()
		FFoldingSkyStoryNodeParams CurrentPostedStory;
	/** Data involving the story node that is currently posted, includes the callback for the blueprint node */
	UPROPERTY()
		int32 ChosenValue = MIN_int32;
};
