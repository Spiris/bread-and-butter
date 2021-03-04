// Copyright Folding Sky Games LLC 2021 All rights reserved.

#include "Player/PlayerCore.h"

UPlayerCore::UPlayerCore()
{
	PlayerId = 0;
}
FString UPlayerCore::GetNickname() const
{
	return FString("NONAME");
}
