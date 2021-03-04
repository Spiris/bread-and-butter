// Copyright Folding Sky Games LLC 2021 All rights reserved.

#include "FoldingSkyStoryBlueprintFactory.h"
#include "InputCoreTypes.h"
#include "UObject/Interface.h"
#include "Layout/Visibility.h"
#include "Input/Reply.h"
#include "Widgets/SWidget.h"
#include "Widgets/DeclarativeSyntaxSupport.h"
#include "Misc/MessageDialog.h"
#include "Modules/ModuleManager.h"
#include "Widgets/SCompoundWidget.h"
#include "Widgets/SBoxPanel.h"
#include "Widgets/SWindow.h"
#include "Widgets/Layout/SBorder.h"
#include "Widgets/Text/STextBlock.h"
#include "Widgets/Layout/SBox.h"
#include "Widgets/Layout/SUniformGridPanel.h"
#include "Widgets/Input/SButton.h"
#include "EditorStyleSet.h"
#include "Editor.h"
#include "EdGraphSchema_K2.h"

#include "ClassViewerModule.h"
#include "Kismet2/BlueprintEditorUtils.h"
#include "Engine/BlueprintGeneratedClass.h"
#include "Kismet2/KismetEditorUtilities.h"
#include "BlueprintEditorSettings.h"

#include "FoldingSkyStoryComponent.h"
#include "FoldingSkyStoryBlueprint.h"
#include "FoldingSkyStoryGraph.h"
#include "FoldingSkyStoryGraphSchema.h"

#include "ClassViewerFilter.h"

#include "SlateOptMacros.h"

#define LOCTEXT_NAMESPACE "UFoldingSkyStoryBlueprintFactory"

BEGIN_SLATE_FUNCTION_BUILD_OPTIMIZATION

class SFoldingSkyStoryBlueprintCreateDialog : public SCompoundWidget
{
public:
	SLATE_BEGIN_ARGS(SFoldingSkyStoryBlueprintCreateDialog){}

	SLATE_END_ARGS()

	void Construct(const FArguments& InArgs)
	{
		bOkClicked = false;
		ParentClass = UFoldingSkyStoryComponent::StaticClass();

		ChildSlot
			[
				SNew(SBorder)
				.Visibility(EVisibility::Visible)
				.BorderImage(FEditorStyle::GetBrush("Menu.Background"))
				[
					SNew(SBox)
					.Visibility(EVisibility::Visible)
					.WidthOverride(500.0f)
					[
						SNew(SVerticalBox)
						+ SVerticalBox::Slot()
						.FillHeight(1)
						[
							SNew(SBorder)
							.BorderImage(FEditorStyle::GetBrush("ToolPanel.GroupBorder"))
							.Content()
							[
								SAssignNew(ParentClassContainer, SVerticalBox)
							]
						]

						// Ok/Cancel buttons
						+ SVerticalBox::Slot()
							.AutoHeight()
							.HAlign(HAlign_Right)
							.VAlign(VAlign_Bottom)
							.Padding(8)
							[
								SNew(SUniformGridPanel)
								.SlotPadding(FEditorStyle::GetMargin("StandardDialog.SlotPadding"))
								.MinDesiredSlotWidth(FEditorStyle::GetFloat("StandardDialog.MinDesiredSlotWidth"))
								.MinDesiredSlotHeight(FEditorStyle::GetFloat("StandardDialog.MinDesiredSlotHeight"))
								+ SUniformGridPanel::Slot(0, 0)
								[
									SNew(SButton)
									.HAlign(HAlign_Center)
									.ContentPadding(FEditorStyle::GetMargin("StandardDialog.ContentPadding"))
									.OnClicked(this, &SFoldingSkyStoryBlueprintCreateDialog::OkClicked)
									.Text(LOCTEXT("CreateFoldingSkyStoryBlueprintOk", "OK"))
								]
								+ SUniformGridPanel::Slot(1, 0)
									[
										SNew(SButton)
										.HAlign(HAlign_Center)
										.ContentPadding(FEditorStyle::GetMargin("StandardDialog.ContentPadding"))
										.OnClicked(this, &SFoldingSkyStoryBlueprintCreateDialog::CancelClicked)
										.Text(LOCTEXT("CreateFoldingSkyStoryBlueprintCancel", "Cancel"))
									]
							]
					]
				]
			];

		MakeParentClassPicker();
	}

	bool ConfigureProperties(TWeakObjectPtr<UFoldingSkyStoryBlueprintFactory> InFoldingSkyStoryBlueprintFactory)
	{
		FoldingSkyStoryBlueprintFactory = InFoldingSkyStoryBlueprintFactory;

		TSharedRef<SWindow> Window = SNew(SWindow)
			.Title(LOCTEXT("CreateFoldingSkyStoryBlueprintOptions", "Create Folding Sky Story Blueprint"))
			.ClientSize(FVector2D(400, 700))
			.SupportsMinimize(false).SupportsMaximize(false)
			[
				AsShared()
			];

		PickerWindow = Window;

		GEditor->EditorAddModalWindow(Window);
		FoldingSkyStoryBlueprintFactory.Reset();

		return bOkClicked;
	}

private:
	class FFoldingSkyStoryBlueprintParentFilter : public IClassViewerFilter
	{
	public:
		TSet< const UClass* > AllowedChildrenOfClasses;

		FFoldingSkyStoryBlueprintParentFilter() {}

		virtual bool IsClassAllowed(const FClassViewerInitializationOptions& InInitOptions, const UClass* InClass, TSharedRef< FClassViewerFilterFuncs > InFilterFuncs) override
		{
			return InFilterFuncs->IfInChildOfClassesSet(AllowedChildrenOfClasses, InClass) != EFilterReturn::Failed;
		}

		virtual bool IsUnloadedClassAllowed(const FClassViewerInitializationOptions& InInitOptions, const TSharedRef< const IUnloadedBlueprintData > InUnloadedClassData, TSharedRef< FClassViewerFilterFuncs > InFilterFuncs) override
		{
			return InFilterFuncs->IfInChildOfClassesSet(AllowedChildrenOfClasses, InUnloadedClassData) != EFilterReturn::Failed;
		}
	};
	void MakeParentClassPicker()
	{
		FClassViewerModule& ClassViewerModule = FModuleManager::LoadModuleChecked<FClassViewerModule>("ClassViewer");
		FClassViewerInitializationOptions Options;
		Options.Mode = EClassViewerMode::ClassPicker;
		Options.bIsBlueprintBaseOnly = true;
		TSharedPtr<FFoldingSkyStoryBlueprintParentFilter> Filter = MakeShareable(new FFoldingSkyStoryBlueprintParentFilter);
		Filter->AllowedChildrenOfClasses.Add(UFoldingSkyStoryComponent::StaticClass());
		Options.ClassFilter = Filter;
		ParentClassContainer->ClearChildren();
		ParentClassContainer->AddSlot()
			.AutoHeight()
			[
				SNew(STextBlock)
				.Text(LOCTEXT("ParentClass", "Parent Class:"))
				.ShadowOffset(FVector2D(1.0f, 1.0f))
			];

		ParentClassContainer->AddSlot()
			[
				ClassViewerModule.CreateClassViewer(Options, FOnClassPicked::CreateSP(this, &SFoldingSkyStoryBlueprintCreateDialog::OnClassPicked))
			];
	}
	void OnClassPicked(UClass* ChosenClass)
	{
		ParentClass = ChosenClass;
	}
	FReply OkClicked()
	{
		if (FoldingSkyStoryBlueprintFactory.IsValid())
		{
			FoldingSkyStoryBlueprintFactory->BlueprintType = BPTYPE_Normal;
			FoldingSkyStoryBlueprintFactory->ParentClass = ParentClass.Get();
		}

		CloseDialog(true);

		return FReply::Handled();
	}

	void CloseDialog(bool bWasPicked = false)
	{
		bOkClicked = bWasPicked;
		if (PickerWindow.IsValid())
		{
			PickerWindow.Pin()->RequestDestroyWindow();
		}
	}
	FReply CancelClicked()
	{
		CloseDialog();
		return FReply::Handled();
	}

	FReply OnKeyDown(const FGeometry& MyGeometry, const FKeyEvent& InKeyEvent)
	{
		if (InKeyEvent.GetKey() == EKeys::Escape)
		{
			CloseDialog();
			return FReply::Handled();
		}
		return SWidget::OnKeyDown(MyGeometry, InKeyEvent);
	}

private:
	TWeakObjectPtr<UFoldingSkyStoryBlueprintFactory> FoldingSkyStoryBlueprintFactory;
	TWeakPtr<SWindow> PickerWindow;
	TSharedPtr<SVerticalBox> ParentClassContainer;
	TWeakObjectPtr<UClass> ParentClass;
	bool bOkClicked;
};
END_SLATE_FUNCTION_BUILD_OPTIMIZATION

UFoldingSkyStoryBlueprintFactory::UFoldingSkyStoryBlueprintFactory(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	bCreateNew = true;
	bEditAfterNew = true;
	SupportedClass = UFoldingSkyStoryBlueprint::StaticClass();
	ParentClass = UFoldingSkyStoryComponent::StaticClass();
}

uint32 UFoldingSkyStoryBlueprintFactory::GetMenuCategories() const
{
	// Blueprints category in the Editor
	return 1 << 8;
}
FText UFoldingSkyStoryBlueprintFactory::GetDisplayName() const
{
	return LOCTEXT("FoldingSkyStoryBlueprintDisplayName", "Story Blueprint");
}

bool UFoldingSkyStoryBlueprintFactory::ConfigureProperties()
{
	TSharedRef<SFoldingSkyStoryBlueprintCreateDialog> Dialog = SNew(SFoldingSkyStoryBlueprintCreateDialog);
	return Dialog->ConfigureProperties(this);
};

UObject* UFoldingSkyStoryBlueprintFactory::FactoryCreateNew(UClass* Class, UObject* InParent, FName Name, EObjectFlags Flags, UObject* Context, FFeedbackContext* Warn, FName CallingContext)
{
	check(Class->IsChildOf(UFoldingSkyStoryBlueprint::StaticClass()));
	if (BlueprintType == BPTYPE_Interface)
	{
		ParentClass = UObject::StaticClass();
	}
	if ((ParentClass == nullptr) || !FKismetEditorUtilities::CanCreateBlueprintOfClass(ParentClass) || !ParentClass->IsChildOf(UFoldingSkyStoryComponent::StaticClass()))
	{
		FFormatNamedArguments Args;
		Args.Add( TEXT("ClassName"), (ParentClass != NULL) ? FText::FromString( ParentClass->GetName() ) : LOCTEXT("Null", "(null)") );
		FMessageDialog::Open( EAppMsgType::Ok, FText::Format( LOCTEXT("CannotCreateFoldingSkyStoryBlueprint", "Cannot create a Story Blueprint based on the class '{ClassName}'."), Args ) );
		return NULL;
	}
	else
	{
		UFoldingSkyStoryBlueprint* NewBP = CastChecked<UFoldingSkyStoryBlueprint>(FKismetEditorUtilities::CreateBlueprint(ParentClass, InParent, Name, BlueprintType, UFoldingSkyStoryBlueprint::StaticClass(), UBlueprintGeneratedClass::StaticClass(), CallingContext));
		if (NewBP)
		{
			const UEdGraphSchema_K2* K2Schema = GetDefault<UEdGraphSchema_K2>();
			UEdGraph* NewGraph = FBlueprintEditorUtils::CreateNewGraph(NewBP, TEXT("Story Graph"), UFoldingSkyStoryGraph::StaticClass(), UFoldingSkyStoryGraphSchema::StaticClass());
			FBlueprintEditorUtils::AddUbergraphPage(NewBP, NewGraph);
			NewBP->LastEditedDocuments.Add(NewGraph);
			NewGraph->bAllowDeletion = true;

			UBlueprintEditorSettings* Settings = GetMutableDefault<UBlueprintEditorSettings>();
			if (Settings && Settings->bSpawnDefaultBlueprintNodes)
			{
				int32 NodePositionY = 0;
				FKismetEditorUtilities::AddDefaultEventNode(NewBP, NewGraph, FName(TEXT("OnStoryBeginGraph_BP")), UFoldingSkyStoryComponent::StaticClass(), NodePositionY);
				FKismetEditorUtilities::AddDefaultEventNode(NewBP, NewGraph, FName(TEXT("OnStoryPosted_BP")), UFoldingSkyStoryComponent::StaticClass(), NodePositionY);
				FKismetEditorUtilities::AddDefaultEventNode(NewBP, NewGraph, FName(TEXT("OnStoryChoiceAccepted_BP")), UFoldingSkyStoryComponent::StaticClass(), NodePositionY);
			}
		}
		return NewBP;
	}
}

UObject* UFoldingSkyStoryBlueprintFactory::FactoryCreateNew(UClass* Class, UObject* InParent, FName Name, EObjectFlags Flags, UObject* Context, FFeedbackContext* Warn)
{
	return FactoryCreateNew(Class, InParent, Name, Flags, Context, Warn, NAME_None);
}
#undef LOCTEXT_NAMESPACE