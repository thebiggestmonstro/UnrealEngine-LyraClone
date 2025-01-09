// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "Camera/LyraCloneCameraMode.h"
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

	/** 어떤 Pawn을 소환할 것인지를 결정하는 Pawn의 Class */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "LyraClone|Pawn")
	TSubclassOf<APawn> PawnClass;

	/** Camera Mode */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Hak|Camera")
	TSubclassOf<ULyraCloneCameraMode> DefaultCameraMode;
};
