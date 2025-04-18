// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "LyraCloneExperienceDefinition.generated.h"

/** forward declaration */
class ULyraClonePawnData;
class UGameFeatureAction;
class ULyraCloneExperienceActionSet;

/**
 * 
 */
UCLASS()
class LYRACLONE_API ULyraCloneExperienceDefinition : public UPrimaryDataAsset
{
	GENERATED_BODY()
	
public:
	ULyraCloneExperienceDefinition(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

#if WITH_EDITORONLY_DATA
	virtual void UpdateAssetBundleData() override;
#endif

	/**
	* member variables
	*/

	/** the default pawn class to spawn for players */
	UPROPERTY(EditDefaultsOnly, Category = Gameplay)
	TObjectPtr<ULyraClonePawnData> DefaultPawnData;

	/**
	 * 해당 property는 단순히 마킹 및 기억용으로 남겨둔다
	 * - 게임 모드에 따른 GameFeature plugin을 로딩하는데 이에 대한 연결고리고 생각하면 된다
	 * - 후일 ShooterCore 관련 Plugin 작업할 경우, 진행하게 될 예정이다
	 */
	UPROPERTY(EditDefaultsOnly, Category = Gameplay)
	TArray<FString> GameFeaturesToEnable;

	/** ExperienceActionSet은 UGameFeatureAction의 Set이며, Gameplay 용도에 맞게 분류의 목적으로 사용한다 */
	UPROPERTY(EditDefaultsOnly, Category = Gameplay)
	TArray<TObjectPtr<ULyraCloneExperienceActionSet>> ActionSets;

	/** 일반적인 GameFeatureAction으로서 추가 */
	UPROPERTY(EditDefaultsOnly, Instanced, Category = "Actions")
	TArray<TObjectPtr<UGameFeatureAction>> Actions;
};
