// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "LyraClonePawnData.generated.h"

/**
 * 
 */
UCLASS()
class LYRACLONE_API ULyraClonePawnData : public UPrimaryDataAsset
{
	GENERATED_BODY()
	
public:
	ULyraClonePawnData(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());
};
