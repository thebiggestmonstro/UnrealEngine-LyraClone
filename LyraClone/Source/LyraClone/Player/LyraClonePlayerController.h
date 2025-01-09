// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "LyraClonePlayerController.generated.h"

/**
 * 
 */
UCLASS()
class LYRACLONE_API ALyraClonePlayerController : public APlayerController
{
	GENERATED_BODY()
	
public:
	ALyraClonePlayerController(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());
};
