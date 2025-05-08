// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "GameplayTagContainer.h"
#include "LyraCloneGameplayTagStack.generated.h"

/**
 * Represents one stack of a gameplay tag (tag + count)
 * : for example, Ammo is representative example for GameplayTagStack
 */
USTRUCT(BlueprintType)
struct FLyraCloneGameplayTagStack
{
	GENERATED_BODY()

	FLyraCloneGameplayTagStack() {}
	FLyraCloneGameplayTagStack(FGameplayTag InTag, int32 InStackCount)
		: Tag(InTag)
		, StackCount(InStackCount)
	{}

	UPROPERTY()
	FGameplayTag Tag;

	UPROPERTY()
	int32 StackCount = 0;
};

/** container of LyraCloneGameplayTagStack */
USTRUCT(BlueprintType)
struct FLyraCloneGameplayTagStackContainer
{
	GENERATED_BODY()

	FLyraCloneGameplayTagStackContainer() {}

	/** add/remove stack count by gameplay-tag */
	void AddStack(FGameplayTag Tag, int32 StackCount);
	void RemoveStack(FGameplayTag Tag, int32 StackCount);

	/** get the count by the gameplay tag */
	int32 GetStackCount(FGameplayTag Tag) const
	{
		return TagToCountMap.FindRef(Tag);
	}

	/** whether gameplay tag exists in LyraCloneGameplayTagStackContainer */
	bool ContainsTag(FGameplayTag Tag) const
	{
		return TagToCountMap.Contains(Tag);
	}

	/** a list of gameplay tag stacks */
	UPROPERTY()
	TArray<FLyraCloneGameplayTagStack> Stacks;

	/**
	 * LUT(Look-up table) to accelerate gameplay tag stack to query [GameplayTag -> Count]
	 * - we also use this LUT to find existance for corresponding gameplay tag
	 */
	TMap<FGameplayTag, int32> TagToCountMap;
};
