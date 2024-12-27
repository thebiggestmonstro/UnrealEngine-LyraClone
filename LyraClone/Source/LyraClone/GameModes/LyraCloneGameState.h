// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "LyraCloneGameState.generated.h"

class ULyraCloneExperienceManagerComponent;
/**
 * 
 */
UCLASS()
class LYRACLONE_API ALyraCloneGameState : public AGameStateBase
{
	GENERATED_BODY()
	
public:
	ALyraCloneGameState();

public:
	UPROPERTY()
	TObjectPtr<ULyraCloneExperienceManagerComponent> ExperienceManagerComponent;
};
