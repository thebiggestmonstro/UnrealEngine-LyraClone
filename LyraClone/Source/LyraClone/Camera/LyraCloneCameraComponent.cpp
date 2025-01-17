// Fill out your copyright notice in the Description page of Project Settings.


#include "Camera/LyraCloneCameraComponent.h"
#include "LyraCloneCameraMode.h"
#include UE_INLINE_GENERATED_CPP_BY_NAME(LyraCloneCameraComponent)

ULyraCloneCameraComponent::ULyraCloneCameraComponent(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{

}

void ULyraCloneCameraComponent::OnRegister()
{
	Super::OnRegister();
	if (!CameraModeStack)
	{
		// 초기화 (BeginPlay와 같은)가 딱히 필요없는 객체로 NewObject로 할당
		CameraModeStack = NewObject<ULyraCloneCameraModeStack>(this);
	}
}

void ULyraCloneCameraComponent::GetCameraView(float deltaTime, FMinimalViewInfo& DesiredView)
{
	check(CameraModeStack);

	UpdateCameraModes();

	// EvaluateStack은 CameraModeStack에 있는 CameraMode를 업데이트(+블랜딩)하고 CameraModeStack을 Bottom-Top까지 업데이트된 CameraMode들을 Lerp를 진행해준다.
	// - 이에 대한 결과는 CameraModeView에 캐싱된다
	FLyraCloneCameraModeView CameraModeView;
	CameraModeStack->EvaluateStack(deltaTime, CameraModeView);

	if (APawn* TargetPawn = Cast<APawn>(GetTargetActor()))
	{
		if (APlayerController* PC = TargetPawn->GetController<APlayerController>())
		{
			// PlayerController의 ControlRotation을 계산된 CamerModeView의 ControlRotation으로 업데이트해주자
			// - SetControlRotation을 외우기보다 한번 코드를 보자:
			//   - 해당 함수는 PC가 Possess하고 있는 Pawn의 RootComponent에 ControlRotation을 반영한다 (== 조정하고 있는 캐릭터에 회전을 시키겠지?)
			PC->SetControlRotation(CameraModeView.ControlRotation);
		}
	}

	// Camera의 Location과 Rotation을 반영하자:
	// - 시간 있으면, SetWorldLocationAndRotation을 보면서 -> UpdateChildTransform까지 보는 것을 추천한다
	// - SceneGraph 관계 업데이트를 이해할 수 있다
	// 간단하게 정리하면, CameraComponent에 대해 Parent의 SceneGraph 관계를 고려하여 업데이트 진행한다
	SetWorldLocationAndRotation(CameraModeView.Location, CameraModeView.Rotation);

	// FOV 업데이트
	FieldOfView = CameraModeView.FieldOfView;

	/**
	 * 여기서 우리는 ControlRotation vs Rotation을 차이점을 이해할 수 있다:
	 * - ControlRotation은 PC가 조정하는 Pawn의 Rotation을 적용할 값이다
	 * - 그에 반해, Rotation은 Camera에 반영하는 Rotation이라는 것이다
	 */

	 // FMinimalViewInfo를 업데이트 해준다:
	 // - CameraComponent의 변화 사항을 반영해서 최종 렌더링까지 반영하게 됨
	DesiredView.Location = CameraModeView.Location;
	DesiredView.Rotation = CameraModeView.Rotation;
	DesiredView.FOV = CameraModeView.FieldOfView;
	DesiredView.OrthoWidth = OrthoWidth;
	DesiredView.OrthoNearClipPlane = OrthoNearClipPlane;
	DesiredView.OrthoFarClipPlane = OrthoFarClipPlane;
	DesiredView.AspectRatio = AspectRatio;
	DesiredView.bConstrainAspectRatio = bConstrainAspectRatio;
	DesiredView.bUseFieldOfViewForLOD = bUseFieldOfViewForLOD;
	DesiredView.ProjectionMode = ProjectionMode;
	DesiredView.PostProcessBlendWeight = PostProcessBlendWeight;
	if (PostProcessBlendWeight > 0.0f)
	{
		DesiredView.PostProcessSettings = PostProcessSettings;
	}
}

void ULyraCloneCameraComponent::UpdateCameraModes()
{
	check(CameraModeStack);

	// DetermineCameraModeDelegate는 CameraMode Class를 반환한다:
	// - 해당 delegate는 HeroComponent의 멤버 함수로 바인딩되어 있다
	if (DetermineCameraModeDelegate.IsBound())
	{
		if (TSubclassOf<ULyraCloneCameraMode> CameraMode = DetermineCameraModeDelegate.Execute())
		{
			CameraModeStack->PushCameraMode(CameraMode);
		}
	}
}
