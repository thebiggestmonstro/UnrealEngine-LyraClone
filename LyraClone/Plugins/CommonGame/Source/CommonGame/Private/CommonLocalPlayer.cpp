// Fill out your copyright notice in the Description page of Project Settings.


#include "CommonLocalPlayer.h"

UCommonLocalPlayer::UCommonLocalPlayer()
// 여기 Super()로 넘기는 과정에서 FObjectInitializer::Get() 통해 넘겼다
// - 이는 우리가 UCommonLocalPlayer()를 상속받는 LocalPlayer는 굳이 FObjectInitializer를 신경 쓸 필요가 없다.
// - 만약 상속받는 LocalPlayer에서 FObjectInitializer를 통해 뭔가 추가 SubObject를 정의한다면 이와 같은 구현을 수정해야 한다
	: Super(FObjectInitializer::Get())
{}

FDelegateHandle UCommonLocalPlayer::CallAndRegister_OnPlayerStateSet(FPlayerStateSetDelegate::FDelegate Delegate)
{
	APlayerController* PC = GetPlayerController(GetWorld());
	APlayerState* PlayerState = PC ? PC->PlayerState : nullptr;
	if (PlayerState)
	{
		// PlayerState가 설정되어 있으면 바로 호출
		Delegate.Execute(this, PlayerState);
	}
	// OnPlayerStateSet에 등록
	return OnPlayerStateSet.Add(Delegate);
}