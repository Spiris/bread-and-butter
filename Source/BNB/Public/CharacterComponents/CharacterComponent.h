// Copyright Folding Sky Games LLC 2021 All rights reserved.

#pragma once

#include "CoreMinimal.h"

#include "StatSheet.h"
#include "CharacterDamage.h"
#include "CurrentStatus.h"
#include "Components/ActorComponent.h"
#include "CharacterComponent.generated.h"

class AActor;
class ATextRenderActor;
class USceneComponent;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnCharacterDamageReport, const FCharacterDamage&, Report, const FCharacterStatus&, NewStatus);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnCharacterEvent);
UCLASS(BlueprintType, meta = (BlueprintSpawnableComponent))
class BNB_API UCharacterComponent : public UActorComponent
{
	GENERATED_BODY()
public:
	UCharacterComponent();

	virtual void BeginPlay() override;
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		FString CharacterName;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		FStatSheet StatSheet;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		FCharacterStatus Status;

	UFUNCTION(BlueprintCallable)
		void Attack(UCharacterComponent* Target, ECharacterDamageType Type);
	UPROPERTY(BlueprintAssignable)
		FOnCharacterDamageReport OnDamageReport; 
	UPROPERTY(BlueprintAssignable)
		FOnCharacterEvent OnDeath;
	void AddDamageToQueue(const FCharacterDamage& Damage);
	void ProcessDamage();
protected:
	UPROPERTY()
		TArray<FCharacterDamage> PendingDamage;
	FTimerHandle PendingDamageHandle;
	bool bHasReportedDeath;

	UFUNCTION(NetMulticast, Reliable)
		void Multicast_ReportDamage(const FCharacterDamage& Damage, const FCharacterStatus& NewStatus);

	bool CheckToHit(FCharacterDamage& Pending);
	int32 CalcDamage(FCharacterDamage& Pending);
};
