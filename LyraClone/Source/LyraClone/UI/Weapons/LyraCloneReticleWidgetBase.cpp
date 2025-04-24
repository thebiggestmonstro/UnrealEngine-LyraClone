// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Weapons/LyraCloneReticleWidgetBase.h"
#include "Weapons/LyraCloneWeaponInstance.h"
#include "Inventory/LyraCloneInventoryItemInstance.h"
#include UE_INLINE_GENERATED_CPP_BY_NAME(LyraCloneReticleWidgetBase)

ULyraCloneReticleWidgetBase::ULyraCloneReticleWidgetBase(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{}

void ULyraCloneReticleWidgetBase::InitializeFromWeapon(ULyraCloneWeaponInstance* InWeapon)
{
	WeaponInstance = InWeapon;
	InventoryInstance = nullptr;
	if (WeaponInstance)
	{
		InventoryInstance = Cast<ULyraCloneInventoryItemInstance>(WeaponInstance->GetInstigator());
	}
}
