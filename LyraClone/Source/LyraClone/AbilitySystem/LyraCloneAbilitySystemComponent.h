// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "LyraCloneAbilitySystemComponent.generated.h"

/**
 * 
 */
UCLASS()
class LYRACLONE_API ULyraCloneAbilitySystemComponent : public UAbilitySystemComponent
{
	GENERATED_BODY()
	
public:
	ULyraCloneAbilitySystemComponent(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());
};
