// Fill out your copyright notice in the Description page of Project Settings.

#include "PlayerControllerCore.h"
#include "PlayerCore.h"
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