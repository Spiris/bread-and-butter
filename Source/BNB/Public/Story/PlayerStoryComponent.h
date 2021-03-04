// Copyright Folding Sky Games LLC 2021 All rights reserved.

#pragma once

#include "CoreMinimal.h"

//#include "Components/InputComponent.h"
#include "Components/ActorComponent.h"
#include "PlayerStoryComponent.generated.h"

class APlayerController;
class UFoldingSkyStoryComponent;

struct FInputBinding;
struct FInputActionBinding;
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnStoryInput);

UCLASS()
class BNB_API UPlayerStoryComponent : public UActorComponent
{
	GENERATED_BODY()
public:
	UFUNCTION(BlueprintPure, meta = (DisplayName = "Get Player Story Component"))
		static UPlayerStoryComponent* GetFromPlayer(APlayerController* Player);
	UPlayerStoryComponent();
	APlayerController* GetOwningPlayer() const { return OwningPlayer; };
	UFoldingSkyStoryComponent* GetEngagedStory() const { return EngagedComponent; };
	UFUNCTION(BlueprintCallable)
		void Engage(UFoldingSkyStoryComponent* StoryComponent);
	UFUNCTION(BlueprintCallable)
		void Disengage();
	UPROPERTY(BlueprintAssignable)
		FOnStoryInput OnInputAccept;
	UPROPERTY(BlueprintAssignable)
		FOnStoryInput OnInputCancel;
	UPROPERTY(BlueprintAssignable)
		FOnStoryInput OnInputNext;
	UPROPERTY(BlueprintAssignable)
		FOnStoryInput OnInputPrev;

protected:
	void SetEngagedComponent(UFoldingSkyStoryComponent* InComponent);	
	void SetBindings(UInputComponent* InputComponent, bool bBindOrUnbind);
	UFUNCTION()
		void OnStoryAccept();
	UFUNCTION()
		void OnStoryCancel();
	UFUNCTION()
		void OnStoryNextChoice();
	UFUNCTION()
		void OnStoryPrevChoice();
	UPROPERTY()
		APlayerController* OwningPlayer = nullptr;
	UPROPERTY()
		UFoldingSkyStoryComponent* EngagedComponent = nullptr;
	bool bIsInputBound;
	TArray<FInputActionBinding> ActionBindings;
};