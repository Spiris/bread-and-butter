// Copyright Folding Sky Games LLC 2021 All rights reserved.

#include "SpawnZoneComponent.h"

#include "Components/BoxComponent.h"
#include "Components/SphereComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "Engine/World.h"
#include "TimerManager.h"


USpawnZoneComponent::USpawnZoneComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
	SpawnTimeMin = 1.0f;
	SpawnTimeMax = 2.0f;
	MaximumSpawnedActors = 2;
	bIsSpawning = true;
}
void USpawnZoneComponent::AddToActors(AActor* Actor)
{
	if (Actor && !SpawnedActors.Contains(Actor))
	{
		Actor->OnEndPlay.AddUniqueDynamic(this, &USpawnZoneComponent::OnSpawnedEndPlay);
		SpawnedActors.Add(Actor);
		if (bIsSpawning && SpawnedActors.Num() >= MaximumSpawnedActors)
		{
			GetWorld()->GetTimerManager().ClearTimer(SpawnTimerHandle);
			SpawnTimerHandle.Invalidate();
		}
	}
}
void USpawnZoneComponent::RemoveFromActors(AActor * Actor)
{
	if (Actor)
	{
		Actor->OnEndPlay.RemoveDynamic(this, &USpawnZoneComponent::OnSpawnedEndPlay);
		SpawnedActors.Remove(Actor);
		RollSpawnTimer();
	}
}
void USpawnZoneComponent::SetIsSpawning(bool Value)
{
	bIsSpawning = Value;
	RollSpawnTimer();
}
void USpawnZoneComponent::BeginPlay()
{
	Super::BeginPlay();
	RollSpawnTimer();
}

void USpawnZoneComponent::Spawn()
{
	if (SpawnableClasses.Num() > 0)
	{
		if (UClass* classToSpawn = SpawnableClasses[FMath::RandRange(0, SpawnableClasses.Num() - 1)])
		{
			FVector spawnLocation = GetOwner()->GetActorLocation();
			if (SpawnAreaComponent)
			{
				if (UBoxComponent* box = Cast<UBoxComponent>(SpawnAreaComponent))
				{
					spawnLocation = UKismetMathLibrary::RandomPointInBoundingBox(box->GetComponentLocation(), box->GetScaledBoxExtent());
				}
				else if (USphereComponent* sphere = Cast<USphereComponent>(SpawnAreaComponent))
				{
					spawnLocation = sphere->GetComponentLocation() + FMath::VRand() * FMath::RandRange(0.0f, 1.0f) * sphere->GetScaledSphereRadius();
				}
			}
			AddToActors(GetWorld()->SpawnActor<AActor>(classToSpawn, spawnLocation, FMath::VRand().ToOrientationRotator()));
		}
	}
	RollSpawnTimer();
}

void USpawnZoneComponent::RollSpawnTimer()
{
	if (bIsSpawning && SpawnedActors.Num() < MaximumSpawnedActors)
	{
		float TimeTillNext = FMath::RandRange(SpawnTimeMin, SpawnTimeMax);
		GetWorld()->GetTimerManager().SetTimer(SpawnTimerHandle, this, &USpawnZoneComponent::Spawn, TimeTillNext);
	}
	else
	{
		GetWorld()->GetTimerManager().ClearTimer(SpawnTimerHandle);
		SpawnTimerHandle.Invalidate();
	}
}


void USpawnZoneComponent::OnSpawnedEndPlay(AActor* Actor, const EEndPlayReason::Type Reason)
{
	RemoveFromActors(Actor);
}

