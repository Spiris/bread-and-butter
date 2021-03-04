// Copyright Folding Sky Games LLC 2021 All rights reserved.

#include "Story/PlayerStoryComponent.h"

#include "Components/InputComponent.h"
#include "GameFramework/Actor.h"
#include "GameFramework/PlayerController.h"
#include "FoldingSkyStoryComponent.h"


UPlayerStoryComponent* UPlayerStoryComponent::GetFromPlayer(APlayerController* Player)
{
	UPlayerStoryComponent* comp = Player->FindComponentByClass<UPlayerStoryComponent>();
	if(!comp)
	{
		comp = Cast<UPlayerStoryComponent>(Player->AddComponentByClass(UPlayerStoryComponent::StaticClass(), false, FTransform::Identity, false));
		comp->OwningPlayer = Player;
	}
	return comp;
}
UPlayerStoryComponent::UPlayerStoryComponent()
{
	bIsInputBound = false;
	FInputActionBinding AcceptBinding = FInputActionBinding(TEXT("StoryAccept"), EInputEvent::IE_Released);
	AcceptBinding.ActionDelegate.BindDelegate(this, &UPlayerStoryComponent::OnStoryAccept);
	FInputActionBinding CancelBinding = FInputActionBinding(TEXT("StoryCancel"), EInputEvent::IE_Released);
	CancelBinding.ActionDelegate.BindDelegate(this, &UPlayerStoryComponent::OnStoryCancel);
	FInputActionBinding NextBinding = FInputActionBinding(TEXT("StoryNextChoice"), EInputEvent::IE_Released);
	NextBinding.ActionDelegate.BindDelegate(this, &UPlayerStoryComponent::OnStoryNextChoice);
	FInputActionBinding PrevBinding = FInputActionBinding(TEXT("StoryPrevChoice"), EInputEvent::IE_Released);
	PrevBinding.ActionDelegate.BindDelegate(this, &UPlayerStoryComponent::OnStoryPrevChoice);
	ActionBindings.Add(AcceptBinding);
	ActionBindings.Add(CancelBinding);
	ActionBindings.Add(NextBinding);
	ActionBindings.Add(PrevBinding);
}
void UPlayerStoryComponent::SetEngagedComponent(UFoldingSkyStoryComponent* InComponent)
{
	EngagedComponent = InComponent;
}
void UPlayerStoryComponent::Engage(UFoldingSkyStoryComponent* StoryComponent)
{
	check(StoryComponent)
	SetEngagedComponent(StoryComponent);
	if (UFoldingSkyStoryComponent* Story = GetEngagedStory())
	{
		//Story->OnPlayerDisengaged.AddUniqueDynamic(this, &UPlayerStoryComponent::DisengageStory);
		if (APlayerController* Player = GetOwningPlayer())
		{
			SetBindings(Player->InputComponent, true);
		}
	}
}

void UPlayerStoryComponent::Disengage()
{
	if (APlayerController* Player = GetOwningPlayer())
	{
		//check(DisengagingPlayer == Player);

		SetBindings(Player->InputComponent, false);
		if (UFoldingSkyStoryComponent* Story = GetEngagedStory())
		{
			//Story->OnPlayerDisengaged.RemoveAll(this);
			SetEngagedComponent(nullptr);
		}
	}
}

void UPlayerStoryComponent::SetBindings(UInputComponent* InputComponent, bool bBindOrUnbind)
{
	if (InputComponent)
	{
		if (bBindOrUnbind)
		{
			if (!bIsInputBound)
			{
				for (FInputActionBinding& b : ActionBindings)
				{
					b = InputComponent->AddActionBinding(b);
				}
				bIsInputBound = true;
			}
		}
		else if (bIsInputBound)
		{
			for (const FInputActionBinding& b : ActionBindings)
			{
				InputComponent->RemoveActionBindingForHandle(b.GetHandle());
			}
			bIsInputBound = false;
		}
	}	
}

void UPlayerStoryComponent::OnStoryAccept()
{
	OnInputAccept.Broadcast();
}
void UPlayerStoryComponent::OnStoryCancel()
{
	OnInputCancel.Broadcast();
}
void UPlayerStoryComponent::OnStoryNextChoice()
{
	OnInputNext.Broadcast();
}
void UPlayerStoryComponent::OnStoryPrevChoice()
{
	OnInputPrev.Broadcast();
}