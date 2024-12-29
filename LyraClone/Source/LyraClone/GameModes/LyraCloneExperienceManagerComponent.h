// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/GameStateComponent.h"
#include "LyraCloneExperienceManagerComponent.generated.h"

class ULyraCloneExperienceDefinition;

// CurrentExperience에 대한 상태
enum class ELyraCloneExperienceLoadState
{
	Unloaded,
	Loading,
	Loaded,
	Deactivating,
};

// 로드를 완료한 Experience를 알려주기 위한 델리게이트
DECLARE_MULTICAST_DELEGATE_OneParam(FOnLyraCloneExperienceLoaded, const ULyraCloneExperienceDefinition*);

/**
 * LyraCloneExperienceManagerComponent
 * - 말 그대로, 해당 component는 game state를 owner로 가지면서, experience의 상태 정보를 가지고 있는 component이다
 * - 뿐만 아니라, manager라는 단어가 포함되어 있듯이, experience 로딩 상태 업데이트 및 이벤트를 관리한다
 */
UCLASS()
class LYRACLONE_API ULyraCloneExperienceManagerComponent : public UGameStateComponent
{
	GENERATED_BODY()
	
public:
	/**
	 * member methods
	 */
	bool IsExperienceLoaded() { return (LoadState == ELyraCloneExperienceLoadState::Loaded) && (CurrentExperience != nullptr); }

	/**
	 * 아래의 OnExperienceLoaded에 바인딩하거나, 이미 Experience 로딩이 완료되었다면 바로 호출함
	 */
	void CallOrRegister_OnExperienceLoaded(FOnLyraCloneExperienceLoaded::FDelegate&& Delegate);

	void ServerSetCurrentExperience(FPrimaryAssetId ExperienceId);
	void StartExperienceLoad();
	void OnExperienceLoadComplete();
	void OnExperienceFullLoadCompleted();
	const ULyraCloneExperienceDefinition* GetCurrentExperienceChecked() const;

public:
	UPROPERTY()
	TObjectPtr<const ULyraCloneExperienceDefinition> CurrentExperience;

	/** Experience의 로딩 상태를 모니터링 */
	ELyraCloneExperienceLoadState LoadState = ELyraCloneExperienceLoadState::Unloaded;

	/** Experience 로딩이 완료된 이후, Broadcasting Delegate */
	FOnLyraCloneExperienceLoaded OnExperienceLoaded;
};
