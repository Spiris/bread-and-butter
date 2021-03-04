// Copyright Folding Sky Games LLC 2021 All rights reserved.

#pragma once

#include "CoreMinimal.h"

#include "FoldingSkyStoryNodeTypeEnum.h"
#include "K2Node_AddPinInterface.h"
#include "K2Node_BaseAsyncTask.h"
#include "K2Node_FoldingSkyStory.generated.h"

UCLASS(Abstract)
class FOLDINGSKYSTORYBLUEPRINTNODES_API UK2Node_FoldingSkyStoryTyped : public UK2Node_BaseAsyncTask
{
	GENERATED_UCLASS_BODY()
#if WITH_EDITOR
public:
	virtual void GetNodeContextMenuActions(UToolMenu* Menu, UGraphNodeContextMenuContext* Context) const override;
	virtual FText GetTooltipText() const override;
protected:
	bool CanChangeToNodeType(EFoldingSkyStoryNodeType NewNodeType) const;
	void ChangeNodeType(EFoldingSkyStoryNodeType NewNodeType);
#endif // WITH_EDITOR
	UPROPERTY()
		int32 NumChoices;
	UPROPERTY()
		EFoldingSkyStoryNodeType NodeType;
	void SetNumChoices(const int32& NewValue);
	void SyncProxyClassAndFunctionName();
};

UCLASS(Abstract)
class FOLDINGSKYSTORYBLUEPRINTNODES_API UK2Node_FoldingSkyStoryWithChoices : public UK2Node_FoldingSkyStoryTyped, public IK2Node_AddPinInterface
{
	GENERATED_BODY()
#if WITH_EDITOR
public:
	virtual void AllocateDefaultPins() override;
	virtual void GetNodeContextMenuActions(UToolMenu* Menu, UGraphNodeContextMenuContext* Context) const override;
	virtual void ExpandNode(class FKismetCompilerContext& CompilerContext, UEdGraph* SourceGraph) override;
protected:
	bool HandleDelegateImplementation(FMulticastDelegateProperty* CurrentProperty, const TArray<UK2Node_BaseAsyncTask::FBaseAsyncTaskHelper::FOutputPinAndLocalVariable>& VariableOutputs,
		UEdGraphPin* ProxyObjectPin, UEdGraphPin*& InOutLastThenPin,
		UK2Node* CurrentNode, UEdGraph* SourceGraph, FKismetCompilerContext& CompilerContext);
	void AddChoicePins(const int32& Identifier);
	void RemoveInputPin(UEdGraphPin* Pin);
	void RemoveTargetPin(UEdGraphPin* PinToRemove);
	bool CanRemoveChoicePin(UEdGraphPin* Pin) const;
	void AddInputPin();
	void ClearChoicePins();
	virtual bool CanAddPin() const override;
	bool IsChoicePin(const UEdGraphPin* Pin) const;
	void SyncPinNames(const TArray<UEdGraphPin*>& ChoicePins, const TArray<UEdGraphPin*>& ExecPins);
	void GetChoiceAndExecPins(TArray<UEdGraphPin*>& ChoicePins, TArray<UEdGraphPin*>& ExecPins) const;
#endif // WITH_EDITOR
};
