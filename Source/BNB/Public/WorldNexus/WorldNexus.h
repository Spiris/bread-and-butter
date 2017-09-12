// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "CoreMinimal.h"
#include "NexusMessage.h"
#include "WorldNexus.generated.h"
DECLARE_LOG_CATEGORY_CLASS(LogNexus, Log, All);
/**
 * 
 */
class FNexusRelay;
class UGameInstanceCore;

UCLASS()
class BNB_API UWorldNexus : public UObject
{
	GENERATED_BODY()
public:
	static UWorldNexus* Get(UObject* WorldContextObject);

	void InitializeNexus();
	void ShutdownNexus();

	void ReceiveMessage(const FNexusMessage& Message);

	UWorldNexus();
private:
	FNexusRelay* Relay;
};
