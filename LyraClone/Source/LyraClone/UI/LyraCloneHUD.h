// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "LyraCloneHUD.generated.h"

/**
 * 
 */
UCLASS()
class LYRACLONE_API ALyraCloneHUD : public AHUD
{
	GENERATED_BODY()
	
public:
	ALyraCloneHUD(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	/**
	 * GameFrameworkComponentManager의 AddReceiver를 위한 메서드들
	 */
	virtual void PreInitializeComponents() override;
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
};
