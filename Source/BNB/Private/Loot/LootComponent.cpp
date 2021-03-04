// Copyright Folding Sky Games LLC 2021 All rights reserved.

#include "Loot/LootComponent.h"


ULootComponent::ULootComponent() : Super()
{
	LootChances = TArray<FLootChance>();
}

void ULootComponent::BeginPlay()
{
	Super::BeginPlay();	
}

TArray<FLootTableRow> ULootComponent::GenerateLootResults()
{
	TArray<FLootTableRow> Loot; Loot.Empty();
	int32 numPossible = LootChances.Num();
	if (numPossible > 0 && Abundance > 0)
	{		
		for (int32 i = 0; i < Abundance; i++)
		{
			TArray<FDataTableRowHandle> handles;
			int32 pick = FMath::RandRange(0, Difficulty);
			for (FLootChance& chance : LootChances)
			{
				if (pick < chance.ProbabilityOfDrop)
				{
					handles.Add(chance.LootHandle);
				}
			}
			if (handles.Num() > 0)
			{
				pick = FMath::RandRange(0, handles.Num() - 1);
				if (FLootTableRow* loot = handles[pick].GetRow<FLootTableRow>("Loot Roll"))
				{
					Loot.Add(FLootTableRow(*loot));
				}
			}			
		}
	}
	return Loot;
}

void ULootComponent::DropLoot()
{
	const TArray<FLootTableRow>& Loot = GenerateLootResults();

	if(Loot.Num() > 0)
	{
		OnLootDropped.Broadcast(Loot);
	}
}
