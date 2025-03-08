// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Equipment/LyraCloneEquipmentInstance.h"
#include "Cosmetics/LyraCloneCosmeticAnimationTypes.h"
#include "LyraCloneWeaponInstance.generated.h"

/**
 * 
 */
UCLASS()
class LYRACLONE_API ULyraCloneWeaponInstance : public ULyraCloneEquipmentInstance
{
	GENERATED_BODY()
	
public:
	ULyraCloneWeaponInstance(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());
	
	/** Weapon에 적용할 AnimLayer를 선택하여 반환 */
	UFUNCTION(BlueprintCallable, BlueprintPure = false, Category = Animation)
	TSubclassOf<UAnimInstance> PickBestAnimLayer(bool bEquipped, const FGameplayTagContainer& CosmeticTags) const;

	/** Weapon에 Equip/Unequip에 대한 Animation Set 정보를 들고 있다 */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Animation)
	FLyraCloneAnimLayerSelectionSet EquippedAnimSet;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Animation)
	FLyraCloneAnimLayerSelectionSet UnequippedAnimSet;
};
