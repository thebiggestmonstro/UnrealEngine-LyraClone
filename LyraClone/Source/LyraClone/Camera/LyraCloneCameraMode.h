// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "LyraCloneCameraMode.generated.h"

/** Camera Blending 대상 유닛 */
UCLASS(Abstract, NotBlueprintable)
class ULyraCloneCameraMode : public UObject
{
	GENERATED_BODY()
	
public:
	ULyraCloneCameraMode(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());
};

/** Camera Blending을 담당하는 객체 */
UCLASS()
class ULyraCloneCameraModeStack : public UObject
{
	GENERATED_BODY()
	
public:
	ULyraCloneCameraModeStack(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	/**
	 * member variables
	 */

	 /** 생성된 CameraMode를 관리 */
	UPROPERTY()
	TArray<TObjectPtr<ULyraCloneCameraMode>> CameraModeInstances;

	/** Camera Matrix Blend 업데이트 진행 큐 */
	UPROPERTY()
	TArray<TObjectPtr<ULyraCloneCameraMode>> CameraModeStack;
};