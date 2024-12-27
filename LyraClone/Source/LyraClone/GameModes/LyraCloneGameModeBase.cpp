// Fill out your copyright notice in the Description page of Project Settings.


#include "LyraCloneGameModeBase.h"

#include "LyraCloneGameState.h"
#include "Character/LyraCloneCharacter.h"
#include "Player/LyraClonePlayerController.h"
#include "Player/LyraClonePlayerState.h"

ALyraCloneGameModeBase::ALyraCloneGameModeBase()
{
	GameStateClass = ALyraCloneGameState::StaticClass();
	PlayerControllerClass = ALyraClonePlayerController::StaticClass();
	PlayerStateClass = ALyraClonePlayerState::StaticClass();
	DefaultPawnClass = ALyraCloneCharacter::StaticClass();
}

void ALyraCloneGameModeBase::InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage)
{
	Super::InitGame(MapName, Options, ErrorMessage);

	// 아직 GameInstance를 통해, 초기화 작업이 진행되므로, 현 프레임에는 Lyra의 Concept인 Experience 처리를 진행할 수 없다:
	// - 이를 처리하기 위해, 한프레임 뒤에 이벤트를 받아 처리를 이어서 진행한다
	// - 정리하면 다음 프레임에서 HandleMatchAssignmentIfNotExpectingOne을 처리한다
	GetWorld()->GetTimerManager().SetTimerForNextTick(this, &ThisClass::HandleMatchAssignmentIfNotExpectingOne);
}

void ALyraCloneGameModeBase::HandleMatchAssignmentIfNotExpectingOne()
{

}
