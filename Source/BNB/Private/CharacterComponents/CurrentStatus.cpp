// Copyright Folding Sky Games LLC 2021 All rights reserved.

#include "CharacterComponents/CurrentStatus.h"


void FCharacterStatus::CalcPercents(const float& MaxHitPoints, const float& MaxBarrier)
{
	PercentHitPoints = HitPoints / MaxHitPoints;
	PercentBarrier = Barrier / MaxBarrier;
}

bool FCharacterStatus::IsDead()
{
	return (HitPoints < 1);
}
