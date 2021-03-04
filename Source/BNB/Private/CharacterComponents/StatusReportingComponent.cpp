// Copyright Folding Sky Games LLC 2021 All rights reserved.

#include "CharacterComponents/StatusReportingComponent.h"

#include "Engine/World.h"
#include "TimerManager.h"

#include "CharacterComponents/CharacterComponent.h"
#include "StatusReportText.h"

UStatusReportingComponent::UStatusReportingComponent()
{
	PendingDamage = TArray<FCharacterDamage>();
}
void UStatusReportingComponent::BeginPlay()
{
	Super::BeginPlay();
	if (UCharacterComponent* comp = Cast<UCharacterComponent>(GetOwner()->GetComponentByClass(UCharacterComponent::StaticClass())))
	{
		comp->OnDamageReport.AddDynamic(this, &UStatusReportingComponent::OnDamageReport);
	}
}
void UStatusReportingComponent::OnDamageReport(const FCharacterDamage& Report, const FCharacterStatus& NewStatus)
{
	AddDamageToQueue(Report);
}
void UStatusReportingComponent::AddDamageToQueue(const FCharacterDamage & Damage)
{
	PendingDamage.Add(Damage);
	if (!PendingStatusHandle.IsValid())
	{
		GetWorld()->GetTimerManager().SetTimer(PendingStatusHandle, this, &UStatusReportingComponent::ProcessStatus, 0.25f, true);
	}
}

void UStatusReportingComponent::ProcessStatus()
{
	if (PendingDamage.Num() > 0)
	{
		HandleReportVisuals(PendingDamage[0]);
		PendingDamage.RemoveAt(0);
	}
	else
	{
		PendingStatusHandle.Invalidate();
	}
}

void UStatusReportingComponent::HandleReportVisuals(const FCharacterDamage & Damage)
{
	if (StatusReportTextClass)
	{
		FVector loc = GetComponentLocation();
		FRotator rot = GetComponentRotation();
		if (AStatusReportText* Text = GetWorld()->SpawnActor<AStatusReportText>(StatusReportTextClass, loc, rot))
		{
			FText Output;
			switch (Damage.ResultType)
			{
			case ECharacterDamageResultType::Barrier:
				Output = FText::Format(NSLOCTEXT("DamageReport", "BarrierDamage", "({0})"), FText::AsNumber(Damage.Barrier));
				break;
			case ECharacterDamageResultType::HitPoints:
				Output = FText::Format(NSLOCTEXT("DamageReport", "HitPointDamage", "{0}"), FText::AsNumber(Damage.HitPoints));
				break;
			case ECharacterDamageResultType::Absorbed:
				Output = FText::Format(NSLOCTEXT("DamageReport", "Absorbed", "Absorbed! ({0})"), FText::AsNumber(Damage.Raw));
				break;
			case ECharacterDamageResultType::Dodged:
				Output = NSLOCTEXT("DamageReport", "Dodged", "Dodged!");
				break;
			case ECharacterDamageResultType::Resisted:
				Output = NSLOCTEXT("DamageReport", "Resisted", "Resisted!");
				break;
			case ECharacterDamageResultType::Deflected:
				Output = NSLOCTEXT("DamageReport", "Deflected", "Deflected!");
				break;
			case ECharacterDamageResultType::None:
			default:
				Output = NSLOCTEXT("DamageReport", "None", "No Damage!");
				break;
			}
			if (FColor* color = DamageColorKey.Find(Damage.ResultType))
			{
				Text->SetTextColor(*color);
			}
			Text->SetText(Output);
		}
	}
}