// Fill out your copyright notice in the Description page of Project Settings.

#include "WorldNexus.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/World.h"
#include "GameInstanceCore.h"
#include "NexusRelay.h"
#include "ConfigCacheIni.h"
#include "IPv4Endpoint.h"

FIPv4Endpoint UWorldNexus::GetLocalEndpoint()
{
	FString epstring;
	if (GConfig->GetString(TEXT("WorldNexus"), TEXT("LocalEndpoint"), epstring, GGameIni))
	{
		FIPv4Endpoint endpoint;
		if (FIPv4Endpoint::Parse(epstring, endpoint))
		{
			return endpoint;
		}
	}
	return FIPv4Endpoint::Any;
}
UWorldNexus* UWorldNexus::Get(UObject* WorldContextObject)
{
	if (UGameInstanceCore* instance = Cast<UGameInstanceCore>(UGameplayStatics::GetGameInstance(WorldContextObject)))
	{
		return instance->GetWorldNexus();
	}
	return nullptr;
}
UWorldNexus::UWorldNexus() : Relay(nullptr) { }

void UWorldNexus::InitializeNexus()
{
	Relay = new FNexusRelay();
	if (Relay)
	{
		Relay->OpenRelay(GetLocalEndpoint());
		UE_LOG(LogNexus, Log, TEXT("Initialized nexus."));
		GetWorld()->GetTimerManager().SetTimer(RelayPollingHandle, this, &UWorldNexus::PollNexusRelay, 0.1f, true);
	}
}
void UWorldNexus::PollNexusRelay()
{
	if (Relay)
	{
		Relay->PollPendingConnections();
		Relay->TestConnections();
		Relay->ReadMessages(this);
	}
}

void UWorldNexus::ShutdownNexus()
{
	if (Relay)
	{
		Relay->CloseRelay();
		Relay->~FNexusRelay();
		Relay = nullptr;
	}
}

void UWorldNexus::ReceiveMessage(const FNexusMessage& Message)
{

}
