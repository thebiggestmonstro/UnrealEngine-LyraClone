// Fill out your copyright notice in the Description page of Project Settings.


#include "Inventory/LyraCloneInventoryManagerComponent.h"
#include "LyraCloneInventoryItemInstance.h"
#include "LyraCloneItemDefinition.h"

ULyraCloneInventoryItemInstance* FLyraCloneInventoryList::AddEntry(TSubclassOf<ULyraCloneInventoryItemDefinition> ItemDef)
{
	ULyraCloneInventoryItemInstance* Result = nullptr;
	check(ItemDef);
	check(OwnerComponent);

	AActor* OwningActor = OwnerComponent->GetOwner();
	check(OwningActor->HasAuthority());

	FLyraCloneInventoryEntry& NewEntry = Entries.AddDefaulted_GetRef();
	NewEntry.Instance = NewObject<ULyraCloneInventoryItemInstance>(OwningActor);
	NewEntry.Instance->ItemDef = ItemDef;

	// iterating fragments and call callback to OnInstanceCreated()
	for (const ULyraCloneInventoryItemFragment* Fragment : GetDefault<ULyraCloneInventoryItemDefinition>(ItemDef)->Fragments)
	{
		if (Fragment)
		{
			Fragment->OnInstanceCreated(NewEntry.Instance);
		}
	}

	Result = NewEntry.Instance;
	return Result;
}

ULyraCloneInventoryManagerComponent::ULyraCloneInventoryManagerComponent(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
	, InventoryList(this)
{

}

ULyraCloneInventoryItemInstance* ULyraCloneInventoryManagerComponent::AddItemDefinition(TSubclassOf<ULyraCloneInventoryItemDefinition> ItemDef)
{
	ULyraCloneInventoryItemInstance* Result = nullptr;
	if (ItemDef)
	{
		Result = InventoryList.AddEntry(ItemDef);
	}
	return Result;
}

