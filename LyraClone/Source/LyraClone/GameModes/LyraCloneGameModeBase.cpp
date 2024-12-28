// Fill out your copyright notice in the Description page of Project Settings.


#include "LyraCloneGameModeBase.h"

#include "LyraCloneGameState.h"
#include "Character/LyraCloneCharacter.h"
#include "Player/LyraClonePlayerController.h"
#include "Player/LyraClonePlayerState.h"
#include "LyraCloneExperienceManagerComponent.h"
#include "LyraCloneExperienceDefinition.h"
#include "LyraCloneLogChannels.h"

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

void ALyraCloneGameModeBase::InitGameState()
{
	Super::InitGameState();
	// Experience 비동기 로딩을 위한 Delegate를 준비한다:
	ULyraCloneExperienceManagerComponent* ExperienceManagerComponent = GameState->FindComponentByClass<ULyraCloneExperienceManagerComponent>();
	check(ExperienceManagerComponent);

	// OnExperienceLoaded 등록
	ExperienceManagerComponent->CallOrRegister_OnExperienceLoaded(FOnLyraCloneExperienceLoaded::FDelegate::CreateUObject(this, &ThisClass::OnExperienceLoaded));
}

void ALyraCloneGameModeBase::HandleStartingNewPlayer_Implementation(APlayerController* NewPlayer)
{
	// Experience가 로드되어있다면, Player에 대한 생성을 진행
	if (IsExperienceLoaded())
	{
		Super::HandleStartingNewPlayer_Implementation(NewPlayer);
	}
}

APawn* ALyraCloneGameModeBase::SpawnDefaultPawnAtTransform_Implementation(AController* NewPlayer, const FTransform& SpawnTransform)
{
	UE_LOG(LogLyraClone, Log, TEXT("SpawnDefaultPawnAtTransform_Implementation is called"));
	return Super::SpawnDefaultPawnAtTransform_Implementation(NewPlayer, SpawnTransform);
}

void ALyraCloneGameModeBase::HandleMatchAssignmentIfNotExpectingOne()
{
	
}

bool ALyraCloneGameModeBase::IsExperienceLoaded() const
{
	check(GameState);
	ULyraCloneExperienceManagerComponent* ExperienceManagerComponent = GameState->FindComponentByClass<ULyraCloneExperienceManagerComponent>();
	check(ExperienceManagerComponent);

	return ExperienceManagerComponent->IsExperienceLoaded();
}

void ALyraCloneGameModeBase::OnExperienceLoaded(const ULyraCloneExperienceDefinition* CurrentExperience)
{

}