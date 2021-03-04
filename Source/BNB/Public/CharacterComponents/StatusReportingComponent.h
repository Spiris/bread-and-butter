// Copyright Folding Sky Games LLC 2021 All rights reserved.

#pragma once

#include "CoreMinimal.h"

#include "CharacterDamage.h"
#include "CurrentStatus.h"
#include "Components/SceneComponent.h"
#include "StatusReportingComponent.generated.h"

class AStatusReportText;

UCLASS(BlueprintType, meta = (BlueprintSpawnableComponent))
class BNB_API UStatusReportingComponent : public USceneComponent
{
	GENERATED_BODY()
public:
	UStatusReportingComponent();

	virtual void BeginPlay() override;
	UPROPERTY(EditAnywhere)
		TSubclassOf<AStatusReportText> StatusReportTextClass;
	UPROPERTY(EditAnywhere)
		TMap<ECharacterDamageResultType, FColor> DamageColorKey;
	UFUNCTION()
		void OnDamageReport(const FCharacterDamage& Report, const FCharacterStatus& NewStatus);
	void AddDamageToQueue(const FCharacterDamage& Damage);
	void ProcessStatus();
protected:
	UPROPERTY()
		TArray<FCharacterDamage> PendingDamage;
	FTimerHandle PendingStatusHandle;
	void HandleReportVisuals(const FCharacterDamage& Damage);
};
