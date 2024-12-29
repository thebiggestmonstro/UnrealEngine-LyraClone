// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "LyraCloneGameMode.generated.h"

class ULyraCloneExperienceDefinition;
class ULyraClonePawnData;

/**
 * 
 */
UCLASS()
class LYRACLONE_API ALyraCloneGameMode : public AGameModeBase
{
	GENERATED_BODY()
	
public:
	ALyraCloneGameMode();

	/**
	 * AGameModeBase interface
	 */
	virtual void InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage) override;
	virtual void InitGameState() override;
	/** GetDefaultPawnClassForController */
	virtual UClass* GetDefaultPawnClassForController_Implementation(AController* InController) final;
	/** HandleStartingNewPlayer */
	virtual void HandleStartingNewPlayer_Implementation(APlayerController* NewPlayer) final;

	/** SpawnDefaultPawnAtTransform */
	virtual APawn* SpawnDefaultPawnAtTransform_Implementation(AController* NewPlayer, const FTransform& SpawnTransform) final;

	/**
	 * member methods
	*/
	void HandleMatchAssignmentIfNotExpectingOne();
	void OnMatchAssignmentGiven(FPrimaryAssetId ExperienceId);
	bool IsExperienceLoaded() const;
	void OnExperienceLoaded(const ULyraCloneExperienceDefinition* CurrentExperience);
	const ULyraClonePawnData* GetPawnDataForController(const AController* InController) const;
};
