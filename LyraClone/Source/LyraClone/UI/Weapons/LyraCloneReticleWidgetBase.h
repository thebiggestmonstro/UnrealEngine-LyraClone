// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CommonUserWidget.h"
#include "UObject/ObjectPtr.h"
#include "UObject/UObjectGlobals.h"
#include "LyraCloneReticleWidgetBase.generated.h"

/** forward declarations */
class ULyraCloneWeaponInstance;
class ULyraCloneInventoryItemInstance;

/**
 * 
 */
UCLASS()
class LYRACLONE_API ULyraCloneReticleWidgetBase : public UCommonUserWidget
{
	GENERATED_BODY()
	
public:
	ULyraCloneReticleWidgetBase(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	UFUNCTION(BlueprintCallable)
	void InitializeFromWeapon(ULyraCloneWeaponInstance* InWeapon);

	/**
	 * WeaponInstance/InventoryInstance를 상태 추적용으로 캐싱 목적
	 */
	UPROPERTY(BlueprintReadOnly)
	TObjectPtr<ULyraCloneWeaponInstance> WeaponInstance;

	UPROPERTY(BlueprintReadOnly)
	TObjectPtr<ULyraCloneInventoryItemInstance> InventoryInstance;
};
