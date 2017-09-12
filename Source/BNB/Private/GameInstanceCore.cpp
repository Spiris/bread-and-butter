// Fill out your copyright notice in the Description page of Project Settings.

#include "GameInstanceCore.h"
#include "WorldNexus.h"

UGameInstanceCore::UGameInstanceCore()
	: Nexus(nullptr)
{

}
UWorldNexus* UGameInstanceCore::GetWorldNexus() 
{ 
	if (!Nexus)
	{
		Nexus = NewObject<UWorldNexus>(this);
		Nexus->InitializeNexus();
	}
	return Nexus; 
}
void UGameInstanceCore::Init() 
{
	Super::Init();
	GetWorldNexus();
}
void UGameInstanceCore::Shutdown()
{
	Nexus->ShutdownNexus();
	Super::Shutdown();	
}
