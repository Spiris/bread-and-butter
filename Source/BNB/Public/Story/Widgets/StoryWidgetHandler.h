// Copyright Folding Sky Games LLC 2021 All rights reserved.

#pragma once

#include "CoreMinimal.h"

#include "Story/StoryStructs.h"
#include "Tickable.h"
#include "StoryWidgetHandler.generated.h"

class USceneComponent;
class UWidgetComponent;
class UUserWidget;
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnStoryVectorEvent, FVector, Vector);

UCLASS(BlueprintType)
class BNB_API UStoryWidgetHandler : public UObject, public FTickableGameObject
{
	GENERATED_BODY()
public:
	UFUNCTION(BlueprintCallable)
		void InitStoryWidget(APlayerController* Player, TSubclassOf<UUserWidget> WidgetClassOverride);
	UFUNCTION(BlueprintCallable)
		void SetAnchorComponent(USceneComponent* Anchor);
	UFUNCTION(BlueprintCallable)
		void HandleStoryPosted();
	UFUNCTION(BlueprintCallable)
		void OpenWidget();
	UFUNCTION(BlueprintCallable)
		void CloseWidget();
	UFUNCTION(BlueprintPure)
		UWidgetComponent* GetWidgetComponent() const;
	UFUNCTION(BlueprintPure)
		UUserWidget* GetWidget() const;
	UFUNCTION(BlueprintPure)
		USceneComponent* GetAnchorComponent() const;

	UPROPERTY(BlueprintAssignable)
		FOnStoryActionEvent OnStoryWidgetOpen;
	UPROPERTY(BlueprintAssignable)
		FOnStoryActionEvent OnStoryWidgetClose;
	UPROPERTY(BlueprintAssignable)
		FOnStoryVectorEvent OnStoryPositionUpdate;
	UPROPERTY(BlueprintAssignable)
		FOnStoryVectorEvent OnStoryPositionInit;
protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (ExposeOnSpawn = true))
		bool bIsProjectingToScreen = false;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (ExposeOnSpawn = true))
		bool bIsFullscreen = false;
	UPROPERTY()
		USceneComponent* AnchorComponent = nullptr;
	UPROPERTY()
		UWidgetComponent* StoryWidgetComponent = nullptr;
	UPROPERTY()
		UUserWidget* UserWidget = nullptr;

	// maybe not needed, a good time to show your widgets
	UFUNCTION(BlueprintImplementableEvent, Category = Story, meta = (DisplayName = "On Story Widget Opened"))
		void OnStoryWidgetOpen_BP();
	// maybe not needed, a good time to hide your widgets
	UFUNCTION(BlueprintImplementableEvent, Category = Story, meta = (DisplayName = "On Story Widget Closed"))
		void OnStoryWidgetClose_BP();
	// called when the widget being shown should intialize its position (if it is being shown in world space or following world space in the viewport)
	UFUNCTION(BlueprintImplementableEvent, Category = Story, meta = (DisplayName = "On Story Position Init"))
		void OnStoryPositionInit_BP(const FVector& AnchorLocation);
	// called when the widget being shown should receive an update of the its position (if it is being shown in world space or following world space in the viewport)
	UFUNCTION(BlueprintImplementableEvent, Category = Story, meta = (DisplayName = "On Story Position Updated"))
		void OnStoryPositionUpdate_BP(const FVector& AnchorLocation);
	
	void ProjectStoryToScreen();

	virtual void Tick(float DeltaTime) override;
	virtual TStatId GetStatId() const override;
};