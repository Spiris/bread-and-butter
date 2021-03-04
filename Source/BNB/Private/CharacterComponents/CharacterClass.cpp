// Copyright Folding Sky Games LLC 2021 All rights reserved.

#include "CharacterComponents/CharacterClass.h"

FCharacterClass::FCharacterClass()
{
	Modifiers = TMap<FString, FStatModifier>();
}

int32 FCharacterClass::GetModifier(const FString& Key)
{
	if (FStatModifier* mod = Modifiers.Find(Key))
	{
		return mod->Value;
	}
	return 0;
}