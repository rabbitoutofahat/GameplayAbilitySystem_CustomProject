// Copyright Druid Mechanics


#include "AbilitySystem/Data/LootTiers.h"

TArray<FLootItem> ULootTiers::GetLootItems()
{
	TArray<FLootItem> ReturnItems;
	for (FLootItem& Item : LootItems)
	{
		for (int32 i = 0; i < Item.MaxSpawnNumber; i++)
		{
			if (FMath::FRandRange(1.f, 100.f) < Item.SpawnChance)
			{
				FLootItem NewItem;
				NewItem.LootClass = Item.LootClass;
				NewItem.bLootLevelOverride = Item.bLootLevelOverride;
				ReturnItems.Add(NewItem); 
			}
		}
	}

	return ReturnItems;
}
