// Fill out your copyright notice in the Description page of Project Settings.


#include "LyraClonePlayerCameraManager.h"
#include UE_INLINE_GENERATED_CPP_BY_NAME(LyraClonePlayerCameraManager)

ALyraClonePlayerCameraManager::ALyraClonePlayerCameraManager(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	DefaultFOV = LYRACLONE_CAMERA_DEFAULT_FOV;
	ViewPitchMin = LYRACLONE_CAMERA_DEFAULT_PITCH_MIN;
	ViewPitchMax = LYRACLONE_CAMERA_DEFAULT_PITCH_MAX;
}
