// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "LyraCloneGameModeBase.generated.h"

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

	/**
	 * member methods
	*/
	void HandleMatchAssignmentIfNotExpectingOne();
};
