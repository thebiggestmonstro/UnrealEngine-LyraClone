// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "LyraCloneGameplayAbility.generated.h"

UENUM(BlueprintType)
enum class ELyraCloneAbilityActivationPolicy : uint8
{
	/** Input이 Trigger 되었을 경우 (Pressed/Released) */
	OnInputTriggered,
	/** Input이 Held되어 있을 경우 */
	WhileInputActive,
	/** avatar가 생성되었을 경우, 바로 할당 */
	OnSpawn,
};

/**
 * 
 */
UCLASS()
class LYRACLONE_API ULyraCloneGameplayAbility : public UGameplayAbility
{
	GENERATED_BODY()
	
public:
	ULyraCloneGameplayAbility(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	/** 언제 GA가 활성화될지 정책 */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "LyraClone|AbilityActivation")
	ELyraCloneAbilityActivationPolicy ActivationPolicy;
};
