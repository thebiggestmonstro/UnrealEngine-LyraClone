// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/LyraClonePlayerController.h"
#include "Camera/LyraClonePlayerCameraManager.h"

ALyraClonePlayerController::ALyraClonePlayerController(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	PlayerCameraManagerClass = ALyraClonePlayerCameraManager::StaticClass();
}
