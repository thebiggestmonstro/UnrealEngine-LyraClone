// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Templates/SubclassOf.h"
#include "Inventory/LyraCloneItemDefinition.h"
#include "InventoryFragment_ReticleConfig.generated.h"

/** forward declaration */
class ULyraCloneReticleWidgetBase;

/**
 * 
 */
UCLASS()
class LYRACLONE_API UInventoryFragment_ReticleConfig : public ULyraCloneInventoryItemFragment
{
	GENERATED_BODY()
	
public:
	/** 무기에 결합된 ReticleWidget 정보를 가지고 있는 Fragment */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Reticle)
	TArray<TSubclassOf<ULyraCloneReticleWidgetBase>> ReticleWidgets;
};
