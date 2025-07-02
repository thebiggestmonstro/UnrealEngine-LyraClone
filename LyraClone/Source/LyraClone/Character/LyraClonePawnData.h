// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "Camera/LyraCloneCameraMode.h"
#include "Input/LyraCloneInputConfig.h"
#include "LyraClonePawnData.generated.h"

class ULyraCloneAbilitySet;

/**
 * 
 */
UCLASS()
class LYRACLONE_API ULyraClonePawnData : public UPrimaryDataAsset
{
	GENERATED_BODY()
	
public:
	ULyraClonePawnData(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	/** 어떤 Pawn을 소환할 것인지를 결정하는 Pawn의 Class */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "LyraClone|Pawn")
	TSubclassOf<APawn> PawnClass;

	/** Camera Mode */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "LyraClone|Camera")
	TSubclassOf<ULyraCloneCameraMode> DefaultCameraMode;

	/** input configuration used by player controlled pawns to create input mappings and bind input actions */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "LyraClone|InputConfig")
	TObjectPtr<ULyraCloneInputConfig> InputConfig;

	/** 해당 Pawn의 Ability System에 허용할 AbilitySet */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "LyraClone|Abilities")
	TArray<TObjectPtr<ULyraCloneAbilitySet>> AbilitySets;
};
