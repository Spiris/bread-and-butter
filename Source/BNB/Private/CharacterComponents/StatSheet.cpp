// Copyright Folding Sky Games LLC 2021 All rights reserved.

#include "CharacterComponents/StatSheet.h"

FStatSheet::FStatSheet()
{
	Modifiers = TMap<FString, FStatModifier>();
}

int32 FStatSheet::RollMeleeHit()
{
	return GetMeleeHitBonus() + RollModifier.MeleeHitRoll();
}
int32 FStatSheet::RollRangedHit()
{
	return GetRangedHitBonus() + RollModifier.RangedHitRoll();
}

int32 FStatSheet::RollSpellHit()
{
	return GetSpellHitBonus() + RollModifier.SpellHitRoll();
}

int32 FStatSheet::GetMeleeHitBonus()
{
	return CombatStats.HitRating +
		AbilityScore.Strength;
}

int32 FStatSheet::GetRangedHitBonus()
{
	return CombatStats.HitRating +
		AbilityScore.Dexterity;
}

int32 FStatSheet::GetSpellHitBonus()
{
	return CombatStats.HitRating +
		AbilityScore.Magic;
}

int32 FStatSheet::RollMeleeDamage()
{
	return GetMeleeDamageBonus() + RollModifier.MeleeRoll();
}

int32 FStatSheet::RollRangedDamage()
{
	return GetRangedDamageBonus() + RollModifier.RangedRoll();;
}

int32 FStatSheet::RollSpellDamage()
{
	return GetSpellDamageBonus() + RollModifier.SpellRoll();;
}

int32 FStatSheet::GetMeleeDamageBonus()
{
	return AbilityScore.Strength * 2;
}

int32 FStatSheet::GetRangedDamageBonus()
{
	return AbilityScore.Dexterity * 2;
}

int32 FStatSheet::GetSpellDamageBonus()
{
	return AbilityScore.Magic * 2;
}

int32 FStatSheet::RollMeleeAvoidance()
{
	return GetMeleeAvoidance() + RollModifier.DodgeRoll();
}
int32 FStatSheet::RollRangedAvoidance()
{
	return GetRangedAvoidance() + RollModifier.DefendRoll();
}
int32 FStatSheet::RollSpellAvoidance()
{
	return GetSpellAvoidance() + RollModifier.ResistRoll();
}
int32 FStatSheet::GetMeleeAvoidance()
{
	return CombatStats.MoveSpeed + AbilityScore.Agility;
}
int32 FStatSheet::GetRangedAvoidance()
{
	return CombatStats.MoveSpeed + AbilityScore.Agility;
}
int32 FStatSheet::GetSpellAvoidance()
{
	return CombatStats.MoveSpeed + AbilityScore.Focus;
}
int32 FStatSheet::GetMeleeDamageMitigation()
{
	return CombatStats.ArmorRating + AbilityScore.Vitality;
}
int32 FStatSheet::GetRangedDamageMitigation()
{
	return CombatStats.ArmorRating + AbilityScore.Agility;
}
int32 FStatSheet::GetSpellDamageMitigation()
{
	return CombatStats.SpellArmorRating + AbilityScore.Focus;
}

int32 FStatSheet::GetModifier(const FString & Key)
{
	if (FStatModifier* mod = Modifiers.Find(Key))
	{
		return mod->Value;
	}
	return 0;
}