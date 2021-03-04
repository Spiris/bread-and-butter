// Copyright Folding Sky Games LLC 2021 All rights reserved.

#include "CharacterComponents/CharacterComponent.h"

#include "Engine/World.h"
#include "Components/SceneComponent.h"
#include "Components/TextRenderComponent.h"
#include "Engine/TextRenderActor.h"
#include "TimerManager.h"

UCharacterComponent::UCharacterComponent()
{
	PendingDamage = TArray<FCharacterDamage>();
	bHasReportedDeath = false;
}
void UCharacterComponent::BeginPlay()
{
	Super::BeginPlay();
	Status.Barrier = StatSheet.CombatStats.Barrier;
	Status.HitPoints = StatSheet.CombatStats.HitPoints;
	Status.CalcPercents(StatSheet.CombatStats.HitPoints, StatSheet.CombatStats.Barrier);
}
void UCharacterComponent::Attack(UCharacterComponent * Target, ECharacterDamageType Type)
{
	if (Target)
	{
		FCharacterDamage NewAttack;
		NewAttack.Source = this;
		NewAttack.DamageType = Type;
		Target->AddDamageToQueue(NewAttack);
	}
}

void UCharacterComponent::AddDamageToQueue(const FCharacterDamage & Damage)
{
	PendingDamage.Add(Damage);
	if (!PendingDamageHandle.IsValid())
	{
		GetWorld()->GetTimerManager().SetTimer(PendingDamageHandle, this, &UCharacterComponent::ProcessDamage, 0.25f, true);
	}
}

void UCharacterComponent::ProcessDamage()
{
	if (PendingDamage.IsValidIndex(0))
	{
		FCharacterDamage Total = PendingDamage[0];
		if (CheckToHit(Total))
		{
			int32 dmg = CalcDamage(Total);
			Total.Raw = dmg;
			if (dmg > 0)
			{
				Total.Barrier = FMath::Min(Status.Barrier, dmg);
				dmg -= Status.Barrier;
				if (dmg > 0)
				{
					Total.HitPoints = dmg;
				}
			}

			if (Total.Barrier > 0)
			{
				Total.ResultType = ECharacterDamageResultType::Barrier;
				if (Total.HitPoints > 0)
				{
					Total.Raw -= Total.Barrier;
					Total.HitPoints = 0;
					PendingDamage.Add(Total);
				}
			}
			else if (Total.HitPoints > 0)
			{
				Total.ResultType = ECharacterDamageResultType::HitPoints;
			}
			Status.Barrier -= Total.Barrier;
			Status.HitPoints -= Total.HitPoints;
			Status.CalcPercents(StatSheet.CombatStats.HitPoints, StatSheet.CombatStats.Barrier);
		}
		Multicast_ReportDamage(Total, Status);
		PendingDamage.RemoveAt(0);
	}
	else
	{
		PendingDamageHandle.Invalidate();
	}
	if (!bHasReportedDeath && Status.IsDead())
	{
		OnDeath.Broadcast();
		bHasReportedDeath = true;
	}
}

bool UCharacterComponent::CheckToHit(FCharacterDamage & Pending)
{
	if (Pending.ResultType != ECharacterDamageResultType::None)
		return true;
	if (Pending.Source)
	{
		FStatSheet& SourceStats = Pending.Source->StatSheet;
		switch (Pending.DamageType)
		{
		case ECharacterDamageType::Melee:
		{
			int32 melee = SourceStats.RollMeleeHit();
			int32 dodge = StatSheet.RollMeleeAvoidance();
			if (melee < dodge)
			{
				Pending.ResultType = ECharacterDamageResultType::Dodged;
				Pending.Raw = 0;
				return false;
			}
			break;
		}
		case ECharacterDamageType::Ranged:
		{
			int32 ranged = SourceStats.RollRangedHit();
			int32 defend = StatSheet.RollRangedAvoidance();
			if (ranged < defend)
			{
				Pending.ResultType = ECharacterDamageResultType::Deflected;
				return false;
			}
			break;
		}
		case ECharacterDamageType::Magical:
		{
			int32 magic = SourceStats.RollSpellHit();
			int32 resist = StatSheet.RollSpellAvoidance();
			if (magic < resist)
			{
				Pending.ResultType = ECharacterDamageResultType::Resisted;
				return false;
			}
			break;
		}
		case ECharacterDamageType::None:
		default:
			return false;
		}
		return true;
	}
	return false;
}

int32 UCharacterComponent::CalcDamage(FCharacterDamage & Pending)
{
	if (Pending.Raw > 0)
		return Pending.Raw;
	if (Pending.Source)
	{
		FStatSheet& SourceStats = Pending.Source->StatSheet;
		switch (Pending.DamageType)
		{
		case ECharacterDamageType::Melee:
			return SourceStats.RollMeleeDamage() - StatSheet.GetMeleeDamageMitigation();
		case ECharacterDamageType::Ranged:
			return SourceStats.RollRangedDamage() - StatSheet.GetRangedDamageMitigation();
		case ECharacterDamageType::Magical:
			return SourceStats.RollSpellDamage() - StatSheet.GetSpellDamageMitigation();
		case ECharacterDamageType::None:
		default:
			break;
		}
	}
	return 0;
}

void UCharacterComponent::Multicast_ReportDamage_Implementation(const FCharacterDamage& Damage, const FCharacterStatus& NewStatus)
{
	if (IsNetMode(NM_DedicatedServer))
	{
		return;
	}
	OnDamageReport.Broadcast(Damage, NewStatus);
}