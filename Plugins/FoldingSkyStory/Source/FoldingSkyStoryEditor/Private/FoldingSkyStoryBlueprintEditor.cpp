// Copyright Folding Sky Games LLC 2021 All rights reserved.

#include "FoldingSkyStoryBlueprintEditor.h"
#include "EditorReimportHandler.h"

#if WITH_EDITOR
#include "Editor.h"
#endif

#include "FoldingSkyStoryBlueprint.h"
#include "FoldingSkyStoryGraphSchema.h"
#include "Kismet2/BlueprintEditorUtils.h"

#define LOCTEXT_NAMESPACE "FFoldingSkyStoryBlueprintEditor"

FName FFoldingSkyStoryBlueprintEditor::GetToolkitFName() const
{
	return FName("FoldingSkyStoryBlueprintEditor");
}

FText FFoldingSkyStoryBlueprintEditor::GetBaseToolkitName() const
{
	return LOCTEXT("FoldingSkyStoryBlueprintEditorAppLabel", "Folding Sky Story Blueprint Editor");
}

FText FFoldingSkyStoryBlueprintEditor::GetToolkitName() const
{
	const TArray<UObject*>& EditingObjs = GetEditingObjects();

	check(EditingObjs.Num() > 0);

	FFormatNamedArguments Args;

	const UObject* EditingObject = EditingObjs[0];

	const bool bDirtyState = EditingObject->GetOutermost()->IsDirty();

	Args.Add(TEXT("ObjectName"), FText::FromString(EditingObject->GetName()));
	Args.Add(TEXT("DirtyState"), bDirtyState ? FText::FromString(TEXT("*")) : FText::GetEmpty());
	return FText::Format(LOCTEXT("StoryToolkitName", "{ObjectName}{DirtyState}"), Args);
}

FText FFoldingSkyStoryBlueprintEditor::GetToolkitToolTipText() const
{
	const UObject* EditingObject = GetEditingObject();

	check (EditingObject != NULL);

	return FAssetEditorToolkit::GetToolTipTextForObject(EditingObject);
}

FString FFoldingSkyStoryBlueprintEditor::GetWorldCentricTabPrefix() const
{
	return TEXT("Folding Sky Story Blueprint Editor");
}

FLinearColor FFoldingSkyStoryBlueprintEditor::GetWorldCentricTabColorScale() const
{
	return FLinearColor::White;
}

UBlueprint* FFoldingSkyStoryBlueprintEditor::GetBlueprintObj() const
{
	const TArray<UObject*>& EditingObjs = GetEditingObjects();
	for (int32 i = 0; i < EditingObjs.Num(); ++i)
	{
		if (EditingObjs[i]->IsA<UFoldingSkyStoryBlueprint>())
		{ 
			return (UBlueprint*)EditingObjs[i]; 
		}
	}
	return nullptr;
}

FString FFoldingSkyStoryBlueprintEditor::GetDocumentationLink() const
{
	return FBlueprintEditor::GetDocumentationLink();
}

#undef LOCTEXT_NAMESPACE

