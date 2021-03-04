// Copyright Folding Sky Games LLC 2021 All rights reserved.

#include "GameInstanceCore.h"
#include "WorldNexus/WorldNexus.h"

UGameInstanceCore::UGameInstanceCore()
	: Nexus(nullptr)
{

}
UWorldNexus* UGameInstanceCore::GetWorldNexus()  {  return Nexus;  }
void UGameInstanceCore::Init() 
{
	Super::Init();
	//FirstLoadHandle = FCoreUObjectDelegates::PostLoadMapWithWorld.AddRaw(this, &UGameInstanceCore::PostFirstLoadMap);
	Nexus = NewObject<UWorldNexus>(this);	
}
void UGameInstanceCore::PostFirstLoadMap(UWorld* World)
{
	FCoreUObjectDelegates::PostLoadMapWithWorld.Remove(FirstLoadHandle);
	Nexus->InitializeNexus();
}
void UGameInstanceCore::Shutdown()
{
	Nexus->ShutdownNexus();
	Super::Shutdown();	
}
