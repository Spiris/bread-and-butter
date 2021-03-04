// Copyright Folding Sky Games LLC 2021 All rights reserved.

#pragma once

#include "CoreMinimal.h"

#include "Story/StoryStructs.h"
#include "Sound/DialogueTypes.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "StoryBlueprintFunctionLibrary.generated.h"

enum class ENearBounds : uint8
{
	FarUnder = 0,
	Under = 1,
	NearlyUnder = 2,
	In = 3,
	NearlyOver = 4,
	Over = 5,
	FarOver = 6
};
class UUserWidget;
class UDialogueWave;
class UAudioComponent;
UCLASS()
class BNB_API UStoryBlueprintFunctionLibrary : public UBlueprintFunctionLibrary
{
    GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, meta = (WorldContext = "WorldContextObject"))
		static void PlayManagedDialogue(UObject* WorldContextObject, UPARAM(ref) FText& Words, const TSoftObjectPtr<UDialogueWave>& SoftDialogue, const FDialogueContext& Context, UAudioComponent* Audio, USceneComponent* Anchor, bool bIs2D = false);
    UFUNCTION(BlueprintCallable)
        static FVector2D GetStickyWidgetScreenPosition(const FVector& AnchorPosition3D, const FVector2D& OldPosition, UUserWidget* Widget);
	UFUNCTION(BlueprintCallable)
		static FVector2D LockInViewBounds(UObject* WorldContextObject, const FVector2D& InPosition, const FVector2D& Buffer);
	static FVector2D GetPositionOnScreen(UObject* WorldContextObject, const FVector& InLocation);
protected:
    static ENearBounds IsNearlyInBounds(const float& Value, const float& MinBound, const float& MaxBound, const float& Tolerance);
	static FVector2D LockInBounds(const FVector2D& InPosition, const FVector2D& ViewportBounds, const FVector2D& Buffer);
	static FVector2D StickNearPosition(const FVector2D& TargetPosition, const FVector2D& InPosition, const float Tolerance);
};