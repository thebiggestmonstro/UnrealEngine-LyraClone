// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/LyraCloneActivatableWidget.h"
#include "LyraCloneHUDLayout.generated.h"

/**
 * PrimaryGameLayout의 Layer에 연동할 HUD Layout (CommonActivatableWidget)
 */
UCLASS(Abstract, BlueprintType, Blueprintable, meta = (DisplayName = "LyraClone HUD Layout", Category = "LyraClone|HUD"))
class LYRACLONE_API ULyraCloneHUDLayout : public ULyraCloneActivatableWidget
{
	GENERATED_BODY()

public:
	ULyraCloneHUDLayout(const FObjectInitializer& ObjectInitializer);
};
