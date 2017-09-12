// Fill out your copyright notice in the Description page of Project Settings.

#include "WorldNexus.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/World.h"
#include "GameInstanceCore.h"
#include "NexusRelay.h"


UWorldNexus* UWorldNexus::Get(UObject* WorldContextObject)
{
	if (UGameInstanceCore* instance = Cast<UGameInstanceCore>(UGameplayStatics::GetGameInstance(WorldContextObject)))
	{
		return instance->GetWorldNexus();
	}
	return nullptr;
}
UWorldNexus::UWorldNexus() :
	Relay(nullptr)
{

}


void UWorldNexus::InitializeNexus()
{
	Relay = new FNexusRelay();
	if (Relay)
	{
		UE_LOG(LogNexus, Log, TEXT("Initialized nexus."));
	}
}

void UWorldNexus::ShutdownNexus()
{
	if (Relay)
	{
		Relay->~FNexusRelay();
		Relay = nullptr;
	}
}

void UWorldNexus::ReceiveMessage(const FNexusMessage& Message)
{

}
