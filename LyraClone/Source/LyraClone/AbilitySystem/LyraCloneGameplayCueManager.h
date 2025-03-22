// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayCueManager.h"
#include "LyraCloneGameplayCueManager.generated.h"

/**
 * 
 */
UCLASS()
class LYRACLONE_API ULyraCloneGameplayCueManager : public UGameplayCueManager
{
	GENERATED_BODY()
	
public:
	static ULyraCloneGameplayCueManager* Get();

	ULyraCloneGameplayCueManager(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	void RefreshGameplayCuePrimaryAsset();
};
