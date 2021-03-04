// Copyright Folding Sky Games LLC 2021 All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Editor/Kismet/Public/BlueprintEditor.h"

class FFoldingSkyStoryBlueprintEditor : public FBlueprintEditor
{
public:
	FFoldingSkyStoryBlueprintEditor() { };
	virtual ~FFoldingSkyStoryBlueprintEditor() { };
public:
	// IToolkit interface
	virtual FName GetToolkitFName() const override;
	virtual FText GetBaseToolkitName() const override;
	virtual FText GetToolkitName() const override;
	virtual FText GetToolkitToolTipText() const override;
	virtual FString GetWorldCentricTabPrefix() const override;
	virtual FLinearColor GetWorldCentricTabColorScale() const override;
	// End of IToolkit interface

	/** @return the documentation location for this editor */
	virtual FString GetDocumentationLink() const override;
	
	/** Returns a pointer to the Blueprint object we are currently editing, as long as we are editing exactly one */
	virtual UBlueprint* GetBlueprintObj() const override;
};
