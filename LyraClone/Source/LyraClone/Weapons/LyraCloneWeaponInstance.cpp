// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapons/LyraCloneWeaponInstance.h"

ULyraCloneWeaponInstance::ULyraCloneWeaponInstance(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{

}

TSubclassOf<UAnimInstance> ULyraCloneWeaponInstance::PickBestAnimLayer(bool bEquipped, const FGameplayTagContainer& CosmeticTags) const
{
	const FLyraCloneAnimLayerSelectionSet& SetToQuery = (bEquipped ? EquippedAnimSet : UnequippedAnimSet);
	return SetToQuery.SelectBestLayer(CosmeticTags);
}