// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "LyraCloneExperienceActionSet.generated.h"

class UGameFeatureAction;

/**
 * 
 */
UCLASS()
class LYRACLONE_API ULyraCloneExperienceActionSet : public UPrimaryDataAsset
{
	GENERATED_BODY()
	
public:
	ULyraCloneExperienceActionSet();

#if WITH_EDITORONLY_DATA
	virtual void UpdateAssetBundleData() override;
#endif

	/**
	 * member variables
	 */
	UPROPERTY(EditAnywhere, Category = "Actions to Perform")
	TArray<TObjectPtr<UGameFeatureAction>> Actions;
};
