// Copyright Folding Sky Games LLC 2021 All rights reserved.

#include "Story/Widgets/StoryWidgetHandler.h"

#include "Components/WidgetComponent.h"

void UStoryWidgetHandler::SetAnchorComponent(USceneComponent* Anchor)
{
	if (Anchor != AnchorComponent)
	{
		CloseWidget();
	}
	AnchorComponent = Anchor;
}

void UStoryWidgetHandler::CloseWidget()
{
	if (bIsProjectingToScreen)
	{
		if (UUserWidget* Widget = GetWidget())
		{
			if (Widget->IsInViewport())
			{
				Widget->RemoveFromViewport();
			}
		}
	}
	else if (UWidgetComponent* Component = GetWidgetComponent())
	{
		Component->SetVisibility(false);
	}
	OnStoryWidgetClose.Broadcast();
	OnStoryWidgetClose_BP();
}


void UStoryWidgetHandler::OpenWidget()
{
	
	if (bIsProjectingToScreen)
	{
		if (UUserWidget* Widget = GetWidget())
		{
			if (!Widget->IsInViewport())
			{
				Widget->AddToViewport();
			}
		}
	}
	else if(UWidgetComponent * Component = GetWidgetComponent())
	{
		Component->SetVisibility(true);
	}
	OnStoryWidgetOpen.Broadcast();
	OnStoryWidgetOpen_BP();
}


UWidgetComponent* UStoryWidgetHandler::GetWidgetComponent() const
{
	if (bIsProjectingToScreen)
		return nullptr;
	return StoryWidgetComponent;
}

UUserWidget* UStoryWidgetHandler::GetWidget() const
{
	return UserWidget;
}
USceneComponent* UStoryWidgetHandler::GetAnchorComponent() const
{
	return AnchorComponent;
}

void UStoryWidgetHandler::InitStoryWidget(APlayerController* Player, TSubclassOf<UUserWidget> InUserWidgetClass)
{	
	if (UserWidget)
		return;
	check(Player);
	if (bIsProjectingToScreen)
	{
		UserWidget = CreateWidget<UUserWidget>(Player, InUserWidgetClass);
	}
	else
	{
		UWidgetComponent* Component = GetWidgetComponent();
		if (!Component)
		{
			Component = Cast<UWidgetComponent>(Player->AddComponentByClass(UWidgetComponent::StaticClass(), false, FTransform::Identity, false));
		}
		Component->SetOwnerPlayer(Player->GetLocalPlayer());
		Component->SetWidgetClass(InUserWidgetClass);
		Component->InitWidget();
		UserWidget = Component->GetWidget();
	}
	if (UserWidget)
	{
		UserWidget->SetOwningPlayer(Player);
	}
}

void UStoryWidgetHandler::HandleStoryPosted()
{
	if (USceneComponent* Anchor = GetAnchorComponent())
	{
		const FVector& AnchorLocation = Anchor->GetComponentLocation();
		if (bIsProjectingToScreen && !bIsFullscreen)
		{
			OnStoryPositionInit.Broadcast(AnchorLocation);
			OnStoryPositionInit_BP(AnchorLocation);
			ProjectStoryToScreen();
		}
		if (UWidgetComponent* Component = GetWidgetComponent())
		{
			Component->SetWorldLocation(AnchorLocation);
			Component->SetVisibility(true);
			if (UUserWidget* Widget = Component->GetWidget())
			{
				Component->SetDrawSize(Widget->GetDesiredSize());
			}
		}
	}
	OpenWidget();
}
void UStoryWidgetHandler::ProjectStoryToScreen()
{
	if (USceneComponent* Anchor = GetAnchorComponent())
	{
		const FVector& AnchorLocation = Anchor->GetComponentLocation();
		OnStoryPositionUpdate.Broadcast(AnchorLocation);
		OnStoryPositionUpdate_BP(AnchorLocation);
	}
}


void UStoryWidgetHandler::Tick(float DeltaTime)
{
	if (bIsProjectingToScreen && !bIsFullscreen)
	{
		ProjectStoryToScreen();
	}
}
TStatId UStoryWidgetHandler::GetStatId() const
{
	RETURN_QUICK_DECLARE_CYCLE_STAT(UStoryWidgetHandler, STATGROUP_Tickables);
}