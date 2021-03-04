// Copyright Folding Sky Games LLC 2021 All rights reserved.

#pragma once

#include "CoreMinimal.h"

class UCharacterComponent;
class BNB_API FCombat
{
public:
	static int32 GetMeleeAttackBonus();
	static int32 GetRangedAttackBonus();
	static int32 GetThrownAttackBonus();
	static int32 GetCombatManeuverBonus();
	static int32 GetFortitudeSave();
	static int32 GetReflexSave();
	static int32 GetWillpowerSave();
};
