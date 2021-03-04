// Copyright Folding Sky Games LLC 2021 All rights reserved.

#pragma once

#include "CoreMinimal.h"

#include "LootInfo.h"
#include "Components/ActorComponent.h"
#include "LootComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnLootDrop, const TArray<FLootTableRow>&, DroppedLoot);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class BNB_API ULootComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	ULootComponent();
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TArray<FLootChance> LootChances;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int32 Abundance = 0;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int32 Difficulty = 1000;
	UFUNCTION(BlueprintCallable)
		void DropLoot();
	UPROPERTY(BlueprintAssignable)
		FOnLootDrop OnLootDropped;
protected:

	virtual void BeginPlay() override;
	TArray<FLootTableRow> GenerateLootResults();	
};
