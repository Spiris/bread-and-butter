// Copyright Folding Sky Games LLC 2021 All rights reserved.

#include "Player/PlayerControllerCore.h"
#include "Player/PlayerCore.h"
#include "WorldNexus/WorldNexus.h"

void APlayerControllerCore::BeginPlay()
{
	Super::BeginPlay();
	UWorldNexus::Get(this)->InitializeNexus();
}


FString APlayerControllerCore::GetCharacterName()
{
	return FString();
}
int32 APlayerControllerCore::GetCharacterId()
{
	return 0;
}

UPlayerCore* APlayerControllerCore::GetPlayerCore()
{
	if (!PlayerCore)
	{
		PlayerCore = Cast<UPlayerCore>(GetLocalPlayer());
	}
	return PlayerCore;
}
FString APlayerControllerCore::GetPlayerNickname()
{
	if (GetPlayerCore())
	{
		return PlayerCore->GetNickname();
	}
	return FString();
}
int32 APlayerControllerCore::GetPlayerId()
{
	return 0;
}