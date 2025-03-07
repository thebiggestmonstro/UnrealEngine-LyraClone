// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "LyraCloneInventoryItemInstance.generated.h"

class ULyraCloneInventoryItemFragment;
class ULyraCloneInventoryItemDefinition;

/**
 * 
 */
UCLASS()
class LYRACLONE_API ULyraCloneInventoryItemInstance : public UObject
{
	GENERATED_BODY()
	
public:
	ULyraCloneInventoryItemInstance(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	UFUNCTION(BlueprintCallable, BlueprintPure = false, meta = (DeterminesOutputType = FragmentClass))
	const ULyraCloneInventoryItemFragment* FindFragmentByClass(TSubclassOf<ULyraCloneInventoryItemFragment> FragmentClass) const;

	template <typename ResultClass>
	const ResultClass* FindFragmentByClass() const
	{
		return (ResultClass*)FindFragmentByClass(ResultClass::StaticClass());
	}

	/** Inventory Item의 인스턴스에는 무엇으로 정의되었는지 메타 클래스인 LyraCloneInventoryItemDefinition을 들고 있다 */
	UPROPERTY()
	TSubclassOf<ULyraCloneInventoryItemDefinition> ItemDef;
};
