// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "LyraCloneCameraMode.h"
#include "Camera/CameraComponent.h"
#include "LyraCloneCameraComponent.generated.h"

class ULyraCloneCameraModeStack;
/** template forward declaration */
template <class TClass> class TSubclassOf;

/** (return type, delegate_name) */
DECLARE_DELEGATE_RetVal(TSubclassOf<ULyraCloneCameraMode>, FLyraCloneCameraModeDelegate);

/**
 * 
 */
UCLASS()
class LYRACLONE_API ULyraCloneCameraComponent : public UCameraComponent
{
	GENERATED_BODY()
	
public:
	ULyraCloneCameraComponent(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	static ULyraCloneCameraComponent* FindCameraComponent(const AActor* Actor) { return (Actor ? Actor->FindComponentByClass<ULyraCloneCameraComponent>() : nullptr); }

	/**
	 * CameraComponent interface
	 */
	virtual void OnRegister() final;
	virtual void GetCameraView(float deltaTime, FMinimalViewInfo& DesiredView) final;

	/**
	 * member methods
	 */
	AActor* GetTargetActor() const { return GetOwner(); }
	void UpdateCameraModes();

	/**
	 * member variables
	 */
	 /** 카메라의 blending 기능을 지원하는 stack */
	UPROPERTY()
	TObjectPtr<ULyraCloneCameraModeStack> CameraModeStack;
	
	/** 현재 CameraMode를 가져오는 Delegate */
	FLyraCloneCameraModeDelegate DetermineCameraModeDelegate;
};
