// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/LyraClonePlayerState.h"
#include "GameModes/LyraCloneExperienceManagerComponent.h"
#include "GameModes/LyraCloneExperienceDefinition.h"
#include "Character/LyraClonePawnData.h"
#include "GameModes/LyraCloneGameMode.h"
#include "AbilitySystem/LyraCloneAbilitySystemComponent.h"
#include "AbilitySystem/Attributes/LyraCloneHealthSet.h"
#include "AbilitySystem/Attributes/LyraCloneCombatSet.h"
#include "Abilities/GameplayAbilityTypes.h"
#include "AbilitySystem/LyraCloneAbilitySet.h"

ALyraClonePlayerState::ALyraClonePlayerState(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	AbilitySystemComponent = ObjectInitializer.CreateDefaultSubobject<ULyraCloneAbilitySystemComponent>(this, TEXT("AbilitySystemComponent"));

	CreateDefaultSubobject<ULyraCloneHealthSet>(TEXT("HealthSet"));
	CreateDefaultSubobject<ULyraCloneCombatSet>(TEXT("CombatSet"));
}

void ALyraClonePlayerState::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	check(AbilitySystemComponent);
	// 아래의 코드는 우리가 InitAbilityActorInfo를 재호출을 통하는 이유를 설명하는 코드이다:
	{
		// 처음 InitAbilityActorInfo를 호출 당시, OwnerActor와 AvatarActo가 같은 Actor를 가르키고 있으며, 이는 PlayerState이다
		// - OwnerActor는 PlayerState가 의도하는게 맞지만, AvatarActor는 PlayerController가 소유하는 대상인 Pawn이 되어야 한다!
		// - 이를 위해 재-세팅을 해준다
		FGameplayAbilityActorInfo* ActorInfo = AbilitySystemComponent->AbilityActorInfo.Get();
		check(ActorInfo->OwnerActor == this);
		check(ActorInfo->OwnerActor == ActorInfo->AvatarActor);
	}
	AbilitySystemComponent->InitAbilityActorInfo(this, GetPawn());

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

	// PawnData의 AbilitySet을 순회하며, ASC에 Ability를 할당(Give)한다
	// - 이 과정에서 ASC의 ActivatableAbilities에 추가된다
	for (ULyraCloneAbilitySet* AbilitySet : PawnData->AbilitySets)
	{
		if (AbilitySet)
		{
			AbilitySet->GiveToAbilitySystem(AbilitySystemComponent, nullptr);
		}
	}
}