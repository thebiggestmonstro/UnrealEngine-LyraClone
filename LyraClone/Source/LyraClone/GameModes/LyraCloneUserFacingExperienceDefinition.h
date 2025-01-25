// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "LyraCloneUserFacingExperienceDefinition.generated.h"

/** forward declarations */
class UCommonSession_HostSessionRequest;

/**
 * 
 */
UCLASS()
class LYRACLONE_API ULyraCloneUserFacingExperienceDefinition : public UPrimaryDataAsset
{
	GENERATED_BODY()
	
public:
	/**
	 * Map 로딩 및 Experience 전환을 위해, MapID와 ExperienceID를 활용하여, HostSessionRequest 생성
	 */
	UFUNCTION(BlueprintCallable, BlueprintPure = false)
	UCommonSession_HostSessionRequest* CreateHostingRequest() const;

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
