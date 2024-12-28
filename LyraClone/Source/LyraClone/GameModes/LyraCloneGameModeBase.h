// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "LyraCloneGameModeBase.generated.h"

class ULyraCloneExperienceDefinition;

/**
 * 
 */
UCLASS()
class LYRACLONE_API ALyraCloneGameModeBase : public AGameModeBase
{
	GENERATED_BODY()
	
public:
	ALyraCloneGameModeBase();

	virtual void InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage) override;
	virtual void InitGameState() override;

	/** HandleStartingNewPlayer */
	virtual void HandleStartingNewPlayer_Implementation(APlayerController* NewPlayer) final;

	/** SpawnDefaultPawnAtTransform */
	virtual APawn* SpawnDefaultPawnAtTransform_Implementation(AController* NewPlayer, const FTransform& SpawnTransform) final;

	/**
	 * member methods
	*/
	void HandleMatchAssignmentIfNotExpectingOne();
	bool IsExperienceLoaded() const;
	void OnExperienceLoaded(const ULyraCloneExperienceDefinition* CurrentExperience);
};
