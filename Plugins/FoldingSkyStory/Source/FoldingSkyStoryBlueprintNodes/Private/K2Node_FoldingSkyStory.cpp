// Copyright Folding Sky Games LLC 2021 All rights reserved.

#include "K2Node_FoldingSkyStory.h"

#include "FoldingSkyStoryCallbackProxy.h"

#include "Kismet2/BlueprintEditorUtils.h"
#include "Kismet/KismetSystemLibrary.h"
#include "ToolMenus.h"
#include "ScopedTransaction.h"
#include "KismetCompiler.h"
#include "K2Node_CallFunction.h"
#include "K2Node_AddDelegate.h"
#include "K2Node_CustomEvent.h"
#include "K2Node_TemporaryVariable.h"
#include "K2Node_IfThenElse.h"
#include "K2Node_AssignmentStatement.h"

#include "Sound/DialogueWave.h"

#define LOCTEXT_NAMESPACE "K2Node_FoldingSkyStory"

struct FStoryNodeNames
{	
	static FName GetInputName(const int32& PinIndex)
	{
		return *FString::Printf(TEXT("Choice%d"), PinIndex);
	}
	static FName GetOutputName(const int32& PinIndex)
	{
		return *FString::Printf(TEXT("OnChoice%d"), PinIndex);
	}
	static FText GetInputFriendlyName(const int32& PinIndex)
	{
		return FText::Format<int32>(LOCTEXT("StoryChoiceInFriendlyName", "Choice {0}"), PinIndex);
	}
	static FText GetOutputFriendlyName(const int32& PinIndex)
	{
		return FText::Format<int32>(LOCTEXT("StoryChoiceOutFriendlyName", "On Choice {0}"), PinIndex);
	}
};

UK2Node_FoldingSkyStoryTyped::UK2Node_FoldingSkyStoryTyped(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	ProxyFactoryClass = UFoldingSkyStoryCallbackProxyFactory::StaticClass();
}
void UK2Node_FoldingSkyStoryTyped::SetNumChoices(const int32& NewValue)
{
	NumChoices = NewValue;
	SyncProxyClassAndFunctionName();
}
void UK2Node_FoldingSkyStoryTyped::SyncProxyClassAndFunctionName()
{
	const int32& index = NumChoices - 1;
	UFoldingSkyStoryCallbackProxyFactory::GetClassAndFunctionName(NodeType, index, ProxyClass, ProxyFactoryFunctionName);
}

#if WITH_EDITOR

bool UK2Node_FoldingSkyStoryWithChoices::IsChoicePin(const UEdGraphPin* Pin) const
{
	return Pin->PinName.ToString().Contains(TEXT("Choice"));
}
void UK2Node_FoldingSkyStoryWithChoices::AllocateDefaultPins()
{
	SyncProxyClassAndFunctionName();
	Super::AllocateDefaultPins();
	// A stupid hack to move all choice pins to a standard location at load time
	ClearChoicePins();
	
	for (int32 i = 1; i <= NumChoices; i++)
	{
		AddChoicePins(i);
	}
}
void UK2Node_FoldingSkyStoryWithChoices::ExpandNode(class FKismetCompilerContext& CompilerContext, UEdGraph* SourceGraph)
{
	UK2Node::ExpandNode(CompilerContext, SourceGraph);
	const UEdGraphSchema_K2* Schema = CompilerContext.GetSchema();
	check(SourceGraph && Schema);
	bool bIsErrorFree = true;

	// Create a call to factory the proxy object
	UK2Node_CallFunction* const CallCreateProxyObjectNode = CompilerContext.SpawnIntermediateNode<UK2Node_CallFunction>(this, SourceGraph);
	CallCreateProxyObjectNode->FunctionReference.SetExternalMember(ProxyFactoryFunctionName, ProxyFactoryClass);
	CallCreateProxyObjectNode->AllocateDefaultPins();
	if (CallCreateProxyObjectNode->GetTargetFunction() == nullptr)
	{
		const FText ClassName = ProxyFactoryClass ? FText::FromString(ProxyFactoryClass->GetName()) : LOCTEXT("MissingClassString", "Unknown Class");
		const FString FormattedMessage = FText::Format(
			LOCTEXT("AsyncTaskErrorFmt", "BaseAsyncTask: Missing function {0} from class {1} for async task @@"),
			FText::FromString(ProxyFactoryFunctionName.GetPlainNameString()),
			ClassName
		).ToString();

		CompilerContext.MessageLog.Error(*FormattedMessage, this);
		return;
	}

	bIsErrorFree &= CompilerContext.MovePinLinksToIntermediate(*FindPinChecked(UEdGraphSchema_K2::PN_Execute), *CallCreateProxyObjectNode->FindPinChecked(UEdGraphSchema_K2::PN_Execute)).CanSafeConnect();

	for (UEdGraphPin* CurrentPin : Pins)
	{
		if (FBaseAsyncTaskHelper::ValidDataPin(CurrentPin, EGPD_Input))
		{
			UEdGraphPin* DestPin = CallCreateProxyObjectNode->FindPin(CurrentPin->PinName); // match function inputs, to pass data to function from CallFunction node
			bIsErrorFree &= DestPin && CompilerContext.MovePinLinksToIntermediate(*CurrentPin, *DestPin).CanSafeConnect();
		}
	}

	UEdGraphPin* const ProxyObjectPin = CallCreateProxyObjectNode->GetReturnValuePin();
	check(ProxyObjectPin);
	UEdGraphPin* OutputAsyncTaskProxy = FindPin(FBaseAsyncTaskHelper::GetAsyncTaskProxyName());
	bIsErrorFree &= !OutputAsyncTaskProxy || CompilerContext.MovePinLinksToIntermediate(*OutputAsyncTaskProxy, *ProxyObjectPin).CanSafeConnect();

	// GATHER OUTPUT PARAMETERS AND PAIR THEM WITH LOCAL VARIABLES
	TArray<FBaseAsyncTaskHelper::FOutputPinAndLocalVariable> VariableOutputs;
	bool bPassedFactoryOutputs = false;
	for (UEdGraphPin* CurrentPin : Pins)
	{
		if ((OutputAsyncTaskProxy != CurrentPin) && FBaseAsyncTaskHelper::ValidDataPin(CurrentPin, EGPD_Output))
		{
			if (!bPassedFactoryOutputs)
			{
				UEdGraphPin* DestPin = CallCreateProxyObjectNode->FindPin(CurrentPin->PinName);
				bIsErrorFree &= DestPin && CompilerContext.MovePinLinksToIntermediate(*CurrentPin, *DestPin).CanSafeConnect();
			}
			else
			{
				const FEdGraphPinType& PinType = CurrentPin->PinType;
				UK2Node_TemporaryVariable* TempVarOutput = CompilerContext.SpawnInternalVariable(
					this, PinType.PinCategory, PinType.PinSubCategory, PinType.PinSubCategoryObject.Get(), PinType.ContainerType, PinType.PinValueType);
				bIsErrorFree &= TempVarOutput->GetVariablePin() && CompilerContext.MovePinLinksToIntermediate(*CurrentPin, *TempVarOutput->GetVariablePin()).CanSafeConnect();
				VariableOutputs.Add(FBaseAsyncTaskHelper::FOutputPinAndLocalVariable(CurrentPin, TempVarOutput));
			}
		}
		else if (!bPassedFactoryOutputs && CurrentPin && CurrentPin->Direction == EGPD_Output)
		{
			// the first exec that isn't the node's then pin is the start of the asyc delegate pins
			// once we hit this point, we've iterated beyond all outputs for the factory function
			bPassedFactoryOutputs = (CurrentPin->PinType.PinCategory == UEdGraphSchema_K2::PC_Exec) && (CurrentPin->PinName != UEdGraphSchema_K2::PN_Then);
		}
	}

	// FOR EACH DELEGATE DEFINE EVENT, CONNECT IT TO DELEGATE AND IMPLEMENT A CHAIN OF ASSIGMENTS
	UEdGraphPin* LastThenPin = CallCreateProxyObjectNode->FindPinChecked(UEdGraphSchema_K2::PN_Then);

	UK2Node_CallFunction* IsValidFuncNode = CompilerContext.SpawnIntermediateNode<UK2Node_CallFunction>(this, SourceGraph);
	const FName IsValidFuncName = GET_FUNCTION_NAME_CHECKED(UKismetSystemLibrary, IsValid);
	IsValidFuncNode->FunctionReference.SetExternalMember(IsValidFuncName, UKismetSystemLibrary::StaticClass());
	IsValidFuncNode->AllocateDefaultPins();
	UEdGraphPin* IsValidInputPin = IsValidFuncNode->FindPinChecked(TEXT("Object"));

	bIsErrorFree &= Schema->TryCreateConnection(ProxyObjectPin, IsValidInputPin);

	UK2Node_IfThenElse* ValidateProxyNode = CompilerContext.SpawnIntermediateNode<UK2Node_IfThenElse>(this, SourceGraph);
	ValidateProxyNode->AllocateDefaultPins();
	bIsErrorFree &= Schema->TryCreateConnection(IsValidFuncNode->GetReturnValuePin(), ValidateProxyNode->GetConditionPin());

	bIsErrorFree &= Schema->TryCreateConnection(LastThenPin, ValidateProxyNode->GetExecPin());
	LastThenPin = ValidateProxyNode->GetThenPin();

	for (TFieldIterator<FMulticastDelegateProperty> PropertyIt(ProxyClass); PropertyIt && bIsErrorFree; ++PropertyIt)
	{
		bIsErrorFree &= HandleDelegateImplementation(*PropertyIt, VariableOutputs, ProxyObjectPin, LastThenPin, this, SourceGraph, CompilerContext);
	}

	if (CallCreateProxyObjectNode->FindPinChecked(UEdGraphSchema_K2::PN_Then) == LastThenPin)
	{
		CompilerContext.MessageLog.Error(*LOCTEXT("MissingDelegateProperties", "BaseAsyncTask: Proxy has no delegates defined. @@").ToString(), this);
		return;
	}

	// Create a call to activate the proxy object if necessary
	if (ProxyActivateFunctionName != NAME_None)
	{
		UK2Node_CallFunction* const CallActivateProxyObjectNode = CompilerContext.SpawnIntermediateNode<UK2Node_CallFunction>(this, SourceGraph);
		CallActivateProxyObjectNode->FunctionReference.SetExternalMember(ProxyActivateFunctionName, ProxyClass);
		CallActivateProxyObjectNode->AllocateDefaultPins();

		// Hook up the self connection
		UEdGraphPin* ActivateCallSelfPin = Schema->FindSelfPin(*CallActivateProxyObjectNode, EGPD_Input);
		check(ActivateCallSelfPin);

		bIsErrorFree &= Schema->TryCreateConnection(ProxyObjectPin, ActivateCallSelfPin);

		// Hook the activate node up in the exec chain
		UEdGraphPin* ActivateExecPin = CallActivateProxyObjectNode->FindPinChecked(UEdGraphSchema_K2::PN_Execute);
		UEdGraphPin* ActivateThenPin = CallActivateProxyObjectNode->FindPinChecked(UEdGraphSchema_K2::PN_Then);

		bIsErrorFree &= Schema->TryCreateConnection(LastThenPin, ActivateExecPin);

		LastThenPin = ActivateThenPin;
	}

	// Move the connections from the original node then pin to the last internal then pin

	UEdGraphPin* OriginalThenPin = FindPin(UEdGraphSchema_K2::PN_Then);

	if (OriginalThenPin)
	{
		bIsErrorFree &= CompilerContext.MovePinLinksToIntermediate(*OriginalThenPin, *LastThenPin).CanSafeConnect();
	}
	bIsErrorFree &= CompilerContext.CopyPinLinksToIntermediate(*LastThenPin, *ValidateProxyNode->GetElsePin()).CanSafeConnect();

	if (!bIsErrorFree)
	{
		CompilerContext.MessageLog.Error(*LOCTEXT("InternalConnectionError", "BaseAsyncTask: Internal connection error. @@").ToString(), this);
	}

	// Make sure we caught everything
	BreakAllNodeLinks();
}

bool UK2Node_FoldingSkyStoryWithChoices::HandleDelegateImplementation(
	FMulticastDelegateProperty* CurrentProperty, const TArray<UK2Node_BaseAsyncTask::FBaseAsyncTaskHelper::FOutputPinAndLocalVariable>& VariableOutputs,
	UEdGraphPin* ProxyObjectPin, UEdGraphPin*& InOutLastThenPin,
	UK2Node* CurrentNode, UEdGraph* SourceGraph, FKismetCompilerContext& CompilerContext)
{
	bool bIsErrorFree = true;
	const UEdGraphSchema_K2* Schema = CompilerContext.GetSchema();
	check(CurrentProperty && ProxyObjectPin && InOutLastThenPin && CurrentNode && SourceGraph && Schema);

	UEdGraphPin* PinForCurrentDelegateProperty = CurrentNode->FindPin(CurrentProperty->GetFName());
	if (!PinForCurrentDelegateProperty || (UEdGraphSchema_K2::PC_Exec != PinForCurrentDelegateProperty->PinType.PinCategory))
	{
		//return true; // HACK TO ALLOW MANY CHOICES WITH FEWER INTERNAL LINKS
		FText ErrorMessage = FText::Format(LOCTEXT("WrongDelegateProperty", "BaseAsyncTask: Cannot find execution pin for delegate "), FText::FromString(CurrentProperty->GetName()));
		CompilerContext.MessageLog.Error(*ErrorMessage.ToString(), CurrentNode);
		return false;
	}

	UK2Node_CustomEvent* CurrentCENode = CompilerContext.SpawnIntermediateEventNode<UK2Node_CustomEvent>(CurrentNode, PinForCurrentDelegateProperty, SourceGraph);
	{
		UK2Node_AddDelegate* AddDelegateNode = CompilerContext.SpawnIntermediateNode<UK2Node_AddDelegate>(CurrentNode, SourceGraph);
		AddDelegateNode->SetFromProperty(CurrentProperty, false, CurrentProperty->GetOwnerClass());
		AddDelegateNode->AllocateDefaultPins();
		bIsErrorFree &= Schema->TryCreateConnection(AddDelegateNode->FindPinChecked(UEdGraphSchema_K2::PN_Self), ProxyObjectPin);
		bIsErrorFree &= Schema->TryCreateConnection(InOutLastThenPin, AddDelegateNode->FindPinChecked(UEdGraphSchema_K2::PN_Execute));
		InOutLastThenPin = AddDelegateNode->FindPinChecked(UEdGraphSchema_K2::PN_Then);
		CurrentCENode->CustomFunctionName = *FString::Printf(TEXT("%s_%s"), *CurrentProperty->GetName(), *CompilerContext.GetGuid(CurrentNode));
		CurrentCENode->AllocateDefaultPins();

		bIsErrorFree &= FBaseAsyncTaskHelper::CreateDelegateForNewFunction(AddDelegateNode->GetDelegatePin(), CurrentCENode->GetFunctionName(), CurrentNode, SourceGraph, CompilerContext);
		bIsErrorFree &= FBaseAsyncTaskHelper::CopyEventSignature(CurrentCENode, AddDelegateNode->GetDelegateSignature(), Schema);
	}

	UEdGraphPin* LastActivatedNodeThen = CurrentCENode->FindPinChecked(UEdGraphSchema_K2::PN_Then);
	for (const FBaseAsyncTaskHelper::FOutputPinAndLocalVariable& OutputPair : VariableOutputs) // CREATE CHAIN OF ASSIGMENTS
	{
		UEdGraphPin* PinWithData = CurrentCENode->FindPin(OutputPair.OutputPin->PinName);
		if (PinWithData == nullptr)
		{
			/*FText ErrorMessage = FText::Format(LOCTEXT("MissingDataPin", "ICE: Pin @@ was expecting a data output pin named {0} on @@ (each delegate must have the same signature)"), FText::FromString(OutputPair.OutputPin->PinName));
			CompilerContext.MessageLog.Error(*ErrorMessage.ToString(), OutputPair.OutputPin, CurrentCENode);
			return false;*/
			continue;
		}

		UK2Node_AssignmentStatement* AssignNode = CompilerContext.SpawnIntermediateNode<UK2Node_AssignmentStatement>(CurrentNode, SourceGraph);
		AssignNode->AllocateDefaultPins();
		bIsErrorFree &= Schema->TryCreateConnection(LastActivatedNodeThen, AssignNode->GetExecPin());
		bIsErrorFree &= Schema->TryCreateConnection(OutputPair.TempVar->GetVariablePin(), AssignNode->GetVariablePin());
		AssignNode->NotifyPinConnectionListChanged(AssignNode->GetVariablePin());
		bIsErrorFree &= Schema->TryCreateConnection(AssignNode->GetValuePin(), PinWithData);
		AssignNode->NotifyPinConnectionListChanged(AssignNode->GetValuePin());

		LastActivatedNodeThen = AssignNode->GetThenPin();
	}

	bIsErrorFree &= CompilerContext.MovePinLinksToIntermediate(*PinForCurrentDelegateProperty, *LastActivatedNodeThen).CanSafeConnect();
	return bIsErrorFree;
}
void UK2Node_FoldingSkyStoryWithChoices::ClearChoicePins()
{
	for (int32 i = 0; i < Pins.Num(); i++)
	{
		if (UEdGraphPin* Pin = Pins[i])
		{
			if (IsChoicePin(Pin))
			{
				Pins.RemoveAt(i);
				Pin->MarkPendingKill();
				i--;
			}
		}
	}
}
void UK2Node_FoldingSkyStoryWithChoices::GetNodeContextMenuActions(UToolMenu* Menu, UGraphNodeContextMenuContext* Context) const
{
	Super::GetNodeContextMenuActions(Menu, Context);

	if (!Context->bIsDebugging)
	{
		FToolMenuSection& Section = Menu->FindOrAddSection(TEXT("Story"));//"K2NodeStory", NSLOCTEXT("K2Nodes", "StoryHeader", "Story"));

		if (Context->Pin)
		{
			if (IsChoicePin(Context->Pin) && Context->Pin->ParentPin == nullptr)
			{
				Section.AddMenuEntry(
					"RemovePin",
					LOCTEXT("RemovePin", "Remove choice pin"),
					LOCTEXT("RemovePinTooltip", "Remove this choice pin and its corresponding execution path"),
					FSlateIcon(),
					FUIAction(
						FExecuteAction::CreateUObject(const_cast<UK2Node_FoldingSkyStoryWithChoices*>(this), &UK2Node_FoldingSkyStoryWithChoices::RemoveInputPin, const_cast<UEdGraphPin*>(Context->Pin)),
						FCanExecuteAction::CreateUObject(this, &UK2Node_FoldingSkyStoryWithChoices::CanRemoveChoicePin, const_cast<UEdGraphPin*>(Context->Pin))
					)
				);
			}
		}
		else
		{
			Section.AddMenuEntry(
				"AddPin",
				LOCTEXT("AddPin", "Add choice pin"),
				LOCTEXT("AddPinTooltip", "Add another choice pin and execution path"),
				FSlateIcon(),
				FUIAction(
					FExecuteAction::CreateUObject(const_cast<UK2Node_FoldingSkyStoryWithChoices*>(this), &UK2Node_FoldingSkyStoryWithChoices::AddInputPin),
					FCanExecuteAction::CreateUObject(this, &UK2Node_FoldingSkyStoryWithChoices::CanAddPin)
				)
			);			
		}
	}
}

void UK2Node_FoldingSkyStoryWithChoices::AddInputPin()
{
	Modify();
	++NumChoices;
	SyncProxyClassAndFunctionName();
	AddChoicePins(NumChoices);

	const bool bIsCompiling = GetBlueprint()->bBeingCompiled;
	if (!bIsCompiling)
	{
		FBlueprintEditorUtils::MarkBlueprintAsStructurallyModified(GetBlueprint());
	}
}
void UK2Node_FoldingSkyStoryWithChoices::AddChoicePins(const int32& Identifier)
{	
	CreatePin(EGPD_Input, UEdGraphSchema_K2::PC_Text, FStoryNodeNames::GetInputName(Identifier));
	CreatePin(EGPD_Output, UEdGraphSchema_K2::PC_Exec, FStoryNodeNames::GetOutputName(Identifier));
}
bool UK2Node_FoldingSkyStoryWithChoices::CanRemoveChoicePin(UEdGraphPin* Pin) const
{
	return NumChoices > 1 && IsChoicePin(Pin);
}
bool UK2Node_FoldingSkyStoryWithChoices::CanAddPin() const
{
	return NumChoices < MAXSTORYCHOICES;
}
void UK2Node_FoldingSkyStoryWithChoices::RemoveTargetPin(UEdGraphPin* PinToRemove)
{
	for (int32 SubPinIndex = PinToRemove->SubPins.Num() - 1; SubPinIndex >= 0; --SubPinIndex)
	{
		RemoveTargetPin(PinToRemove->SubPins[SubPinIndex]);
	}

	int32 PinRemovalIndex = INDEX_NONE;
	if (Pins.Find(PinToRemove, PinRemovalIndex))
	{
		Pins.RemoveAt(PinRemovalIndex);
		PinToRemove->MarkPendingKill();
	}
}
void UK2Node_FoldingSkyStoryWithChoices::RemoveInputPin(UEdGraphPin* Pin)
{
	check(Pin->ParentPin == nullptr);
	checkSlow(Pins.Contains(Pin));

	FScopedTransaction Transaction(LOCTEXT("RemovePinTx", "RemovePin"));
	Modify();

	TArray<UEdGraphPin*> ChoicePins, ExecPins;
	GetChoiceAndExecPins(ChoicePins, ExecPins);
	int32 PinIndex = INDEX_NONE;
	if (ChoicePins.Find(Pin, PinIndex))
	{
		if (ExecPins.IsValidIndex(PinIndex))
		{
			RemoveTargetPin(ExecPins[PinIndex]);
		}
	} 
	else if (ExecPins.Find(Pin, PinIndex))
	{
		if (ChoicePins.IsValidIndex(PinIndex))
		{
			RemoveTargetPin(ChoicePins[PinIndex]);
		}
	}
	RemoveTargetPin(Pin);
	SyncPinNames(ChoicePins, ExecPins);
	PinConnectionListChanged(Pin);	
	
	FBlueprintEditorUtils::MarkBlueprintAsStructurallyModified(GetBlueprint());
}
void UK2Node_FoldingSkyStoryWithChoices::GetChoiceAndExecPins(TArray<UEdGraphPin*>& ChoicePins, TArray<UEdGraphPin*>& ExecPins) const
{
	for (UEdGraphPin* CurrentPin : Pins)
	{
		if (CurrentPin->PinName.ToString().Contains(TEXT("Choice")))
		{
			if (CurrentPin->Direction == EGPD_Input && CurrentPin->ParentPin == nullptr && CurrentPin->PinType.PinCategory == UEdGraphSchema_K2::PC_Text)
			{
				ChoicePins.Add(CurrentPin);
			}
			if (CurrentPin->Direction == EGPD_Output && CurrentPin->ParentPin == nullptr && CurrentPin->PinType.PinCategory == UEdGraphSchema_K2::PC_Exec)
			{
				ExecPins.Add(CurrentPin);
			}
		}
	}
	check(ChoicePins.Num() == ExecPins.Num());
}
void UK2Node_FoldingSkyStoryWithChoices::SyncPinNames(const TArray<UEdGraphPin*>& ChoicePins, const TArray<UEdGraphPin*>& ExecPins)
{
	int32 CurrentNumInputPins = 0;
	for (int32 PinIndex = 0; PinIndex < ChoicePins.Num(); ++PinIndex)
	{
		UEdGraphPin* CurrentPin = ChoicePins[PinIndex];
		if (CurrentPin->GetOwningNodeUnchecked())
		{
			CurrentNumInputPins++;
			CurrentPin->Modify();
			CurrentPin->PinName = FStoryNodeNames::GetInputName(CurrentNumInputPins);
			CurrentPin->PinFriendlyName = FStoryNodeNames::GetInputFriendlyName(CurrentNumInputPins);
		}
	}
	int32 CurrentNumOutputPins = 0;
	for (int32 PinIndex = 0; PinIndex < ExecPins.Num(); ++PinIndex)
	{
		UEdGraphPin* CurrentPin = ExecPins[PinIndex];
		if (CurrentPin->GetOwningNodeUnchecked())
		{
			CurrentNumOutputPins++;
			CurrentPin->Modify();
			CurrentPin->PinName = FStoryNodeNames::GetOutputName(CurrentNumOutputPins);
			CurrentPin->PinFriendlyName = FStoryNodeNames::GetOutputFriendlyName(CurrentNumOutputPins);
		}
	}
	check(CurrentNumInputPins == CurrentNumOutputPins);
	SetNumChoices(CurrentNumInputPins);
}

void UK2Node_FoldingSkyStoryTyped::ChangeNodeType(EFoldingSkyStoryNodeType NewNodeType)
{
	if (NewNodeType != NodeType)
	{
		UEdGraphPin* PinToChange = nullptr;
		const FString& DialogueName = TEXT("Dialogue");
		const FString& DialogueTooltip = TEXT("Dialogue\nDialogue Wave Soft Object Reference");
		const FString& TextName = TEXT("Story Text");
		const FString& TextTooltip = TEXT("Story Text\nText");
		const FString& CustomName = TEXT("Custom Struct");
		switch (NodeType)
		{		
		case EFoldingSkyStoryNodeType::DialogueNode:
			PinToChange = FindPin(DialogueName, EGPD_Input);
			break;
		case EFoldingSkyStoryNodeType::CustomNode:
			PinToChange = FindPin(CustomName.Replace(TEXT(" "), TEXT("")), EGPD_Input);
			break;			
		case EFoldingSkyStoryNodeType::TextNode:			
		default:
			PinToChange = FindPin(TextName.Replace(TEXT(" "), TEXT("")), EGPD_Input);
			break;
		}
		if (PinToChange)
		{
			Modify();
			switch (NewNodeType)
			{			
			case EFoldingSkyStoryNodeType::DialogueNode:
				PinToChange->PinName = FName(DialogueName);
				PinToChange->PinFriendlyName = FText::FromString(DialogueName);
				// PinToChange->PinFriendlyName = FText::FromString(DialogueName);
				PinToChange->PinType.PinCategory = UEdGraphSchema_K2::PC_SoftObject;
				PinToChange->PinType.PinSubCategoryObject = UDialogueWave::StaticClass();
				PinToChange->PinToolTip = DialogueTooltip;
				break;
			case EFoldingSkyStoryNodeType::CustomNode:
				PinToChange->PinName = FName(CustomName.Replace(TEXT(" "), TEXT("")));
				PinToChange->PinFriendlyName = FText::FromString(CustomName);
				PinToChange->PinType.PinCategory = UEdGraphSchema_K2::PC_Wildcard;
				PinToChange->PinType.PinSubCategoryObject = nullptr;
				PinToChange->PinToolTip = CustomName;
				break;
			case EFoldingSkyStoryNodeType::TextNode:
				PinToChange->PinName = FName(TextName.Replace(TEXT(" "), TEXT("")));
				PinToChange->PinFriendlyName = FText::FromString(TextName);
				PinToChange->PinType.PinCategory = UEdGraphSchema_K2::PC_Text;
				PinToChange->PinType.PinSubCategoryObject = nullptr;
				PinToChange->PinToolTip = TextTooltip;
			default:
				break;
			}
			PinTypeChanged(PinToChange);
			const bool bIsCompiling = GetBlueprint()->bBeingCompiled;
			if (!bIsCompiling)
			{
				FBlueprintEditorUtils::MarkBlueprintAsStructurallyModified(GetBlueprint());
			}
		}
		NodeType = NewNodeType;
		SyncProxyClassAndFunctionName();
	}
}
bool UK2Node_FoldingSkyStoryTyped::CanChangeToNodeType(EFoldingSkyStoryNodeType NewNodeType) const
{ 
	if (NewNodeType == EFoldingSkyStoryNodeType::CustomNode)
		return false; // TODO ADD SUPPORT FOR CUSTOM NODES
	return NewNodeType != NodeType; 
}
void UK2Node_FoldingSkyStoryTyped::GetNodeContextMenuActions(UToolMenu* Menu, UGraphNodeContextMenuContext* Context) const
{
	Super::GetNodeContextMenuActions(Menu, Context);

	if (!Context->bIsDebugging)
	{
		FToolMenuSection& Section = Menu->AddSection("K2NodeStory", NSLOCTEXT("K2Nodes", "StoryHeader", "Story"));
		Section.AddMenuEntry(
			"ChangeToTextNode",
			LOCTEXT("ChangeToTextNode", "Change node to Text type"),
			LOCTEXT("ChangeToTextNodeTooltip", "Changes this story nodes type to text"),
			FSlateIcon(),
			FUIAction(
				FExecuteAction::CreateUObject(const_cast<UK2Node_FoldingSkyStoryTyped*>(this), &UK2Node_FoldingSkyStoryTyped::ChangeNodeType, EFoldingSkyStoryNodeType::TextNode),
				FCanExecuteAction::CreateUObject(this, &UK2Node_FoldingSkyStoryTyped::CanChangeToNodeType, EFoldingSkyStoryNodeType::TextNode)
			)
		);
		Section.AddMenuEntry(
			"ChangeToDialogueNode",
			LOCTEXT("ChangeToDialogueNode", "Change node to Dialogue type"),
			LOCTEXT("ChangeToDialogueNodeTooltip", "Changes this story nodes type to dialogue"),
			FSlateIcon(),
			FUIAction(
				FExecuteAction::CreateUObject(const_cast<UK2Node_FoldingSkyStoryTyped*>(this), &UK2Node_FoldingSkyStoryTyped::ChangeNodeType, EFoldingSkyStoryNodeType::DialogueNode),
				FCanExecuteAction::CreateUObject(this, &UK2Node_FoldingSkyStoryTyped::CanChangeToNodeType, EFoldingSkyStoryNodeType::DialogueNode)
			)
		);
		Section.AddMenuEntry(
			"ChangeToCustomNode",
			LOCTEXT("ChangeToCustomNode", "*NOT YET SUPPORTED* Change node to Custom type"),
			LOCTEXT("ChangeToCustomNodeTooltip", "*NOT YET SUPPORTED* Changes this story nodes type to a wildcard struct"),
			FSlateIcon(),
			FUIAction(
				FExecuteAction::CreateUObject(const_cast<UK2Node_FoldingSkyStoryTyped*>(this), &UK2Node_FoldingSkyStoryTyped::ChangeNodeType, EFoldingSkyStoryNodeType::CustomNode),
				FCanExecuteAction::CreateUObject(this, &UK2Node_FoldingSkyStoryTyped::CanChangeToNodeType, EFoldingSkyStoryNodeType::CustomNode)
			)
		);
	}
}

FText UK2Node_FoldingSkyStoryTyped::GetTooltipText() const
{
	const FText& NodeTypeText = NodeType == EFoldingSkyStoryNodeType::TextNode ? LOCTEXT("StoryTextTypeTooltip", "a text pin") : (NodeType == EFoldingSkyStoryNodeType::DialogueNode ? LOCTEXT("StoryDialogueTypeTooltip", "a dialogue pin") : LOCTEXT("CustomStoryTypeTooltip", "a custom struct pin")) ;
	const FText& NumChoicesText = NumChoices == 0 ? LOCTEXT("StoryOneWayTooltip", "only one choice, based on a story component default") : (NumChoices == -1 ? LOCTEXT("StoryTwoWayTooltip", "only two choices, based on story component defaults") : LOCTEXT("StoryMultipleChoiceTooltip", "multiple choices can be defined on the node"));
	return FText::FormatOrdered(FTextFormat::FromString(TEXT("A story node with {0} to be passed along the story event graph\n{1}")), NodeTypeText, NumChoicesText);
}
#endif // WITH_EDITOR

#undef LOCTEXT_NAMESPACE