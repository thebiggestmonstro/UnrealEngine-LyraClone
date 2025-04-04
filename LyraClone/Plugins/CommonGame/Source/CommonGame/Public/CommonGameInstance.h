// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "CommonGameInstance.generated.h"

/**
 * 
 */
UCLASS(Abstract)
class COMMONGAME_API UCommonGameInstance : public UGameInstance
{
	GENERATED_BODY()
	
public:
	/**
	 * GameInstance's interfaces
	 */
	virtual int32 AddLocalPlayer(ULocalPlayer* NewPlayer, FPlatformUserId UserId) override;
	virtual bool RemoveLocalPlayer(ULocalPlayer* ExistingPlayer) override;

	/** 메인 로컬 플레이어를 캐싱한다 */
	TWeakObjectPtr<ULocalPlayer> PrimaryPlayer;
};
