// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "GameplayAbilitySpec.h"
#include "LyraCloneAbilitySet.generated.h"

class ULyraCloneGameplayAbility;
class ULyraCloneAbilitySystemComponent;

/**
 * GameplayAbility의 Wrapper 클래스
 * - 추가적인 커스터마이징이 가능함
 */
USTRUCT(BlueprintType)
struct FLyraCloneAbilitySet_GameplayAbility
{
	GENERATED_BODY()
	
public:
	/** 허용된 GameplayAbility */
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<ULyraCloneGameplayAbility> Ability = nullptr;

	/** Input 처리를 위한 GameplayTag */
	UPROPERTY(EditDefaultsOnly)
	FGameplayTag InputTag;

	/** Ability의 허용 조건 (Level) */
	UPROPERTY(EditDefaultsOnly)
	int32 AbilityLevel = 1;
};

USTRUCT(BlueprintType)
struct FLyraCloneAbilitySet_GrantedHandles
{
	GENERATED_BODY()

	void AddAbilitySpecHandle(const FGameplayAbilitySpecHandle& Handle);

protected:
	/** 허용된 GameplayAbilitySpecHandle(int32) */
	UPROPERTY()
	TArray<FGameplayAbilitySpecHandle> AbilitySpecHandles;
};

/**
 * 
 */
UCLASS()
class LYRACLONE_API ULyraCloneAbilitySet : public UPrimaryDataAsset
{
	GENERATED_BODY()
	
public:
	ULyraCloneAbilitySet(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	/** ASC에 허용가능한 Ability를 추가한다 */
	void GiveToAbilitySystem(ULyraCloneAbilitySystemComponent* LyraCloneASC, FLyraCloneAbilitySet_GrantedHandles* OutGrantedHandles, UObject* SourceObject = nullptr);

	/** 허용된 GameplayAbilities */
	UPROPERTY(EditDefaultsOnly, Category = "Gameplay Abilities")
	TArray<FLyraCloneAbilitySet_GameplayAbility> GrantedGameplayAbilities;
};
