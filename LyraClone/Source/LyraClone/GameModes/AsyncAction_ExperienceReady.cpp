// Fill out your copyright notice in the Description page of Project Settings.


#include "GameModes/AsyncAction_ExperienceReady.h"
#include "LyraCloneExperienceManagerComponent.h"

UAsyncAction_ExperienceReady::UAsyncAction_ExperienceReady(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{}

UAsyncAction_ExperienceReady* UAsyncAction_ExperienceReady::WaitForExperienceReady(UObject* WorldContextObject)
{
	UAsyncAction_ExperienceReady* Action = nullptr;
	if (UWorld* World = GEngine->GetWorldFromContextObject(WorldContextObject, EGetWorldErrorMode::LogAndReturnNull))
	{
		Action = NewObject<UAsyncAction_ExperienceReady>();
		Action->WorldPtr = World;
		Action->RegisterWithGameInstance(World);
	}
	return Action;
}

void UAsyncAction_ExperienceReady::Activate()
{
	if (UWorld* World = WorldPtr.Get())
	{
		// GameState가 이미 World에 준비되어 있으면, Step1을 스킵하고 바로 Step2를 진행한다
		if (AGameStateBase* GameState = World->GetGameState())
		{
			Step2_ListenToExperienceLoading(GameState);
		}
		else
		{
			// 아직 준비되어 있지 않으면, UWorld::GameStateSetEvent에 Step1_HandleGameStateSet을 Delegate로 바인딩시킨다
			// - Step1부터 시작할 것이다
			World->GameStateSetEvent.AddUObject(this, &ThisClass::Step1_HandleGameStateSet);
		}
	}
	else
	{
		// 현재 AsyncAction 대상이었던 World가 더이상 Valid하지 않으므로, 제거 진행
		// - 코드를 내려가보면, GameInstance에서 레퍼런스 대상으로 제거시킨다
		// - UAsyncAction의 캐싱 위치가 GameInstance임을 알 수 있다
		SetReadyToDestroy();
	}
}

void UAsyncAction_ExperienceReady::Step1_HandleGameStateSet(AGameStateBase* GameState)
{
	if (UWorld* World = WorldPtr.Get())
	{
		World->GameStateSetEvent.RemoveAll(this);
	}

	Step2_ListenToExperienceLoading(GameState);
}

void UAsyncAction_ExperienceReady::Step2_ListenToExperienceLoading(AGameStateBase* GameState)
{
	check(GameState);
	// 여기서 왜 우리가 GameStateSetEvent에 Delegate로 바인딩시켰는지 이유가 나온다:
	// - Expierence 로딩 상태 정보가 GameState에 붙어있는 ExperienceManagerComponent에서 가져올 수 있기 때문이다!
	ULyraCloneExperienceManagerComponent* ExperienceManagerComponent = GameState->FindComponentByClass<ULyraCloneExperienceManagerComponent>();
	check(ExperienceManagerComponent);

	// 만약 이미 Experience가 준비되었다면, Step3를 스킵하고 Step4로 간다
	if (ExperienceManagerComponent->IsExperienceLoaded())
	{
		UWorld* World = GameState->GetWorld();
		check(World);

		// 캐릭터의 Mesh는 PawnComponent_CharacterParts에 의해서 결정이 된다.
		// 정확하게 말하면 ControllerComponent_CharacterParts에서 설정된 Character Part의 정보에 따라서 Mesh를 선택해서 결정해준다.
		// ControllerComponent_CharacterParts가 설정하는 시점에는 Pawn이 Possess되지 않기 때문에 UPawnComponent_CharacterParts을 호출할 수 없어서 결론적으로 Mesh 설정이 불가능하다.
		// 그래서 ControllerComponent_CharacterParts는 OnPossessedPawnChanged 델리게이트를 통해 Possess되는 시점에 발동하도록 설정하는데
		// 여기서 문제가 발생하게 된다 왜냐면 Possess 함수는 내부적으로 OnPossessedPawnChanged 델리게이트를 Possess 가장 마지막 단계로 호출하기 때문에
		// 현재 캐릭터의 PossessedBy를 먼저 호출하게 된다 그렇게 되면 Mesh가 설정이 되어 있지 않아서 아이템 장착할 때 AnimLayer 그리고 장비 아이템 장착이 불가능하다.
		// 그래서 해결 방법으로 현 프레임이 아닌 안전하게 다음 프레임에서 처리하도록 해서 OnPossessedPawnChanged가 먼저 호출 되도록 유도한다.
		// 아미 Experience가 준비되었다고 해도, 아직 준비되지 않은 상태가 있을 수도 있으니 (혹은 과정중이라던가?) 그래서 그냥 다음 Tick에 실행되도록 Timer를 바인딩시킨다
		World->GetTimerManager().SetTimerForNextTick(FTimerDelegate::CreateUObject(this, &ThisClass::Step4_BroadcastReady));
	}
	else
	{
		// 준비가 안되었다면, OnExperienceLoaded에 바인딩시킨다 (후일 로딩 끝나면 Step3부터 실행이 될 것이다)
		ExperienceManagerComponent->CallOrRegister_OnExperienceLoaded(FOnLyraCloneExperienceLoaded::FDelegate::CreateUObject(this, &ThisClass::Step3_HandleExperienceLoaded));
	}
}

void UAsyncAction_ExperienceReady::Step3_HandleExperienceLoaded(const ULyraCloneExperienceDefinition* CurrentExperience)
{
	// 현재 따로 ExperienceDefinition에 대해 처리할 로직은 없다

	Step4_BroadcastReady();
}

void UAsyncAction_ExperienceReady::Step4_BroadcastReady()
{
	// 바인딩된 BP 혹은 UFUNCTION을 호출해준다
	OnReady.Broadcast();
	SetReadyToDestroy();
}
