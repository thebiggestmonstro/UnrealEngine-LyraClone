// Fill out your copyright notice in the Description page of Project Settings.


#include "Inventory/LyraCloneInventoryFragment_SetStats.h"
#include "LyraCloneInventoryItemInstance.h"

void ULyraCloneInventoryFragment_SetStats::OnInstanceCreated(ULyraCloneInventoryItemInstance* Instance) const
{
	// iterating InitialItemStats and add stat tag to InventoryItemInstance
	for (const auto& InitialItemStat : InitialItemStats)
	{
		Instance->AddStatTagStack(InitialItemStat.Key, InitialItemStat.Value);
	}
}