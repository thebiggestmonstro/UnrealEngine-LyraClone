// Fill out your copyright notice in the Description page of Project Settings.


#include "CommonLocalPlayer.h"

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