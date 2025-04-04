// Fill out your copyright notice in the Description page of Project Settings.


#include "CommonPlayerController.h"
#include "CommonLocalPlayer.h"

void ACommonPlayerController::ReceivedPlayer()
{
	Super::ReceivedPlayer();

	// PlayerController가 LocalPlayer에 붙으면 활성화되는 이벤트가 ReceivedPlayer()이다
	if (UCommonLocalPlayer* LocalPlayer = Cast<UCommonLocalPlayer>(Player))
	{
		LocalPlayer->OnPlayerControllerSet.Broadcast(LocalPlayer, this);
	}
}