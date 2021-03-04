// Copyright Folding Sky Games LLC 2021 All rights reserved.

#include "FoldingSkyStoryCallbackProxy.h"
#include "FoldingSkyStoryComponent.h"
#include "JsonObjectConverter.h"

DECLARE_LOG_CATEGORY_CLASS(LogStoryCallbackProxy, Log, All);


UWorld* UBaseFoldingSkyStoryCallbackProxy::GetWorld() const
{
	return GetOuter() ? GetOuter()->GetWorld() : nullptr;
}
void UBaseFoldingSkyStoryCallbackProxy::BeginDestroy()
{	
	UE_LOG(LogStoryCallbackProxy, Log, TEXT("%s getting collected"), *GetName());
	Super::BeginDestroy();
}
TArray<FOnStoryOptionChosen*> UOneWayFoldingSkyStoryCallbackProxy::GetStoryOptionPins()
{
	return TArray<FOnStoryOptionChosen*> { &OnWayOne };
}
TArray<FOnStoryOptionChosen*>  UTwoWayFoldingSkyStoryCallbackProxy::GetStoryOptionPins()
{
	return TArray<FOnStoryOptionChosen*> { &OnWayOne, &OnWayTwo };
}
TArray<FOnStoryOptionChosen*> UOneChoiceFoldingSkyStoryCallbackProxy::GetStoryOptionPins()
{
	return TArray<FOnStoryOptionChosen*>{ &OnChoice1 };
}
TArray<FOnStoryOptionChosen*> UTwoChoiceFoldingSkyStoryCallbackProxy::GetStoryOptionPins()
{
	return TArray<FOnStoryOptionChosen*>{ &OnChoice1, &OnChoice2 };
}
TArray<FOnStoryOptionChosen*> UThreeChoiceFoldingSkyStoryCallbackProxy::GetStoryOptionPins()
{
	return TArray<FOnStoryOptionChosen*>{ &OnChoice1, &OnChoice2, &OnChoice3 };
}
TArray<FOnStoryOptionChosen*> UFourChoiceFoldingSkyStoryCallbackProxy::GetStoryOptionPins()
{
	return TArray<FOnStoryOptionChosen*>{ &OnChoice1, &OnChoice2, &OnChoice3, &OnChoice4 };
}
TArray<FOnStoryOptionChosen*> UFiveChoiceFoldingSkyStoryCallbackProxy::GetStoryOptionPins()
{
	return TArray<FOnStoryOptionChosen*>{ &OnChoice1, &OnChoice2, &OnChoice3, &OnChoice4, &OnChoice5 };
}
TArray<FOnStoryOptionChosen*> USixChoiceFoldingSkyStoryCallbackProxy::GetStoryOptionPins()
{
	return TArray<FOnStoryOptionChosen*>{ &OnChoice1, &OnChoice2, &OnChoice3, &OnChoice4, &OnChoice5, &OnChoice6 };
}
TArray<FOnStoryOptionChosen*> USevenChoiceFoldingSkyStoryCallbackProxy::GetStoryOptionPins()
{
	return TArray<FOnStoryOptionChosen*>{ &OnChoice1, &OnChoice2, &OnChoice3, &OnChoice4, &OnChoice5, &OnChoice6, &OnChoice7 };
}
TArray<FOnStoryOptionChosen*> UEightChoiceFoldingSkyStoryCallbackProxy::GetStoryOptionPins()
{
	return TArray<FOnStoryOptionChosen*>{ &OnChoice1, & OnChoice2, &OnChoice3, &OnChoice4, &OnChoice5, &OnChoice6, &OnChoice7, &OnChoice8 };
}
TArray<FOnStoryOptionChosen*> UNineChoiceFoldingSkyStoryCallbackProxy::GetStoryOptionPins()
{
	return TArray<FOnStoryOptionChosen*>{ &OnChoice1, &OnChoice2, &OnChoice3, &OnChoice4, &OnChoice5, &OnChoice6, & OnChoice7, &OnChoice8, &OnChoice9 };
}
TArray<FOnStoryOptionChosen*> UTenChoiceFoldingSkyStoryCallbackProxy::GetStoryOptionPins()
{
	return TArray<FOnStoryOptionChosen*>{ &OnChoice1, &OnChoice2, &OnChoice3, &OnChoice4, &OnChoice5, &OnChoice6, &OnChoice7, &OnChoice8, &OnChoice9, &OnChoice10 };
}

void UBaseFoldingSkyStoryCallbackProxy::Trigger(UFoldingSkyStoryComponent* StoryComponent, const FText& Text, const TArray<FText>& Choices)
{
	FOnStoryChoiceMade Callback;
	Callback.BindDynamic(this, &UBaseFoldingSkyStoryCallbackProxy::OnOptionChosen);
	const FFoldingSkyStoryNodeParams& NodeParams = FFoldingSkyStoryNodeParams(Callback, Text, Choices);
	StoryComponent->SendStory_NodeInternalUseOnly(NodeParams);
}
void UBaseFoldingSkyStoryCallbackProxy::Trigger(UFoldingSkyStoryComponent* StoryComponent, const TSoftObjectPtr<UDialogueWave>& Dialogue, const TArray<FText>& Choices)
{
	FOnStoryChoiceMade Callback;
	Callback.BindDynamic(this, &UBaseFoldingSkyStoryCallbackProxy::OnOptionChosen);
	const FFoldingSkyStoryNodeParams& NodeParams = FFoldingSkyStoryNodeParams(Callback, Dialogue, Choices);
	StoryComponent->SendStory_NodeInternalUseOnly(NodeParams);
}
void UBaseFoldingSkyStoryCallbackProxy::Trigger(UFoldingSkyStoryComponent* StoryComponent,
	const FString& CustomData,
	const TArray<FText>& Choices)
{
	FOnStoryChoiceMade Callback;
	Callback.BindDynamic(this, &UBaseFoldingSkyStoryCallbackProxy::OnOptionChosen);
	//const FFoldingSkyStoryNodeParams& NodeParams = FFoldingSkyStoryNodeParams(Callback, CustomData, Choices);
	//StoryComponent->SendStory_NodeInternalUseOnly(NodeParams);
}

void UBaseFoldingSkyStoryCallbackProxy::OnOptionChosen(int32 Option)
{
	const TArray<FOnStoryOptionChosen*>& StoryOptionPins = GetStoryOptionPins();
	if (StoryOptionPins.IsValidIndex(Option))
	{
		StoryOptionPins[Option]->Broadcast();
		OnComplete.Broadcast();
	}
	else if (Option == -1)
	{
		OnCancel.Broadcast();
		OnComplete.Broadcast();
	}
	else
	{
		UE_LOG(LogStoryCallbackProxy, Error, TEXT("The option %d does not refer to a valid option pin on the blueprint node."), Option);
	}
}


UOneWayFoldingSkyStoryCallbackProxy* UFoldingSkyStoryCallbackProxyFactory::CreateBasicStoryProxyObject(
	UFoldingSkyStoryComponent* StoryComponent,
	FText Text)
{
	const TArray<FText>& Choices = StoryComponent->GetBasicChoices();
	return TriggerTextStoryNode<UOneWayFoldingSkyStoryCallbackProxy>(StoryComponent, Text, Choices);
}
UTwoWayFoldingSkyStoryCallbackProxy* UFoldingSkyStoryCallbackProxyFactory::CreateBasicBinaryStoryProxyObject(
	UFoldingSkyStoryComponent* StoryComponent,
	FText StoryText)
{
	const TArray<FText>& Choices = StoryComponent->GetBinaryChoices();
	return TriggerTextStoryNode<UTwoWayFoldingSkyStoryCallbackProxy>(StoryComponent, StoryText, Choices);
}
UOneChoiceFoldingSkyStoryCallbackProxy* UFoldingSkyStoryCallbackProxyFactory::CreateOneChoiceStoryProxyObject(
	UFoldingSkyStoryComponent* StoryComponent,
	FText StoryText,
	FText Choice1)
{
	const TArray<FText>& Choices = TArray<FText>{ Choice1 };
	return TriggerTextStoryNode<UOneChoiceFoldingSkyStoryCallbackProxy>(StoryComponent, StoryText, Choices);
}
UTwoChoiceFoldingSkyStoryCallbackProxy* UFoldingSkyStoryCallbackProxyFactory::CreateTwoChoiceStoryProxyObject(
	UFoldingSkyStoryComponent* StoryComponent,
	FText StoryText,
	FText Choice1, FText Choice2)
{
	const TArray<FText>& Choices = TArray<FText>{ Choice1, Choice2 };
	return TriggerTextStoryNode<UTwoChoiceFoldingSkyStoryCallbackProxy>(StoryComponent, StoryText, Choices);
}
UThreeChoiceFoldingSkyStoryCallbackProxy* UFoldingSkyStoryCallbackProxyFactory::CreateThreeChoiceStoryProxyObject(
	UFoldingSkyStoryComponent* StoryComponent,
	FText StoryText,
	FText Choice1, FText Choice2, FText Choice3)
{
	const TArray<FText>& Choices = TArray<FText>{ Choice1, Choice2, Choice3 };
	return TriggerTextStoryNode<UThreeChoiceFoldingSkyStoryCallbackProxy>(StoryComponent, StoryText, Choices);
}
UFourChoiceFoldingSkyStoryCallbackProxy* UFoldingSkyStoryCallbackProxyFactory::CreateFourChoiceStoryProxyObject(
	UFoldingSkyStoryComponent* StoryComponent,
	FText StoryText,
	FText Choice1, FText Choice2, FText Choice3, FText Choice4)
{
	const TArray<FText>& Choices = TArray<FText>{ Choice1, Choice2, Choice3, Choice4 };
	return TriggerTextStoryNode<UFourChoiceFoldingSkyStoryCallbackProxy>(StoryComponent, StoryText, Choices);
}
UFiveChoiceFoldingSkyStoryCallbackProxy* UFoldingSkyStoryCallbackProxyFactory::CreateFiveChoiceStoryProxyObject(
	UFoldingSkyStoryComponent* StoryComponent,
	FText StoryText,
	FText Choice1, FText Choice2, FText Choice3, FText Choice4, FText Choice5)
{
	const TArray<FText>& Choices = TArray<FText>{ Choice1, Choice2, Choice3, Choice4, Choice5 };
	return TriggerTextStoryNode<UFiveChoiceFoldingSkyStoryCallbackProxy>(StoryComponent, StoryText, Choices);
}
USixChoiceFoldingSkyStoryCallbackProxy* UFoldingSkyStoryCallbackProxyFactory::CreateSixChoiceStoryProxyObject(
	UFoldingSkyStoryComponent* StoryComponent,
	FText StoryText,
	FText Choice1, FText Choice2, FText Choice3, FText Choice4, FText Choice5, FText Choice6)
{
	const TArray<FText>& Choices = TArray<FText>{ Choice1, Choice2, Choice3, Choice4, Choice5, Choice6 };
	return TriggerTextStoryNode<USixChoiceFoldingSkyStoryCallbackProxy>(StoryComponent, StoryText, Choices);
}
USevenChoiceFoldingSkyStoryCallbackProxy* UFoldingSkyStoryCallbackProxyFactory::CreateSevenChoiceStoryProxyObject(
	UFoldingSkyStoryComponent* StoryComponent,
	FText StoryText,
	FText Choice1, FText Choice2, FText Choice3, FText Choice4, FText Choice5, FText Choice6, FText Choice7)
{
	const TArray<FText>& Choices = TArray<FText>{ Choice1, Choice2, Choice3, Choice4, Choice5, Choice6, Choice7 };
	return TriggerTextStoryNode<USevenChoiceFoldingSkyStoryCallbackProxy>(StoryComponent, StoryText, Choices);
}
UEightChoiceFoldingSkyStoryCallbackProxy* UFoldingSkyStoryCallbackProxyFactory::CreateEightChoiceStoryProxyObject(
	UFoldingSkyStoryComponent* StoryComponent,
	FText StoryText,
	FText Choice1, FText Choice2, FText Choice3, FText Choice4, FText Choice5, FText Choice6, FText Choice7, FText Choice8)
{
	const TArray<FText>& Choices = TArray<FText>{ Choice1, Choice2, Choice3, Choice4, Choice5, Choice6, Choice7, Choice8 };
	return TriggerTextStoryNode<UEightChoiceFoldingSkyStoryCallbackProxy>(StoryComponent, StoryText, Choices);
}
UNineChoiceFoldingSkyStoryCallbackProxy* UFoldingSkyStoryCallbackProxyFactory::CreateNineChoiceStoryProxyObject(
	UFoldingSkyStoryComponent* StoryComponent,
	FText StoryText,
	FText Choice1, FText Choice2, FText Choice3, FText Choice4, FText Choice5, FText Choice6, FText Choice7, FText Choice8, FText Choice9)
{
	const TArray<FText>& Choices = TArray<FText>{ Choice1, Choice2, Choice3, Choice4, Choice5, Choice6, Choice7, Choice8, Choice9 };
	return TriggerTextStoryNode<UNineChoiceFoldingSkyStoryCallbackProxy>(StoryComponent, StoryText, Choices);
}
UTenChoiceFoldingSkyStoryCallbackProxy* UFoldingSkyStoryCallbackProxyFactory::CreateTenChoiceStoryProxyObject(
	UFoldingSkyStoryComponent* StoryComponent,
	FText StoryText,
	FText Choice1, FText Choice2, FText Choice3, FText Choice4, FText Choice5, FText Choice6, FText Choice7, FText Choice8, FText Choice9, FText Choice10)
{
	const TArray<FText>& Choices = TArray<FText>{ Choice1, Choice2, Choice3, Choice4, Choice5, Choice6, Choice7, Choice8, Choice9, Choice10 };
	return TriggerTextStoryNode<UTenChoiceFoldingSkyStoryCallbackProxy>(StoryComponent, StoryText, Choices);
}

UOneWayFoldingSkyStoryCallbackProxy* UFoldingSkyStoryCallbackProxyFactory::CreateVoicedBasicStoryProxyObject(
	UFoldingSkyStoryComponent* StoryComponent,
	TSoftObjectPtr<UDialogueWave> Dia)
{
	const TArray<FText>& Choices = StoryComponent->GetBasicChoices();
	return TriggerVoicedStoryNode<UOneWayFoldingSkyStoryCallbackProxy>(StoryComponent, Dia, Choices);
}
UTwoWayFoldingSkyStoryCallbackProxy* UFoldingSkyStoryCallbackProxyFactory::CreateVoicedBinaryStoryProxyObject(
	UFoldingSkyStoryComponent* StoryComponent,
	TSoftObjectPtr<UDialogueWave> Dialogue)
{
	const TArray<FText>& Choices = StoryComponent->GetBinaryChoices();
	return TriggerVoicedStoryNode<UTwoWayFoldingSkyStoryCallbackProxy>(StoryComponent, Dialogue, Choices);
}
UOneChoiceFoldingSkyStoryCallbackProxy* UFoldingSkyStoryCallbackProxyFactory::CreateVoicedOneChoiceStoryProxyObject(
	UFoldingSkyStoryComponent* StoryComponent,
	TSoftObjectPtr<UDialogueWave> Dialogue,
	FText Choice1)
{
	const TArray<FText>& Choices = TArray<FText>{ Choice1 };
	return TriggerVoicedStoryNode<UOneChoiceFoldingSkyStoryCallbackProxy>(StoryComponent, Dialogue, Choices);
}
UTwoChoiceFoldingSkyStoryCallbackProxy* UFoldingSkyStoryCallbackProxyFactory::CreateVoicedTwoChoiceStoryProxyObject(
	UFoldingSkyStoryComponent* StoryComponent,
	TSoftObjectPtr<UDialogueWave> Dialogue,
	FText Choice1, FText Choice2)
{
	const TArray<FText>& Choices = TArray<FText>{ Choice1, Choice2 };
	return TriggerVoicedStoryNode<UTwoChoiceFoldingSkyStoryCallbackProxy>(StoryComponent, Dialogue, Choices);
}
UThreeChoiceFoldingSkyStoryCallbackProxy* UFoldingSkyStoryCallbackProxyFactory::CreateVoicedThreeChoiceStoryProxyObject(
	UFoldingSkyStoryComponent * StoryComponent,
	TSoftObjectPtr<UDialogueWave> Dialogue,
	FText Choice1, FText Choice2, FText Choice3)
{
	const TArray<FText>& Choices = TArray<FText>{ Choice1, Choice2, Choice3 };
	return TriggerVoicedStoryNode<UThreeChoiceFoldingSkyStoryCallbackProxy>(StoryComponent, Dialogue, Choices);
}
UFourChoiceFoldingSkyStoryCallbackProxy* UFoldingSkyStoryCallbackProxyFactory::CreateVoicedFourChoiceStoryProxyObject(
	UFoldingSkyStoryComponent* StoryComponent,
	TSoftObjectPtr<UDialogueWave> Dialogue,
	FText Choice1, FText Choice2, FText Choice3, FText Choice4)
{
	const TArray<FText>& Choices = TArray<FText>{ Choice1, Choice2, Choice3, Choice4 };
	return TriggerVoicedStoryNode<UFourChoiceFoldingSkyStoryCallbackProxy>(StoryComponent, Dialogue, Choices);
}
UFiveChoiceFoldingSkyStoryCallbackProxy* UFoldingSkyStoryCallbackProxyFactory::CreateVoicedFiveChoiceStoryProxyObject(
	UFoldingSkyStoryComponent* StoryComponent,
	TSoftObjectPtr<UDialogueWave> Dialogue,
	FText Choice1, FText Choice2, FText Choice3, FText Choice4, FText Choice5)
{
	const TArray<FText>& Choices = TArray<FText>{ Choice1, Choice2, Choice3, Choice4, Choice5 };
	return TriggerVoicedStoryNode<UFiveChoiceFoldingSkyStoryCallbackProxy>(StoryComponent, Dialogue, Choices);
}
USixChoiceFoldingSkyStoryCallbackProxy* UFoldingSkyStoryCallbackProxyFactory::CreateVoicedSixChoiceStoryProxyObject(
	UFoldingSkyStoryComponent* StoryComponent,
	TSoftObjectPtr<UDialogueWave> Dialogue,
	FText Choice1, FText Choice2, FText Choice3, FText Choice4, FText Choice5, FText Choice6)
{
	const TArray<FText>& Choices = TArray<FText>{ Choice1, Choice2, Choice3, Choice4, Choice5, Choice6 };
	return TriggerVoicedStoryNode<USixChoiceFoldingSkyStoryCallbackProxy>(StoryComponent, Dialogue, Choices);
}
USevenChoiceFoldingSkyStoryCallbackProxy* UFoldingSkyStoryCallbackProxyFactory::CreateVoicedSevenChoiceStoryProxyObject(
	UFoldingSkyStoryComponent* StoryComponent,
	TSoftObjectPtr<UDialogueWave> Dialogue,
	FText Choice1, FText Choice2, FText Choice3, FText Choice4, FText Choice5, FText Choice6, FText Choice7)
{
	const TArray<FText>& Choices = TArray<FText>{ Choice1, Choice2, Choice3, Choice4, Choice5, Choice6, Choice7 };
	return TriggerVoicedStoryNode<USevenChoiceFoldingSkyStoryCallbackProxy>(StoryComponent, Dialogue, Choices);
}
UEightChoiceFoldingSkyStoryCallbackProxy* UFoldingSkyStoryCallbackProxyFactory::CreateVoicedEightChoiceStoryProxyObject(
	UFoldingSkyStoryComponent* StoryComponent,
	TSoftObjectPtr<UDialogueWave> Dialogue,
	FText Choice1, FText Choice2, FText Choice3, FText Choice4, FText Choice5, FText Choice6, FText Choice7, FText Choice8)
{
	const TArray<FText>& Choices = TArray<FText>{ Choice1, Choice2, Choice3, Choice4, Choice5, Choice6, Choice7, Choice8 };
	return TriggerVoicedStoryNode<UEightChoiceFoldingSkyStoryCallbackProxy>(StoryComponent, Dialogue, Choices);
}
UNineChoiceFoldingSkyStoryCallbackProxy* UFoldingSkyStoryCallbackProxyFactory::CreateVoicedNineChoiceStoryProxyObject(
	UFoldingSkyStoryComponent* StoryComponent,
	TSoftObjectPtr<UDialogueWave> Dialogue,
	FText Choice1, FText Choice2, FText Choice3, FText Choice4, FText Choice5, FText Choice6, FText Choice7, FText Choice8, FText Choice9)
{
	const TArray<FText>& Choices = TArray<FText>{ Choice1, Choice2, Choice3, Choice4, Choice5, Choice6, Choice7, Choice8, Choice9 };
	return TriggerVoicedStoryNode<UNineChoiceFoldingSkyStoryCallbackProxy>(StoryComponent, Dialogue, Choices);
}
UTenChoiceFoldingSkyStoryCallbackProxy* UFoldingSkyStoryCallbackProxyFactory::CreateVoicedTenChoiceStoryProxyObject(
	UFoldingSkyStoryComponent* StoryComponent,
	TSoftObjectPtr<UDialogueWave> Dialogue,
	FText Choice1, FText Choice2, FText Choice3, FText Choice4, FText Choice5, FText Choice6, FText Choice7, FText Choice8, FText Choice9, FText Choice10)
{
	const TArray<FText>& Choices = TArray<FText>{ Choice1, Choice2, Choice3, Choice4, Choice5, Choice6, Choice7, Choice8, Choice9, Choice10 };
	return TriggerVoicedStoryNode<UTenChoiceFoldingSkyStoryCallbackProxy>(StoryComponent, Dialogue, Choices);
}


void UFoldingSkyStoryCallbackProxyFactory::GetProxyClassByIndex(const int32& Index, UClass*& OutClass)
{	
	const TArray<UClass*>& Classes = TArray<UClass*>{
		UTwoWayFoldingSkyStoryCallbackProxy::StaticClass(),
		UOneWayFoldingSkyStoryCallbackProxy::StaticClass(),
		UOneChoiceFoldingSkyStoryCallbackProxy::StaticClass(),
		UTwoChoiceFoldingSkyStoryCallbackProxy::StaticClass(),
		UThreeChoiceFoldingSkyStoryCallbackProxy::StaticClass(),
		UFourChoiceFoldingSkyStoryCallbackProxy::StaticClass(),
		UFiveChoiceFoldingSkyStoryCallbackProxy::StaticClass(),
		USixChoiceFoldingSkyStoryCallbackProxy::StaticClass(),
		USevenChoiceFoldingSkyStoryCallbackProxy::StaticClass(),
		UEightChoiceFoldingSkyStoryCallbackProxy::StaticClass(),
		UNineChoiceFoldingSkyStoryCallbackProxy::StaticClass(),
		UTenChoiceFoldingSkyStoryCallbackProxy::StaticClass()
	};
	if (Classes.IsValidIndex(Index))
	{
		OutClass = Classes[Index];
	}
	else
	{
		OutClass = UOneChoiceFoldingSkyStoryCallbackProxy::StaticClass();
	}
}

void UFoldingSkyStoryCallbackProxyFactory::GetClassAndFunctionName(const EFoldingSkyStoryNodeType& Type, const int32& Index, UClass*& OutClass, FName& OutFunctionName)
{
	int32 ModifiedIndex = Index + 2; // nasty hack to pack more into a simpler api
	GetProxyClassByIndex(ModifiedIndex, OutClass);
	GetFunctionByIndex(Type, ModifiedIndex, OutFunctionName);
}
void UFoldingSkyStoryCallbackProxyFactory::GetFunctionByIndex(const EFoldingSkyStoryNodeType& Type, const int32& Index, FName& OutFunctionName)
{
	TArray<FName> Names;
	switch (Type)
	{
	case EFoldingSkyStoryNodeType::DialogueNode:
		Names = TArray<FName>{
			GET_FUNCTION_NAME_CHECKED(UFoldingSkyStoryCallbackProxyFactory, CreateVoicedBinaryStoryProxyObject),
			GET_FUNCTION_NAME_CHECKED(UFoldingSkyStoryCallbackProxyFactory, CreateVoicedBasicStoryProxyObject),
			GET_FUNCTION_NAME_CHECKED(UFoldingSkyStoryCallbackProxyFactory, CreateVoicedOneChoiceStoryProxyObject),
			GET_FUNCTION_NAME_CHECKED(UFoldingSkyStoryCallbackProxyFactory, CreateVoicedTwoChoiceStoryProxyObject),
			GET_FUNCTION_NAME_CHECKED(UFoldingSkyStoryCallbackProxyFactory, CreateVoicedThreeChoiceStoryProxyObject),
			GET_FUNCTION_NAME_CHECKED(UFoldingSkyStoryCallbackProxyFactory, CreateVoicedFourChoiceStoryProxyObject),
			GET_FUNCTION_NAME_CHECKED(UFoldingSkyStoryCallbackProxyFactory, CreateVoicedFiveChoiceStoryProxyObject),
			GET_FUNCTION_NAME_CHECKED(UFoldingSkyStoryCallbackProxyFactory, CreateVoicedSixChoiceStoryProxyObject),
			GET_FUNCTION_NAME_CHECKED(UFoldingSkyStoryCallbackProxyFactory, CreateVoicedSevenChoiceStoryProxyObject),
			GET_FUNCTION_NAME_CHECKED(UFoldingSkyStoryCallbackProxyFactory, CreateVoicedEightChoiceStoryProxyObject),
			GET_FUNCTION_NAME_CHECKED(UFoldingSkyStoryCallbackProxyFactory, CreateVoicedNineChoiceStoryProxyObject),
			GET_FUNCTION_NAME_CHECKED(UFoldingSkyStoryCallbackProxyFactory, CreateVoicedTenChoiceStoryProxyObject)
		};
		break;
	case EFoldingSkyStoryNodeType::CustomNode:
		Names = TArray<FName>{
			GET_FUNCTION_NAME_CHECKED(UFoldingSkyStoryCallbackProxyFactory, CreateCustomBinaryStoryProxyObject),
			GET_FUNCTION_NAME_CHECKED(UFoldingSkyStoryCallbackProxyFactory, CreateCustomBasicStoryProxyObject),
			GET_FUNCTION_NAME_CHECKED(UFoldingSkyStoryCallbackProxyFactory, CreateCustomOneChoiceStoryProxyObject),
			GET_FUNCTION_NAME_CHECKED(UFoldingSkyStoryCallbackProxyFactory, CreateCustomTwoChoiceStoryProxyObject),
			GET_FUNCTION_NAME_CHECKED(UFoldingSkyStoryCallbackProxyFactory, CreateCustomThreeChoiceStoryProxyObject),
			GET_FUNCTION_NAME_CHECKED(UFoldingSkyStoryCallbackProxyFactory, CreateCustomFourChoiceStoryProxyObject),
			GET_FUNCTION_NAME_CHECKED(UFoldingSkyStoryCallbackProxyFactory, CreateCustomFiveChoiceStoryProxyObject),
			GET_FUNCTION_NAME_CHECKED(UFoldingSkyStoryCallbackProxyFactory, CreateCustomSixChoiceStoryProxyObject)
		}; 
		break;
	case EFoldingSkyStoryNodeType::TextNode:
	default:
		Names = TArray<FName>{
			GET_FUNCTION_NAME_CHECKED(UFoldingSkyStoryCallbackProxyFactory, CreateBasicBinaryStoryProxyObject),
			GET_FUNCTION_NAME_CHECKED(UFoldingSkyStoryCallbackProxyFactory, CreateBasicStoryProxyObject),
			GET_FUNCTION_NAME_CHECKED(UFoldingSkyStoryCallbackProxyFactory, CreateOneChoiceStoryProxyObject),
			GET_FUNCTION_NAME_CHECKED(UFoldingSkyStoryCallbackProxyFactory, CreateTwoChoiceStoryProxyObject),
			GET_FUNCTION_NAME_CHECKED(UFoldingSkyStoryCallbackProxyFactory, CreateThreeChoiceStoryProxyObject),
			GET_FUNCTION_NAME_CHECKED(UFoldingSkyStoryCallbackProxyFactory, CreateFourChoiceStoryProxyObject),
			GET_FUNCTION_NAME_CHECKED(UFoldingSkyStoryCallbackProxyFactory, CreateFiveChoiceStoryProxyObject),
			GET_FUNCTION_NAME_CHECKED(UFoldingSkyStoryCallbackProxyFactory, CreateSixChoiceStoryProxyObject),
			GET_FUNCTION_NAME_CHECKED(UFoldingSkyStoryCallbackProxyFactory, CreateSevenChoiceStoryProxyObject),
			GET_FUNCTION_NAME_CHECKED(UFoldingSkyStoryCallbackProxyFactory, CreateEightChoiceStoryProxyObject),
			GET_FUNCTION_NAME_CHECKED(UFoldingSkyStoryCallbackProxyFactory, CreateNineChoiceStoryProxyObject),
			GET_FUNCTION_NAME_CHECKED(UFoldingSkyStoryCallbackProxyFactory, CreateTenChoiceStoryProxyObject)
		};		
		break;
	}
	if (Names.IsValidIndex(Index))
	{
		OutFunctionName = Names[Index];
	}
	else
	{
		OutFunctionName = GET_FUNCTION_NAME_CHECKED(UFoldingSkyStoryCallbackProxyFactory, CreateOneChoiceStoryProxyObject);
	}
}
// BEGIN CUSTOM STRUCT REGION
#pragma region CUSTOM

FString UFoldingSkyStoryCallbackProxyFactory::CreateCustomStoryProxyJson(UStruct* CustomStruct)
{
	TSharedRef<FJsonObject> JsonObject = MakeShareable(new FJsonObject);
	FJsonObjectConverter::UStructToJsonObject(CustomStruct->GetClass(), CustomStruct, JsonObject, 0, 0);

	FString JsonString;
	TSharedRef<TJsonWriter<>> Writer = TJsonWriterFactory<>::Create(&JsonString);
	FJsonSerializer::Serialize(JsonObject, Writer);
	return JsonString;
}

UOneWayFoldingSkyStoryCallbackProxy* UFoldingSkyStoryCallbackProxyFactory::CreateCustomBasicStoryProxyObject(
	UFoldingSkyStoryComponent* StoryComponent,
	UStruct* CustomStruct)
{
	const TArray<FText>& Choices = StoryComponent->GetBasicChoices();
	const FString& CustomJson = CreateCustomStoryProxyJson(CustomStruct);
	return TriggerCustomStoryNode<UOneWayFoldingSkyStoryCallbackProxy>(StoryComponent, CustomJson, Choices);
}
UTwoWayFoldingSkyStoryCallbackProxy* UFoldingSkyStoryCallbackProxyFactory::CreateCustomBinaryStoryProxyObject(
	UFoldingSkyStoryComponent* StoryComponent,
	UStruct* CustomStruct)
{
	const TArray<FText>& Choices = StoryComponent->GetBinaryChoices();
	const FString& CustomJson = CreateCustomStoryProxyJson(CustomStruct);
	return TriggerCustomStoryNode<UTwoWayFoldingSkyStoryCallbackProxy>(StoryComponent, CustomJson, Choices);
}
UOneChoiceFoldingSkyStoryCallbackProxy* UFoldingSkyStoryCallbackProxyFactory::CreateCustomOneChoiceStoryProxyObject(
	UFoldingSkyStoryComponent* StoryComponent,
	UStruct* CustomStruct,
	FText Choice1)
{
	const TArray<FText>& Choices = TArray<FText>{ Choice1 };
	const FString& CustomJson = CreateCustomStoryProxyJson(CustomStruct);
	return TriggerCustomStoryNode<UOneChoiceFoldingSkyStoryCallbackProxy>(StoryComponent, CustomJson, Choices);
}
USixChoiceFoldingSkyStoryCallbackProxy* UFoldingSkyStoryCallbackProxyFactory::CreateCustomTwoChoiceStoryProxyObject(
	UFoldingSkyStoryComponent* StoryComponent,
	UStruct* CustomStruct,
	FText Choice1,
	FText Choice2)
{
	const TArray<FText>& Choices = TArray<FText>{ Choice1, Choice2 };
	const FString& CustomJson = CreateCustomStoryProxyJson(CustomStruct);
	return TriggerCustomStoryNode<USixChoiceFoldingSkyStoryCallbackProxy>(StoryComponent, CustomJson, Choices);
}
USixChoiceFoldingSkyStoryCallbackProxy* UFoldingSkyStoryCallbackProxyFactory::CreateCustomThreeChoiceStoryProxyObject(
	UFoldingSkyStoryComponent* StoryComponent,
	UStruct* CustomStruct,
	FText Choice1,
	FText Choice2,
	FText Choice3)
{
	const TArray<FText>& Choices = TArray<FText>{ Choice1, Choice2, Choice3 };
	const FString& CustomJson = CreateCustomStoryProxyJson(CustomStruct);
	return TriggerCustomStoryNode<USixChoiceFoldingSkyStoryCallbackProxy>(StoryComponent, CustomJson, Choices);
}
USixChoiceFoldingSkyStoryCallbackProxy* UFoldingSkyStoryCallbackProxyFactory::CreateCustomFourChoiceStoryProxyObject(
	UFoldingSkyStoryComponent* StoryComponent,
	UStruct* CustomStruct,
	FText Choice1,
	FText Choice2,
	FText Choice3,
	FText Choice4)
{
	const TArray<FText>& Choices = TArray<FText>{ Choice1, Choice2, Choice3, Choice4 };
	const FString& CustomJson = CreateCustomStoryProxyJson(CustomStruct);
	return TriggerCustomStoryNode<USixChoiceFoldingSkyStoryCallbackProxy>(StoryComponent, CustomJson, Choices);
}
USixChoiceFoldingSkyStoryCallbackProxy* UFoldingSkyStoryCallbackProxyFactory::CreateCustomFiveChoiceStoryProxyObject(
	UFoldingSkyStoryComponent* StoryComponent,
	UStruct* CustomStruct,
	FText Choice1,
	FText Choice2,
	FText Choice3,
	FText Choice4,
	FText Choice5)
{
	const TArray<FText>& Choices = TArray<FText>{ Choice1, Choice2, Choice3, Choice4, Choice5 };
	const FString& CustomJson = CreateCustomStoryProxyJson(CustomStruct);
	return TriggerCustomStoryNode<USixChoiceFoldingSkyStoryCallbackProxy>(StoryComponent, CustomJson, Choices);
}
USixChoiceFoldingSkyStoryCallbackProxy* UFoldingSkyStoryCallbackProxyFactory::CreateCustomSixChoiceStoryProxyObject(
	UFoldingSkyStoryComponent* StoryComponent,
	UStruct* CustomStruct,
	FText Choice1,
	FText Choice2,
	FText Choice3,
	FText Choice4,
	FText Choice5,
	FText Choice6)
{
	const TArray<FText>& Choices = TArray<FText>{ Choice1, Choice2, Choice3, Choice4, Choice5, Choice6 };
	const FString& CustomJson = CreateCustomStoryProxyJson(CustomStruct);
	return TriggerCustomStoryNode<USixChoiceFoldingSkyStoryCallbackProxy>(StoryComponent, CustomJson, Choices);
}
#pragma endregion
// END CUSTOM STRUCT REGION