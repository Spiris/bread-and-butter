// Copyright Folding Sky Games LLC 2021 All rights reserved.

#include "Story/StoryBlueprintFunctionLibrary.h"

#include "Sound/DialogueWave.h"
#include "Components/AudioComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Blueprint/UserWidget.h"

void UStoryBlueprintFunctionLibrary::PlayManagedDialogue(UObject* WorldContextObject, FText& Words, const TSoftObjectPtr<UDialogueWave>& SoftDialogue, const FDialogueContext& Context, UAudioComponent* Audio, USceneComponent* Anchor, bool bIs2D)
{
	
	if (UDialogueWave* Dialogue = SoftDialogue.LoadSynchronous())
	{
		Words = Dialogue->GetLocalizedSpokenText(Context);
		if (bIs2D)
		{
			UGameplayStatics::PlayDialogue2D(WorldContextObject, Dialogue, Context);
		}
		else
		{
			if (Audio)
			{
				if (USoundBase* Wave = Dialogue->GetWaveFromContext(Context))
				{
					Audio->SetSound(Wave);
					Audio->Play();
				}
				else
				{
					UE_LOG(LogTemp, Error, TEXT("No Sound Wave acquired from Dialogue Context."));
				}
			}
			else
			{
				const FVector& Loc = Anchor ? Anchor->GetComponentLocation() : FVector::ZeroVector;
				UGameplayStatics::PlayDialogueAtLocation(WorldContextObject, Dialogue, Context, Loc);
			}
		}
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("No Dialogue acquired from soft object pointer"));
	}
}

FVector2D  UStoryBlueprintFunctionLibrary::GetStickyWidgetScreenPosition(
	const FVector& AnchorPosition3D, const FVector2D& OldPosition, UUserWidget* Widget)
{
	FVector2D WidgetPosition = FVector2D::ZeroVector;
	if (APlayerController* Player = UGameplayStatics::GetPlayerController(Widget, 0))
	{
		int32 vx, vy;
		FVector2D AnchorPosition;
		Player->GetViewportSize(vx, vy);
		const FVector2D& ViewBounds = FVector2D(vx, vy);		
		UGameplayStatics::ProjectWorldToScreen(Player, AnchorPosition3D, AnchorPosition);
		const FVector2D& Target = AnchorPosition + FVector2D(0, -50);
		const FVector2D& StoryWidgetSize = Widget->GetDesiredSize();
		WidgetPosition = StickNearPosition(Target, OldPosition, 50.0f);
		WidgetPosition = LockInBounds(WidgetPosition, ViewBounds, StoryWidgetSize * 0.25f);
		ENearBounds NearX = IsNearlyInBounds(Target.X, 0, ViewBounds.X, StoryWidgetSize.X * 0.25f);
		if (NearX <	ENearBounds::NearlyUnder || NearX > ENearBounds::NearlyOver)
		{
			WidgetPosition.X = Target.X;
		}
		ENearBounds NearY = IsNearlyInBounds(Target.Y, 0, ViewBounds.Y, StoryWidgetSize.Y);
		if (NearY <	ENearBounds::NearlyUnder || NearY > ENearBounds::NearlyOver)
		{
			WidgetPosition.Y = Target.Y;
		}
	}
	return WidgetPosition;
}

FVector2D UStoryBlueprintFunctionLibrary::GetPositionOnScreen(UObject* WorldContextObject, const FVector& InLocation)
{
	FVector2D RetVal = FVector2D::ZeroVector;
	if (APlayerController* Player = UGameplayStatics::GetPlayerController(WorldContextObject, 0))
	{
		UGameplayStatics::ProjectWorldToScreen(Player, InLocation, RetVal);
	}
	return RetVal;
}
FVector2D UStoryBlueprintFunctionLibrary::LockInViewBounds(UObject* WorldContextObject, const FVector2D& InPosition, const FVector2D& Buffer)
{
	FVector2D RetVal = FVector2D::ZeroVector;
	if (APlayerController* Player = UGameplayStatics::GetPlayerController(WorldContextObject, 0))
	{
		int32 vx, vy;
		FVector2D AnchorScreenPosition;
		Player->GetViewportSize(vx, vy);
		const FVector2D& ViewBounds = FVector2D(vx, vy);
		RetVal = LockInBounds(InPosition, ViewBounds, Buffer);
	}
	return RetVal;
}
FVector2D UStoryBlueprintFunctionLibrary::LockInBounds(const FVector2D& InPosition, const FVector2D& ViewportBounds, const FVector2D& Buffer)
{
	FVector2D RetVal = InPosition;
	const ENearBounds NearX = IsNearlyInBounds(InPosition.X, 0.f, ViewportBounds.X, Buffer.X);
	if (NearX >= ENearBounds::NearlyOver)
	{
		RetVal.X = ViewportBounds.X - Buffer.X;
	}
	else if (NearX <= ENearBounds::NearlyUnder)
	{
		RetVal.X = Buffer.X;
	}
	const ENearBounds NearY = IsNearlyInBounds(InPosition.Y, 0.f, ViewportBounds.Y, Buffer.Y);
	if (NearY >= ENearBounds::NearlyOver)
	{
		RetVal.Y = ViewportBounds.Y - Buffer.Y;
	}
	else if (NearY <= ENearBounds::NearlyUnder)
	{
		RetVal.Y = Buffer.Y;
	}
	return RetVal;
}
FVector2D UStoryBlueprintFunctionLibrary::StickNearPosition(const FVector2D& TargetPosition, const FVector2D& InPosition, const float Tolerance)
{
	FVector2D RetVal = InPosition;
	float dsq = FVector2D::DistSquared(InPosition, TargetPosition);
	float tsq = Tolerance * Tolerance;
	if (dsq > tsq)
	{
		RetVal = InPosition + ((TargetPosition - InPosition) * 0.06f);
	}
	return RetVal;
}

ENearBounds UStoryBlueprintFunctionLibrary::
	IsNearlyInBounds(const float& Value, const float& MinBound, const float& MaxBound, const float& Tolerance)
{
	if (Value < MinBound + Tolerance)
	{
		if (Value > MinBound - Tolerance)
		{
			return ENearBounds::NearlyUnder;
		}
		if (Value < MinBound - Tolerance * 4.f)
		{
			return ENearBounds::FarUnder;
		}
		return ENearBounds::Under;
	}
	else if (Value > MaxBound - Tolerance)
	{
		if (Value < MaxBound + Tolerance)
		{
			return ENearBounds::NearlyOver;
		}
		if (Value > MaxBound + Tolerance * 4.f)
		{
			return ENearBounds::FarOver;
		}
		return ENearBounds::Over;
	}
	return ENearBounds::In;
}