// Copyright Folding Sky Games LLC 2021 All rights reserved.

#include "SpellsBlueprintFunctionLibrary.h"


TArray<FSpellKeyCombo> USpellsBlueprintFunctionLibrary::FilterInvalidSpells(const FKey& InputKey, const int32& SpellPosition, const TArray<FSpellKeyCombo>& CurrentSpells)
{
	TArray<FSpellKeyCombo> RetVal;
	for (const FSpellKeyCombo& Spell : CurrentSpells)
	{
		if (Spell.Incantation.IsValidIndex(SpellPosition) && Spell.Incantation[SpellPosition] == InputKey)
		{
			RetVal.Add(Spell);
		}		
	}
	return RetVal;
}