// Copyright Folding Sky Games LLC 2021 All rights reserved.

#pragma once

#include "CoreMinimal.h"

#include "Components/ActorComponent.h"
#include "SpawnZoneComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class BNB_API USpawnZoneComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	USpawnZoneComponent();
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		float SpawnTimeMin;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		float SpawnTimeMax;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		int32 MaximumSpawnedActors;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		TArray<TSubclassOf<AActor>> SpawnableClasses;
	UPROPERTY(BlueprintReadWrite)
		class UPrimitiveComponent* SpawnAreaComponent;
	UFUNCTION(BlueprintCallable)
		void AddToActors(AActor* Actor);
	UFUNCTION(BlueprintCallable)
		void RemoveFromActors(AActor* Actor);
	UFUNCTION(BlueprintCallable)
		void SetIsSpawning(bool Value);
	UFUNCTION(BlueprintPure)
		bool GetIsSpawning() { return bIsSpawning; }
protected:
	virtual void BeginPlay() override;
	void Spawn();
	void RollSpawnTimer();
	FTimerHandle SpawnTimerHandle;
	bool bIsSpawning;
	UPROPERTY()
		TArray<AActor*> SpawnedActors;
	UFUNCTION()
		void OnSpawnedEndPlay(AActor* Actor, const EEndPlayReason::Type Reason);
};
