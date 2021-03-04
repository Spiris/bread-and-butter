// Copyright Folding Sky Games LLC 2021 All rights reserved.

#pragma once

#include "CoreMinimal.h"

#include "FoldingSkyStoryNodeTypeEnum.h"
#include "FoldingSkyStoryStructs.h"
#include "UObject/Object.h"
#include "FoldingSkyStoryCallbackProxy.generated.h"

#define MAXSTORYCHOICES 10

class UFoldingSkyStoryComponent;
class UDialogueWave; // #include "Sound/DialogueWave.h"
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnStoryOptionChosen);

/** The Story Callback Proxies are used to compose the latent nodes in the story blueprint 
*	each story component manages moving the graph forward by using the callback to message the
*	blueprint node when it is ready to move on, and along what path */
UCLASS(MinimalAPI)
class UBaseFoldingSkyStoryCallbackProxy : public UObject
{
	GENERATED_BODY()
public:
	virtual UWorld* GetWorld() const override;
	virtual void BeginDestroy() override;
	/** Fired every time any choice is made even if that choice is to cancel */
	UPROPERTY(BlueprintAssignable)
		FOnStoryOptionChosen OnComplete;
	/** Fired only when the choice to cancel is made */
	UPROPERTY(BlueprintAssignable)
		FOnStoryOptionChosen OnCancel;
	/** this function indicates that the story blueprint node has been reached and is 
	*	awaiting input to determine what path to take. It sends its contents to the 
	*	story component so that they may be used elsewhere */
	void Trigger(UFoldingSkyStoryComponent* StoryComponent,
		const FText& Text,
		const TArray<FText>& Choices);
	void Trigger(UFoldingSkyStoryComponent* StoryComponent,
		const TSoftObjectPtr<UDialogueWave>& Dia,
		const TArray<FText>& Choices);
	void Trigger(UFoldingSkyStoryComponent* StoryComponent,
		const FString& CustomData,
		const TArray<FText>& Choices);
protected:
	/**	The code representation of the array of choices as output pins on the blueprint graph */
	virtual TArray<FOnStoryOptionChosen*> GetStoryOptionPins() { return TArray<FOnStoryOptionChosen*>(); };
	/** This function is called by the story component the node is waiting on to determine
	*	the route on the blueprint graph that the code will traverse */
	UFUNCTION()
		void OnOptionChosen(int32 Option);
};

/** The basic callback proxy displays only one choice, and uses a value defined on 
*	the story component to determine what text is shown for its choice */
UCLASS(MinimalAPI)
class UOneWayFoldingSkyStoryCallbackProxy : public UBaseFoldingSkyStoryCallbackProxy
{
	GENERATED_BODY()
public:
	/** called as option 0, the text used to denote this even is the value assigned to 
	*	The StoryComponent variable : OneWayOptionText  */
	UPROPERTY(BlueprintAssignable)
		FOnStoryOptionChosen OnWayOne;
	virtual TArray<FOnStoryOptionChosen*> GetStoryOptionPins() override;
};

/** The binary callback proxy displays only two choices, and uses values defined on
*	the story component to determine what text is shown for its choices */
UCLASS(MinimalAPI)
class UTwoWayFoldingSkyStoryCallbackProxy : public UBaseFoldingSkyStoryCallbackProxy
{
	GENERATED_BODY()
public:
	/** called as option 0, the text used to denote this even is the value assigned to
	*	The StoryComponent variable : TwoWayStoryOptionOneText  */
	UPROPERTY(BlueprintAssignable)
		FOnStoryOptionChosen OnWayOne;
	/** called as option 1, the text used to denote this even is the value assigned to
	*	The StoryComponent variable : TwoWayStoryOptionTwoText  */
	UPROPERTY(BlueprintAssignable)
		FOnStoryOptionChosen OnWayTwo;
	virtual TArray<FOnStoryOptionChosen*> GetStoryOptionPins() override;
};

UCLASS(MinimalAPI)
class UOneChoiceFoldingSkyStoryCallbackProxy : public UBaseFoldingSkyStoryCallbackProxy
{
	GENERATED_BODY()
public:
	/** event routed as a result from chosing option 0, the input text 'can' be relational */
	UPROPERTY(BlueprintAssignable)
		FOnStoryOptionChosen OnChoice1;
	virtual TArray<FOnStoryOptionChosen*> GetStoryOptionPins() override;
};

UCLASS(MinimalAPI)
class UTwoChoiceFoldingSkyStoryCallbackProxy : public UBaseFoldingSkyStoryCallbackProxy
{
	GENERATED_BODY()
public:
	/** event routed as a result from chosing option 0 */
	UPROPERTY(BlueprintAssignable)
		FOnStoryOptionChosen OnChoice1;
	/** event routed as a result from chosing option 1 */
	UPROPERTY(BlueprintAssignable)
		FOnStoryOptionChosen OnChoice2;
	virtual TArray<FOnStoryOptionChosen*> GetStoryOptionPins() override;
};

UCLASS(MinimalAPI)
class UThreeChoiceFoldingSkyStoryCallbackProxy : public UBaseFoldingSkyStoryCallbackProxy
{
	GENERATED_BODY()
public:
	/** event routed as a result from chosing option 0 */
	UPROPERTY(BlueprintAssignable)
		FOnStoryOptionChosen OnChoice1;
	/** event routed as a result from chosing option 1 */
	UPROPERTY(BlueprintAssignable)
		FOnStoryOptionChosen OnChoice2;
	/** event routed as a result from chosing option 2 */
	UPROPERTY(BlueprintAssignable)
		FOnStoryOptionChosen OnChoice3;
	virtual TArray<FOnStoryOptionChosen*> GetStoryOptionPins() override;
};

UCLASS(MinimalAPI)
class UFourChoiceFoldingSkyStoryCallbackProxy : public UBaseFoldingSkyStoryCallbackProxy
{
	GENERATED_BODY()
public:
	/** event routed as a result from chosing option 0 */
	UPROPERTY(BlueprintAssignable)
		FOnStoryOptionChosen OnChoice1;
	/** event routed as a result from chosing option 1 */
	UPROPERTY(BlueprintAssignable)
		FOnStoryOptionChosen OnChoice2;
	/** event routed as a result from chosing option 2 */
	UPROPERTY(BlueprintAssignable)
		FOnStoryOptionChosen OnChoice3;
	/** event routed as a result from chosing option 3 */
	UPROPERTY(BlueprintAssignable)
		FOnStoryOptionChosen OnChoice4;
	virtual TArray<FOnStoryOptionChosen*> GetStoryOptionPins() override;
};

UCLASS(MinimalAPI)
class UFiveChoiceFoldingSkyStoryCallbackProxy : public UBaseFoldingSkyStoryCallbackProxy
{
	GENERATED_BODY()
public:
	/** event routed as a result from chosing option 0 */
	UPROPERTY(BlueprintAssignable)
		FOnStoryOptionChosen OnChoice1;
	/** event routed as a result from chosing option 1 */
	UPROPERTY(BlueprintAssignable)
		FOnStoryOptionChosen OnChoice2;
	/** event routed as a result from chosing option 2 */
	UPROPERTY(BlueprintAssignable)
		FOnStoryOptionChosen OnChoice3;
	/** event routed as a result from chosing option 3 */
	UPROPERTY(BlueprintAssignable)
		FOnStoryOptionChosen OnChoice4;
	/** event routed as a result from chosing option 4 */
	UPROPERTY(BlueprintAssignable)
		FOnStoryOptionChosen OnChoice5;
	virtual TArray<FOnStoryOptionChosen*> GetStoryOptionPins() override;
};

UCLASS(MinimalAPI)
class USixChoiceFoldingSkyStoryCallbackProxy : public UBaseFoldingSkyStoryCallbackProxy
{
	GENERATED_BODY()
public:
	/** event routed as a result from chosing option 0 */
	UPROPERTY(BlueprintAssignable)
		FOnStoryOptionChosen OnChoice1;
	/** event routed as a result from chosing option 1 */
	UPROPERTY(BlueprintAssignable)
		FOnStoryOptionChosen OnChoice2;
	/** event routed as a result from chosing option 2 */
	UPROPERTY(BlueprintAssignable)
		FOnStoryOptionChosen OnChoice3;
	/** event routed as a result from chosing option 3 */
	UPROPERTY(BlueprintAssignable)
		FOnStoryOptionChosen OnChoice4;
	/** event routed as a result from chosing option 4 */
	UPROPERTY(BlueprintAssignable)
		FOnStoryOptionChosen OnChoice5;
	/** event routed as a result from chosing option 5 */
	UPROPERTY(BlueprintAssignable)
		FOnStoryOptionChosen OnChoice6;
	virtual TArray<FOnStoryOptionChosen*> GetStoryOptionPins() override;
};
UCLASS(MinimalAPI)
class USevenChoiceFoldingSkyStoryCallbackProxy : public UBaseFoldingSkyStoryCallbackProxy
{
	GENERATED_BODY()
public:
	UPROPERTY(BlueprintAssignable)
		FOnStoryOptionChosen OnChoice1;
	UPROPERTY(BlueprintAssignable)
		FOnStoryOptionChosen OnChoice2;
	UPROPERTY(BlueprintAssignable)
		FOnStoryOptionChosen OnChoice3;
	UPROPERTY(BlueprintAssignable)
		FOnStoryOptionChosen OnChoice4;
	UPROPERTY(BlueprintAssignable)
		FOnStoryOptionChosen OnChoice5;
	UPROPERTY(BlueprintAssignable)
		FOnStoryOptionChosen OnChoice6;
	UPROPERTY(BlueprintAssignable)
		FOnStoryOptionChosen OnChoice7;
	virtual TArray<FOnStoryOptionChosen*> GetStoryOptionPins() override;
};
UCLASS(MinimalAPI)
class UEightChoiceFoldingSkyStoryCallbackProxy : public UBaseFoldingSkyStoryCallbackProxy
{
	GENERATED_BODY()
public:
	UPROPERTY(BlueprintAssignable)
		FOnStoryOptionChosen OnChoice1;
	UPROPERTY(BlueprintAssignable)
		FOnStoryOptionChosen OnChoice2;
	UPROPERTY(BlueprintAssignable)
		FOnStoryOptionChosen OnChoice3;
	UPROPERTY(BlueprintAssignable)
		FOnStoryOptionChosen OnChoice4;
	UPROPERTY(BlueprintAssignable)
		FOnStoryOptionChosen OnChoice5;
	UPROPERTY(BlueprintAssignable)
		FOnStoryOptionChosen OnChoice6;
	UPROPERTY(BlueprintAssignable)
		FOnStoryOptionChosen OnChoice7;
	UPROPERTY(BlueprintAssignable)
		FOnStoryOptionChosen OnChoice8;
	virtual TArray<FOnStoryOptionChosen*> GetStoryOptionPins() override;
};
UCLASS(MinimalAPI)
class UNineChoiceFoldingSkyStoryCallbackProxy : public UBaseFoldingSkyStoryCallbackProxy
{
	GENERATED_BODY()
public:
	UPROPERTY(BlueprintAssignable)
		FOnStoryOptionChosen OnChoice1;
	UPROPERTY(BlueprintAssignable)
		FOnStoryOptionChosen OnChoice2;
	UPROPERTY(BlueprintAssignable)
		FOnStoryOptionChosen OnChoice3;
	UPROPERTY(BlueprintAssignable)
		FOnStoryOptionChosen OnChoice4;
	UPROPERTY(BlueprintAssignable)
		FOnStoryOptionChosen OnChoice5;
	UPROPERTY(BlueprintAssignable)
		FOnStoryOptionChosen OnChoice6;
	UPROPERTY(BlueprintAssignable)
		FOnStoryOptionChosen OnChoice7;
	UPROPERTY(BlueprintAssignable)
		FOnStoryOptionChosen OnChoice8;
	UPROPERTY(BlueprintAssignable)
		FOnStoryOptionChosen OnChoice9;
	virtual TArray<FOnStoryOptionChosen*> GetStoryOptionPins() override;
};
UCLASS(MinimalAPI)
class UTenChoiceFoldingSkyStoryCallbackProxy : public UBaseFoldingSkyStoryCallbackProxy
{
	GENERATED_BODY()
public:
	UPROPERTY(BlueprintAssignable)
		FOnStoryOptionChosen OnChoice1;
	UPROPERTY(BlueprintAssignable)
		FOnStoryOptionChosen OnChoice2;
	UPROPERTY(BlueprintAssignable)
		FOnStoryOptionChosen OnChoice3;
	UPROPERTY(BlueprintAssignable)
		FOnStoryOptionChosen OnChoice4;
	UPROPERTY(BlueprintAssignable)
		FOnStoryOptionChosen OnChoice5;
	UPROPERTY(BlueprintAssignable)
		FOnStoryOptionChosen OnChoice6;
	UPROPERTY(BlueprintAssignable)
		FOnStoryOptionChosen OnChoice7;
	UPROPERTY(BlueprintAssignable)
		FOnStoryOptionChosen OnChoice8;
	UPROPERTY(BlueprintAssignable)
		FOnStoryOptionChosen OnChoice9;
	UPROPERTY(BlueprintAssignable)
		FOnStoryOptionChosen OnChoice10;
	virtual TArray<FOnStoryOptionChosen*> GetStoryOptionPins() override;
};

UCLASS(MinimalAPI)
class UFoldingSkyStoryCallbackProxyFactory : public UObject
{
	GENERATED_BODY()
public:	
	// BEGIN TEXT FUNCTIONS
	template<class T>
	static T* TriggerTextStoryNode(UFoldingSkyStoryComponent* StoryComponent, const FText& Text, const TArray<FText>& Choices)
	{
		T* Proxy = NewObject<T>(StoryComponent);
		if (Proxy)
		{
			Proxy->Trigger(StoryComponent, Text, Choices);
		}
		return Proxy;
	}
	UFUNCTION(BlueprintCallable, BlueprintInternalUseOnly, meta = (HidePin = "StoryComponent", DefaultToSelf = "StoryComponent", DisplayName = "Story - One Way Text"), Category = "Story|Nodes")
		static UOneWayFoldingSkyStoryCallbackProxy* CreateBasicStoryProxyObject(
			UFoldingSkyStoryComponent* StoryComponent,
			FText StoryText);
	UFUNCTION(BlueprintCallable, BlueprintInternalUseOnly, meta = (HidePin = "StoryComponent", DefaultToSelf = "StoryComponent", DisplayName = "Story - Two Way Text"), Category = "Story|Nodes")
		static UTwoWayFoldingSkyStoryCallbackProxy* CreateBasicBinaryStoryProxyObject(
			UFoldingSkyStoryComponent* StoryComponent,
			FText StoryText);
	UFUNCTION(BlueprintCallable, BlueprintInternalUseOnly, meta = (HidePin = "StoryComponent", DefaultToSelf = "StoryComponent", DisplayName = "Story - With Choices Text"), Category = "Story|Nodes")
		static UOneChoiceFoldingSkyStoryCallbackProxy* CreateOneChoiceStoryProxyObject(
			UFoldingSkyStoryComponent* StoryComponent,
			FText StoryText,
			FText Choice1);
	UFUNCTION(BlueprintCallable, BlueprintInternalUseOnly, meta = (HidePin = "StoryComponent", DefaultToSelf = "StoryComponent", DisplayName = "Story - With Choices(2) Text"), Category = "Story|Nodes")
		static UTwoChoiceFoldingSkyStoryCallbackProxy* CreateTwoChoiceStoryProxyObject(
			UFoldingSkyStoryComponent* StoryComponent,
			FText StoryText,
			FText Choice1,
			FText Choice2);
	UFUNCTION(BlueprintCallable, BlueprintInternalUseOnly, meta = (HidePin = "StoryComponent", DefaultToSelf = "StoryComponent", DisplayName = "Story - With Choices(3) Text"), Category = "Story|Nodes")
		static UThreeChoiceFoldingSkyStoryCallbackProxy* CreateThreeChoiceStoryProxyObject(
			UFoldingSkyStoryComponent* StoryComponent,
			FText StoryText,
			FText Choice1,
			FText Choice2,
			FText Choice3);
	UFUNCTION(BlueprintCallable, BlueprintInternalUseOnly, meta = (HidePin = "StoryComponent", DefaultToSelf = "StoryComponent", DisplayName = "Story - With Choices(4) Text"), Category = "Story|Nodes")
		static UFourChoiceFoldingSkyStoryCallbackProxy* CreateFourChoiceStoryProxyObject(
			UFoldingSkyStoryComponent* StoryComponent,
			FText StoryText,
			FText Choice1,
			FText Choice2,
			FText Choice3,
			FText Choice4);
	UFUNCTION(BlueprintCallable, BlueprintInternalUseOnly, meta = (HidePin = "StoryComponent", DefaultToSelf = "StoryComponent", DisplayName = "Story - With Choices(5) Text"), Category = "Story|Nodes")
		static UFiveChoiceFoldingSkyStoryCallbackProxy* CreateFiveChoiceStoryProxyObject(
			UFoldingSkyStoryComponent* StoryComponent,
			FText StoryText,
			FText Choice1,
			FText Choice2,
			FText Choice3,
			FText Choice4,
			FText Choice5);
	UFUNCTION(BlueprintCallable, BlueprintInternalUseOnly, meta = (HidePin = "StoryComponent", DefaultToSelf = "StoryComponent", DisplayName = "Story - With Choices(6) Text"), Category = "Story|Nodes")
		static USixChoiceFoldingSkyStoryCallbackProxy* CreateSixChoiceStoryProxyObject(
			UFoldingSkyStoryComponent* StoryComponent,
			FText StoryText,
			FText Choice1,
			FText Choice2,
			FText Choice3,
			FText Choice4,
			FText Choice5,
			FText Choice6);
	UFUNCTION(BlueprintCallable, BlueprintInternalUseOnly, meta = (HidePin = "StoryComponent", DefaultToSelf = "StoryComponent", DisplayName = "Story - With Choices(7) Text"), Category = "Story|Nodes")
		static USevenChoiceFoldingSkyStoryCallbackProxy* CreateSevenChoiceStoryProxyObject(
			UFoldingSkyStoryComponent* StoryComponent,
			FText StoryText,
			FText Choice1,
			FText Choice2,
			FText Choice3,
			FText Choice4,
			FText Choice5,
			FText Choice6,
			FText Choice7);
	UFUNCTION(BlueprintCallable, BlueprintInternalUseOnly, meta = (HidePin = "StoryComponent", DefaultToSelf = "StoryComponent", DisplayName = "Story - With Choices(8) Text"), Category = "Story|Nodes")
		static UEightChoiceFoldingSkyStoryCallbackProxy* CreateEightChoiceStoryProxyObject(
			UFoldingSkyStoryComponent* StoryComponent,
			FText StoryText,
			FText Choice1,
			FText Choice2,
			FText Choice3,
			FText Choice4,
			FText Choice5,
			FText Choice6,
			FText Choice7,
			FText Choice8);
	UFUNCTION(BlueprintCallable, BlueprintInternalUseOnly, meta = (HidePin = "StoryComponent", DefaultToSelf = "StoryComponent", DisplayName = "Story - With Choices(9) Text"), Category = "Story|Nodes")
		static UNineChoiceFoldingSkyStoryCallbackProxy* CreateNineChoiceStoryProxyObject(
			UFoldingSkyStoryComponent* StoryComponent,
			FText StoryText,
			FText Choice1,
			FText Choice2,
			FText Choice3,
			FText Choice4,
			FText Choice5,
			FText Choice6,
			FText Choice7,
			FText Choice8,
			FText Choice9);
	UFUNCTION(BlueprintCallable, BlueprintInternalUseOnly, meta = (HidePin = "StoryComponent", DefaultToSelf = "StoryComponent", DisplayName = "Story - With Choices(10) Text"), Category = "Story|Nodes")
		static UTenChoiceFoldingSkyStoryCallbackProxy* CreateTenChoiceStoryProxyObject(
			UFoldingSkyStoryComponent* StoryComponent,
			FText StoryText,
			FText Choice1,
			FText Choice2,
			FText Choice3,
			FText Choice4,
			FText Choice5,
			FText Choice6,
			FText Choice7,
			FText Choice8,
			FText Choice9,
			FText Choice10);
	// END TEXT FUNCTIONS
	// BEGIN VOICED FUNCTIONS
	template<class T>
	static T* TriggerVoicedStoryNode(UFoldingSkyStoryComponent* StoryComponent, const TSoftObjectPtr<UDialogueWave>& Dia, const TArray<FText>& Choices)
	{
		T* Proxy = NewObject<T>(StoryComponent);
		if (Proxy)
		{
			Proxy->Trigger(StoryComponent, Dia, Choices);
		}
		return Proxy;
	}
	UFUNCTION(BlueprintCallable, BlueprintInternalUseOnly, meta = (HidePin = "StoryComponent", DefaultToSelf = "StoryComponent", DisplayName = "Story - One Way Dialogue"), Category = "Story|Nodes")
		static UOneWayFoldingSkyStoryCallbackProxy* CreateVoicedBasicStoryProxyObject(
			UFoldingSkyStoryComponent* StoryComponent,
			TSoftObjectPtr<UDialogueWave> Dialogue);	
	UFUNCTION(BlueprintCallable, BlueprintInternalUseOnly, meta = (HidePin = "StoryComponent", DefaultToSelf = "StoryComponent", DisplayName = "Story - Two Way Dialogue"), Category = "Story|Nodes")
		static UTwoWayFoldingSkyStoryCallbackProxy* CreateVoicedBinaryStoryProxyObject(
			UFoldingSkyStoryComponent* StoryComponent,
			TSoftObjectPtr<UDialogueWave> Dialogue);	
	UFUNCTION(BlueprintCallable, BlueprintInternalUseOnly, meta = (HidePin = "StoryComponent", DefaultToSelf = "StoryComponent", DisplayName = "Story - With Choices Dialogue"), Category = "Story|Nodes")
		static UOneChoiceFoldingSkyStoryCallbackProxy* CreateVoicedOneChoiceStoryProxyObject(
			UFoldingSkyStoryComponent* StoryComponent,
			TSoftObjectPtr<UDialogueWave> Dialogue,
			FText Choice1);	
	UFUNCTION(BlueprintCallable, BlueprintInternalUseOnly, meta = (HidePin = "StoryComponent", DefaultToSelf = "StoryComponent", DisplayName = "Story - With Choices(2) Dialogue"), Category = "Story|Nodes")
		static UTwoChoiceFoldingSkyStoryCallbackProxy* CreateVoicedTwoChoiceStoryProxyObject(
			UFoldingSkyStoryComponent* StoryComponent,
			TSoftObjectPtr<UDialogueWave> Dialogue,
			FText Choice1,
			FText Choice2);	
	UFUNCTION(BlueprintCallable, BlueprintInternalUseOnly, meta = (HidePin = "StoryComponent", DefaultToSelf = "StoryComponent", DisplayName = "Story - With Choices(3) Dialogue"), Category = "Story|Nodes")
		static UThreeChoiceFoldingSkyStoryCallbackProxy* CreateVoicedThreeChoiceStoryProxyObject(
			UFoldingSkyStoryComponent* StoryComponent,
			TSoftObjectPtr<UDialogueWave> Dialogue,
			FText Choice1,
			FText Choice2,
			FText Choice3);	
	UFUNCTION(BlueprintCallable, BlueprintInternalUseOnly, meta = (HidePin = "StoryComponent", DefaultToSelf = "StoryComponent", DisplayName = "Story - With Choices(4) Dialogue"), Category = "Story|Nodes")
		static UFourChoiceFoldingSkyStoryCallbackProxy* CreateVoicedFourChoiceStoryProxyObject(
			UFoldingSkyStoryComponent* StoryComponent,
			TSoftObjectPtr<UDialogueWave> Dialogue,
			FText Choice1,
			FText Choice2,
			FText Choice3,
			FText Choice4);	
	UFUNCTION(BlueprintCallable, BlueprintInternalUseOnly, meta = (HidePin = "StoryComponent", DefaultToSelf = "StoryComponent", DisplayName = "Story - With Choices(5) Dialogue"), Category = "Story|Nodes")
		static UFiveChoiceFoldingSkyStoryCallbackProxy* CreateVoicedFiveChoiceStoryProxyObject(
			UFoldingSkyStoryComponent* StoryComponent,
			TSoftObjectPtr<UDialogueWave> Dialogue,
			FText Choice1,
			FText Choice2,
			FText Choice3,
			FText Choice4,
			FText Choice5);	
	UFUNCTION(BlueprintCallable, BlueprintInternalUseOnly, meta = (HidePin = "StoryComponent", DefaultToSelf = "StoryComponent", DisplayName = "Story - With Choices(6) Dialogue"), Category = "Story|Nodes")
		static USixChoiceFoldingSkyStoryCallbackProxy* CreateVoicedSixChoiceStoryProxyObject(
			UFoldingSkyStoryComponent* StoryComponent,
			TSoftObjectPtr<UDialogueWave> Dialogue,
			FText Choice1,
			FText Choice2,
			FText Choice3,
			FText Choice4,
			FText Choice5,
			FText Choice6);
	UFUNCTION(BlueprintCallable, BlueprintInternalUseOnly, meta = (HidePin = "StoryComponent", DefaultToSelf = "StoryComponent", DisplayName = "Story - With Choices(7) Dialogue"), Category = "Story|Nodes")
		static USevenChoiceFoldingSkyStoryCallbackProxy* CreateVoicedSevenChoiceStoryProxyObject(
			UFoldingSkyStoryComponent* StoryComponent,
			TSoftObjectPtr<UDialogueWave> Dialogue,
			FText Choice1,
			FText Choice2,
			FText Choice3,
			FText Choice4,
			FText Choice5,
			FText Choice6,
			FText Choice7);
	UFUNCTION(BlueprintCallable, BlueprintInternalUseOnly, meta = (HidePin = "StoryComponent", DefaultToSelf = "StoryComponent", DisplayName = "Story - With Choices(8) Dialogue"), Category = "Story|Nodes")
		static UEightChoiceFoldingSkyStoryCallbackProxy* CreateVoicedEightChoiceStoryProxyObject(
			UFoldingSkyStoryComponent* StoryComponent,
			TSoftObjectPtr<UDialogueWave> Dialogue,
			FText Choice1,
			FText Choice2,
			FText Choice3,
			FText Choice4,
			FText Choice5,
			FText Choice6,
			FText Choice7,
			FText Choice8);
	UFUNCTION(BlueprintCallable, BlueprintInternalUseOnly, meta = (HidePin = "StoryComponent", DefaultToSelf = "StoryComponent", DisplayName = "Story - With Choices(9) Dialogue"), Category = "Story|Nodes")
		static UNineChoiceFoldingSkyStoryCallbackProxy* CreateVoicedNineChoiceStoryProxyObject(
			UFoldingSkyStoryComponent* StoryComponent,
			TSoftObjectPtr<UDialogueWave> Dialogue,
			FText Choice1,
			FText Choice2,
			FText Choice3,
			FText Choice4,
			FText Choice5,
			FText Choice6,
			FText Choice7,
			FText Choice8,
			FText Choice9);
	UFUNCTION(BlueprintCallable, BlueprintInternalUseOnly, meta = (HidePin = "StoryComponent", DefaultToSelf = "StoryComponent", DisplayName = "Story - With Choices(10) Dialogue"), Category = "Story|Nodes")
		static UTenChoiceFoldingSkyStoryCallbackProxy* CreateVoicedTenChoiceStoryProxyObject(
			UFoldingSkyStoryComponent* StoryComponent,
			TSoftObjectPtr<UDialogueWave> Dialogue,
			FText Choice1,
			FText Choice2,
			FText Choice3,
			FText Choice4,
			FText Choice5,
			FText Choice6,
			FText Choice7,
			FText Choice8,
			FText Choice9,
			FText Choice10);
	// END VOICED FUNCTIONS
	FOLDINGSKYSTORY_API static void GetClassAndFunctionName(const EFoldingSkyStoryNodeType& Type, const int32& Index, UClass*& OutClass, FName& OutFunctionName);
protected:
		static void GetProxyClassByIndex(const int32& Index, UClass*& OutClass);
		static void GetFunctionByIndex(const EFoldingSkyStoryNodeType& Type, const int32& Index, FName& OutFunctionName);
#pragma region CUSTOM
public:
	template<class T>
	static T* TriggerCustomStoryNode(UFoldingSkyStoryComponent* StoryComponent, const FString& CustomJson, const TArray<FText>& Choices)
	{
		T* Proxy = NewObject<T>(StoryComponent);
		if (Proxy)
		{
			Proxy->Trigger(StoryComponent, CustomJson, Choices);
		}
		return Proxy;
	}
	UFUNCTION(BlueprintCallable, BlueprintInternalUseOnly, meta = (CustomStructureParam = "CustomStruct", HidePin = "StoryComponent", DefaultToSelf = "StoryComponent", DisplayName = "Story - Basic Custom"), Category = "Story|Nodes")
		static UOneWayFoldingSkyStoryCallbackProxy* CreateCustomBasicStoryProxyObject(
			UFoldingSkyStoryComponent* StoryComponent,
			UStruct* CustomStruct);
	UFUNCTION(BlueprintCallable, BlueprintInternalUseOnly, meta = (CustomStructureParam = "CustomStruct", HidePin = "StoryComponent", DefaultToSelf = "StoryComponent", DisplayName = "Story - Two Way Custom"), Category = "Story|Nodes")
		static UTwoWayFoldingSkyStoryCallbackProxy* CreateCustomBinaryStoryProxyObject(
			UFoldingSkyStoryComponent* StoryComponent,
			UStruct* CustomStruct);
	UFUNCTION(BlueprintCallable, BlueprintInternalUseOnly, meta = (CustomStructureParam = "CustomStruct", HidePin = "StoryComponent", DefaultToSelf = "StoryComponent", DisplayName = "Story - With Choices Custom"), Category = "Story|Nodes")
		static UOneChoiceFoldingSkyStoryCallbackProxy* CreateCustomOneChoiceStoryProxyObject(
			UFoldingSkyStoryComponent* StoryComponent,
			UStruct* CustomStruct,
			FText Choice1);
	UFUNCTION(BlueprintCallable, BlueprintInternalUseOnly, meta = (CustomStructureParam = "CustomStruct", HidePin = "StoryComponent", DefaultToSelf = "StoryComponent", DisplayName = "Story - With Choices(2) Custom"), Category = "Story|Nodes")
		static USixChoiceFoldingSkyStoryCallbackProxy* CreateCustomTwoChoiceStoryProxyObject(
			UFoldingSkyStoryComponent* StoryComponent,
			UStruct* CustomStruct,
			FText Choice1,
			FText Choice2);
	UFUNCTION(BlueprintCallable, BlueprintInternalUseOnly, meta = (CustomStructureParam = "CustomStruct", HidePin = "StoryComponent", DefaultToSelf = "StoryComponent", DisplayName = "Story - With Choices(3) Custom"), Category = "Story|Nodes")
		static USixChoiceFoldingSkyStoryCallbackProxy* CreateCustomThreeChoiceStoryProxyObject(
			UFoldingSkyStoryComponent* StoryComponent,
			UStruct* CustomStruct,
			FText Choice1,
			FText Choice2,
			FText Choice3);
	UFUNCTION(BlueprintCallable, BlueprintInternalUseOnly, meta = (CustomStructureParam = "CustomStruct", HidePin = "StoryComponent", DefaultToSelf = "StoryComponent", DisplayName = "Story - With Choices(4) Custom"), Category = "Story|Nodes")
		static USixChoiceFoldingSkyStoryCallbackProxy* CreateCustomFourChoiceStoryProxyObject(
			UFoldingSkyStoryComponent* StoryComponent,
			UStruct* CustomStruct,
			FText Choice1,
			FText Choice2,
			FText Choice3,
			FText Choice4);
	UFUNCTION(BlueprintCallable, BlueprintInternalUseOnly, meta = (CustomStructureParam = "CustomStruct", HidePin = "StoryComponent", DefaultToSelf = "StoryComponent", DisplayName = "Story - With Choices(5) Custom"), Category = "Story|Nodes")
		static USixChoiceFoldingSkyStoryCallbackProxy* CreateCustomFiveChoiceStoryProxyObject(
			UFoldingSkyStoryComponent* StoryComponent,
			UStruct* CustomStruct,
			FText Choice1,
			FText Choice2,
			FText Choice3,
			FText Choice4,
			FText Choice5);
	UFUNCTION(BlueprintCallable, BlueprintInternalUseOnly, meta = (CustomStructureParam = "CustomStruct", HidePin = "StoryComponent", DefaultToSelf = "StoryComponent", DisplayName = "Story - With Choices(6) Custom"), Category = "Story|Nodes")
		static USixChoiceFoldingSkyStoryCallbackProxy* CreateCustomSixChoiceStoryProxyObject(
			UFoldingSkyStoryComponent* StoryComponent,
			UStruct* CustomStruct,
			FText Choice1,
			FText Choice2,
			FText Choice3,
			FText Choice4,
			FText Choice5,
			FText Choice6);

protected:
	static FString CreateCustomStoryProxyJson(UStruct* CustomStruct);
#pragma endregion	
};