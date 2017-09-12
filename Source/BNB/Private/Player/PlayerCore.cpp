// Fill out your copyright notice in the Description page of Project Settings.

#include "PlayerCore.h"

UPlayerCore::UPlayerCore()
{
	PlayerId = 0;
}
FString UPlayerCore::GetNickname() const
{
	return FString("NONAME");
}
