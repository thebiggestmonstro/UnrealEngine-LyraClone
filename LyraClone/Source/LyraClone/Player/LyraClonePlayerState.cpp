// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/LyraClonePlayerState.h"
#include "GameModes/LyraCloneExperienceManagerComponent.h"
#include "GameModes/LyraCloneExperienceDefinition.h"
#include "Character/LyraClonePawnData.h"
#include "GameModes/LyraCloneGameMode.h"

void ALyraClonePlayerState::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	AGameStateBase* GameState = GetWorld()->GetGameState();
	check(GameState);

	ULyraCloneExperienceManagerComponent* ExperienceManagerComponent = GameState->FindComponentByClass<ULyraCloneExperienceManagerComponent>();
	check(ExperienceManagerComponent);

	ExperienceManagerComponent->CallOrRegister_OnExperienceLoaded(
		FOnLyraCloneExperienceLoaded::FDelegate::CreateUObject(this, &ThisClass::OnExperienceLoaded)
	);
}

void ALyraClonePlayerState::OnExperienceLoaded(const ULyraCloneExperienceDefinition* CurrentExperience)
{
	if (ALyraCloneGameMode* GameMode = GetWorld()->GetAuthGameMode<ALyraCloneGameMode>())
	{
		// ALyraCloneGameMode에서 GetPawnDataForController를 구현해야 함
		// - GetPawnDataForController에서 우리는 아직 PawnData를 설정하지 않았으므로, ExperienceMangerComponent의 DefaultPawnData로 설정한다
		const ULyraClonePawnData* NewPawnData = GameMode->GetPawnDataForController(GetOwningController());
		check(NewPawnData);

		SetPawnData(NewPawnData);
	}
}

void ALyraClonePlayerState::SetPawnData(const ULyraClonePawnData* InPawnData)
{
	check(InPawnData);

	// PawnData가 두번 설정되는 것은 원하지 않음!
	check(!PawnData);

	PawnData = InPawnData;
}