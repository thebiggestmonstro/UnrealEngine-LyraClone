// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Camera/LyraCloneCameraMode.h"
#include "LyraCloneCameraMode_ThirdPerson.generated.h"

/**
 * 
 */
UCLASS(Abstract, Blueprintable)
class LYRACLONE_API ULyraCloneCameraMode_ThirdPerson : public ULyraCloneCameraMode
{
	GENERATED_BODY()
	
public:
	ULyraCloneCameraMode_ThirdPerson(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());
};
