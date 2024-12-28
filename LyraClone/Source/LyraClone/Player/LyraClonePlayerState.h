// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "LyraClonePlayerState.generated.h"

class ULyraCloneExperienceDefinition;
class ULyraClonePawnData;

/**
 * 
 */
UCLASS()
class LYRACLONE_API ALyraClonePlayerState : public APlayerState
{
	GENERATED_BODY()
	
public:
	/**
	 * AActor's interface
	 */
	virtual void PostInitializeComponents() final;

	/**
	 * member methods
	 */
	void OnExperienceLoaded(const ULyraCloneExperienceDefinition* CurrentExperience);

	UPROPERTY()
	TObjectPtr<const ULyraClonePawnData> PawnData;
};
