// Copyright Folding Sky Games LLC 2021 All rights reserved.

#include "CharacterComponents/RollModifier.h"

FRollModifier::FRollModifier()
{
	MeleeModifiers = TArray<FStatModifier>();
	RangedModifiers = TArray<FStatModifier>();
	SpellModifiers = TArray<FStatModifier>();
	MeleeHitModifiers = TArray<FStatModifier>();
	RangedHitModifiers = TArray<FStatModifier>();
	SpellHitModifiers = TArray<FStatModifier>();
	DodgeModifiers = TArray<FStatModifier>();
	DefendModifiers = TArray<FStatModifier>();
	ResistModifiers = TArray<FStatModifier>();
	Random = FRandomStream(FMath::Rand());
}


int32 FRollModifier::MeleeRoll()
{
	int32 max = Melee;
	for (FStatModifier& mod : MeleeModifiers)
		max += mod.Value;
	return Random.RandRange(0, max);
}

int32 FRollModifier::RangedRoll()
{
	int32 max = Ranged;
	for (FStatModifier& mod : RangedModifiers)
		max += mod.Value;
	return Random.RandRange(0, max);
}

int32 FRollModifier::SpellRoll()
{
	int32 max = Spell;
	for (FStatModifier& mod : SpellModifiers)
		max += mod.Value;
	return Random.RandRange(0, max);
}

int32 FRollModifier::MeleeHitRoll()
{
	int32 max = MeleeHit;
	for (FStatModifier& mod : MeleeHitModifiers)
		max += mod.Value;
	return Random.RandRange(0, max);
}

int32 FRollModifier::RangedHitRoll()
{
	int32 max = RangedHit;
	for (FStatModifier& mod : RangedHitModifiers)
		max += mod.Value;
	return Random.RandRange(0, max);
}

int32 FRollModifier::SpellHitRoll()
{
	int32 max = SpellHit;
	for (FStatModifier& mod : SpellHitModifiers)
		max += mod.Value;
	return Random.RandRange(0, max);
}

int32 FRollModifier::DodgeRoll()
{
	int32 max = Dodge;
	for (FStatModifier& mod : DodgeModifiers)
		max += mod.Value;
	return Random.RandRange(0, max);
}

int32 FRollModifier::DefendRoll()
{
	int32 max = Defend;
	for (FStatModifier& mod : DefendModifiers)
		max += mod.Value;
	return Random.RandRange(0, max);
}

int32 FRollModifier::ResistRoll()
{
	int32 max = Resist;
	for (FStatModifier& mod : ResistModifiers)
		max += mod.Value;
	return Random.RandRange(0, max);
}
