// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Abilities/LyraCloneGameplayAbility.h"
#include "LyraCloneGameplayAbility_FromEquipment.generated.h"

class ULyraCloneEquipmentInstance;
class ULyraCloneInventoryItemInstance;

/**
 * 
 */
UCLASS()
class LYRACLONE_API ULyraCloneGameplayAbility_FromEquipment : public ULyraCloneGameplayAbility
{
	GENERATED_BODY()
	
public:
	ULyraCloneEquipmentInstance* GetAssociatedEquipment() const;
	ULyraCloneInventoryItemInstance* GetAssociatedItem() const;
};
