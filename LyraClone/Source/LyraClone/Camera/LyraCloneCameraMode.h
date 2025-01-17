// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "LyraCloneCameraMode.generated.h"

class ULyraCloneCameraComponent;

/**
 * [0,1]을 BlendFunction에 맞게 재매핑을 위한 타입
 */
UENUM(BlueprintType)
enum class ELyraCloneCameraModeBlendFunction : uint8
{
	Linear,
	/**
	 * EaseIn/Out은 exponent 값에 의해 조절된다:
	 */
	EaseIn,
	EaseOut,
	EaseInOut,
	COUNT
};

/**
 * FLyraCloneCameraModeView
 */
struct FLyraCloneCameraModeView
{
	FLyraCloneCameraModeView();

	void Blend(const FLyraCloneCameraModeView& Other, float OtherWeight);

	FVector Location;
	FRotator Rotation;
	FRotator ControlRotation;
	float FieldOfView;
};

/** Camera Blending 대상 유닛 */
UCLASS(Abstract, NotBlueprintable)
class ULyraCloneCameraMode : public UObject
{
	GENERATED_BODY()
	
public:
	ULyraCloneCameraMode(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	/*
	* member methods
	*/
	ULyraCloneCameraComponent* GetLyraCloneCameraComponent() const;
	FVector GetPivotLocation() const;
	FRotator GetPivotRotation() const;
	AActor* GetTargetActor() const;
	void UpdateCameraMode(float DeltaTime);
	virtual void UpdateView(float DeltaTime);
	void UpdateBlending(float DeltaTime);

	/**
	 * member variables
	 */
	 /** CameraMode에 의해 생성된 CameraModeView */
	FLyraCloneCameraModeView View;

	/** Camera Mode의 FOV */
	UPROPERTY(EditDefaultsOnly, Category = "View", Meta = (UIMin = "5.0", UIMax = "170", ClampMin = "5.0", Clampmax = "170.0"))
	float FieldOfView;

	/** View에 대한 Pitch [Min, Max] */
	UPROPERTY(EditDefaultsOnly, Category = "View", Meta = (UIMin = "-89.9", UIMax = "89.9", ClampMin = "-89.9", Clampmax = "89.9"))
	float ViewPitchMin;

	UPROPERTY(EditDefaultsOnly, Category = "View", Meta = (UIMin = "-89.9", UIMax = "89.9", ClampMin = "-89.9", Clampmax = "89.9"))
	float ViewPitchMax;

	/** 얼마동안 Blend를 진행할지 시간을 의미 */
	UPROPERTY(EditDefaultsOnly, Category = "Blending")
	float BlendTime;

	/** 선형적인 Blend 값 [0, 1] */
	float BlendAlpha;

	/**
	 * 해당 CameraMode의 최종 Blend 값
	 * 앞서 BlendAlpha의 선형 값을 매핑하여 최종 BlendWeight를 계산 (코드를 보며, 이해해보자)
	 */
	float BlendWeight;

	/**
	 * EaseIn/Out에 사용한 Exponent
	 */
	UPROPERTY(EditDefaultsOnly, Category = "Blending")
	float BlendExponent;

	/** Blend function */
	ELyraCloneCameraModeBlendFunction BlendFunction;
};

/** Camera Blending을 담당하는 객체 */
UCLASS()
class ULyraCloneCameraModeStack : public UObject
{
	GENERATED_BODY()
	
public:
	ULyraCloneCameraModeStack(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	/*
	* member methods
	*/
	ULyraCloneCameraMode* GetCameraModeInstance(TSubclassOf<ULyraCloneCameraMode>& CameraModeClass);
	void PushCameraMode(TSubclassOf<ULyraCloneCameraMode>& CameraModeClass);
	void EvaluateStack(float DeltaTime, FLyraCloneCameraModeView& OutCameraModeView);
	void UpdateStack(float DeltaTime);
	void BlendStack(FLyraCloneCameraModeView& OutCameraModeView) const;
	
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