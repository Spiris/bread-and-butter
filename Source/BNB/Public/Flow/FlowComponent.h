// Copyright Folding Sky Games LLC 2021 All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "FlowComponent.generated.h"

class UPrimitiveComponent;
UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class BNB_API UFlowComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UFlowComponent();
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FVector FlowVector;
	UFUNCTION(BlueprintCallable)
		void SetPrimitiveComponent(UPrimitiveComponent* NewPrimitiveComponent);
	UFUNCTION(BlueprintPure)
		UPrimitiveComponent* GetPrimitiveComponent() { return PrimitiveComponent; }
protected:
	virtual void BeginPlay() override;
	UPROPERTY()
		UPrimitiveComponent* PrimitiveComponent;		
	UFUNCTION()
		void OnComponentBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	UFUNCTION()
		void OnComponentEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
};
