// Copyright Folding Sky Games LLC 2021 All rights reserved.

#include "Story/Widgets/StoryWidget.h"

#include "Story/Widgets/StoryChoiceWidget.h"
#include "Story/Widgets/StoryTextBlock.h"
#include "Story/Widgets/StoryWrapBox.h"
#include "Story/StoryBlueprintFunctionLibrary.h"

#include "Components/Image.h"
#include "Components/ScrollBox.h"
#include "Components/TextBlock.h"

void UStoryWidget::NativeConstruct()
{
	Super::NativeConstruct();
}

void UStoryWidget::SetPortrait(TSoftObjectPtr<UTexture2D> Image)
{
	StoryImage = Image;
	if (UImage* Container = GetPortraitContainer())
	{
		if(!StoryImage.IsNull())
		{
			Container->SetBrushFromSoftTexture(StoryImage);
		}
		else if(!DefaultUnknownImage.IsNull())
		{
			Container->SetBrushFromSoftTexture(DefaultUnknownImage);
		}
	}
}
void UStoryWidget::SetChoices(const TArray<FText>& Choices)
{
	StoryChoices = Choices;
	StoryChoiceWidgets.Empty();
	if (UScrollBox* ChoiceContainer = GetChoiceContainer())
	{
		ChoiceContainer->ClearChildren();
		for (const FText& ChoiceText : StoryChoices)
		{
			UStoryChoiceWidget* ChoiceWidget = CreateWidget<UStoryChoiceWidget>(this, StoryChoiceClass);
			ChoiceContainer->AddChild(ChoiceWidget);
			StoryChoiceWidgets.Add(ChoiceWidget);
			ChoiceWidget->SetStoryChoiceText(ChoiceText);
			ChoiceWidget->SetSelectionState(false);	
		}
	}
	SelectChoice(0);
}

void UStoryWidget::ChangeChoice(int32 Modifier)
{
	SelectChoice(GetCurrentChoice() + Modifier);
}
void UStoryWidget::SelectChoice(int32 NewChoice)
{
	NewChoice = StoryChoiceWidgets.Num() > 1 ? WrapIntInclusive(NewChoice, 0, StoryChoiceWidgets.Num() - 1) : 0;
	if (UScrollBox* ChoiceContainer = GetChoiceContainer())
	{
		for (UStoryChoiceWidget* w : StoryChoiceWidgets)
		{
			w->SetSelectionState(false);
		}
		if (StoryChoiceWidgets.IsValidIndex(NewChoice))
		{
			StoryChoiceWidgets[NewChoice]->SetSelectionState(true);
			ChoiceContainer->ScrollWidgetIntoView(StoryChoiceWidgets[NewChoice], true, EDescendantScrollDestination::Center, 5);
		}
	}
	SelectedChoice = NewChoice;
}
void UStoryWidget::SetStoryContent(const FStoryPoint& StoryPoint, bool bAutoResize)
{
	NameText = StoryPoint.SpeakerName;
	SetStoryText(StoryPoint.Text, bAutoResize);	
	SetChoices(StoryPoint.Choices);
	SetPortrait(StoryPoint.Image);
}
FVector2D UStoryWidget::GetSizeFromText()
{
	FVector2D DesiredSize = FVector2D::ZeroVector;
	if (UStoryTextBlock* Container = GetTextContainer())
	{
		DesiredSize = Container->GetDesiredSize();
	}
	FVector2D WrapBoxSize = FVector2D::ZeroVector;
	if (UStoryWrapBox* WrapBox = GetWrapBox())
	{
		WrapBoxSize = WrapBox->GetDesiredSize();
	}
	DesiredSize = WrapBoxSize > DesiredSize ? WrapBoxSize : DesiredSize;
	return FVector2D(DesiredSize.X + Buffer, DesiredSize.Y + Buffer);
}

void UStoryWidget::SetStoryText(const FText& InText, bool bAutoResize)
{
	StoryText = InText;
	if (UStoryWrapBox* StoryBox = GetWrapBox())
	{
		StoryBox->WriteAnimated(StoryText.ToString());
	}	
	if (bAutoResize)
	{
		const FVector2D& NewSize = GetSizeFromText();
		SetMinimumDesiredSize(NewSize);
		SetDesiredSizeInViewport(NewSize);
	}
}

int32 UStoryWidget::WrapIntInclusive(int32 Value, int32 Min, int32 Max) const
{
	int32 retVal = Value;
	if (Value < Min)
	{
		retVal = Max;
	}
	else if (Value > Max)
	{
		retVal = Min;
	}
	return retVal;
}

void UStoryWidget::SetScreenPosition(const FVector2D& Target, const FVector2D& TailTarget)
{
	SetPositionInViewport(Target);
	const FVector2D& ds = GetMinimumDesiredSize();	
	const FVector2D& BottomMid = FVector2D(ds.X * 0.5f, ds.Y - 3);
	const FVector2D& Direction = (TailTarget - Target);
	TailBorderVecs.Empty(); TailInsideVecs.Empty();
	if (Direction.Y > 5)
	{
		float Width = 40;// Normal.X > 0 ? FMath::Clamp<float>(Normal.X, 30, ds.X * 0.2f) : FMath::Clamp<float>(Normal.X, ds.X * -0.2f, -30);
		// Outline Tail
		TailBorderVecs.Add(FVector2D(BottomMid.X - Width, BottomMid.Y));
		TailBorderVecs.Add(BottomMid + Direction);
		TailBorderVecs.Add(FVector2D(BottomMid.X + Width, BottomMid.Y));	

		// Fill Tail
		TailTip = BottomMid + Direction * 0.8f;
		const float a = BottomMid.X - Width;
		const float b = BottomMid.X + Width;
		const int32 num = FMath::Abs(b - a) * 0.2f;
		for (int32 i = 1; i < num; i++)
		{
			const float alpha = (float)i / (float)num;
			const float x = FMath::Lerp(a, b, alpha);
			const FVector2D& PointOnLine = FVector2D(x, BottomMid.Y);
			TailInsideVecs.Add(PointOnLine);
		}
	}
	
}

FText UStoryWidget::GetCurrentChoiceText() const
{
	FText RetVal = FText::GetEmpty();
	if (StoryChoices.IsValidIndex(GetCurrentChoice()))
	{
		RetVal = StoryChoices[GetCurrentChoice()];
	}
	return RetVal;
}

void UStoryWidget::InitPosition(const FVector& AnchorLocation)
{
	const FVector2D& AnchorPosition = UStoryBlueprintFunctionLibrary::GetPositionOnScreen(this, AnchorLocation);
	const FVector2D& AboveAnchorPosition = AnchorPosition + FVector2D(0, -50);
	const FVector2D& BoundaryBuffer = GetDesiredSize() * 0.25f;
	ViewportPosition = UStoryBlueprintFunctionLibrary::LockInViewBounds(this, AboveAnchorPosition, BoundaryBuffer);
	SetAlignmentInViewport(FVector2D(0.5f, 0.5f));
	SetScreenPosition(ViewportPosition, AnchorPosition);
}

void UStoryWidget::UpdatePosition(const FVector& AnchorLocation)
{	
	const FVector2D& AnchorScreenPosition = UStoryBlueprintFunctionLibrary::GetPositionOnScreen(this, AnchorLocation);
	ViewportPosition = UStoryBlueprintFunctionLibrary::GetStickyWidgetScreenPosition(AnchorLocation, ViewportPosition, this);
	SetScreenPosition(ViewportPosition, AnchorScreenPosition);
}