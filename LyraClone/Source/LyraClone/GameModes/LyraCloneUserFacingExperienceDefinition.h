// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "LyraCloneUserFacingExperienceDefinition.generated.h"

/**
 * 
 */
UCLASS()
class LYRACLONE_API ULyraCloneUserFacingExperienceDefinition : public UPrimaryDataAsset
{
	GENERATED_BODY()
	
public:
	/**
	* member variables
	*/

	/** the specific map to load */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = Experience, meta = (AllowedTypes = "Map"))
	FPrimaryAssetId MapID;

	/** the gameplay expierence to load */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = Experience, meta = (AllowedTypes = "LyraCloneExperienceDefinition"))
	FPrimaryAssetId ExperienceID;
};
