// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "LyraCloneCharacterPartTypes.generated.h"

USTRUCT(BlueprintType)
struct FLyraCloneCharacterPartHandle
{
	GENERATED_BODY()

	void Reset()
	{
		PartHandle = INDEX_NONE;
	}

	bool IsValid() const
	{
		return PartHandle != INDEX_NONE;
	}

	UPROPERTY()
	int32 PartHandle = INDEX_NONE;
};

USTRUCT(BlueprintType)
struct FLyraCloneCharacterPart
{
	GENERATED_BODY()

	/** 파츠로 생성된 Actor의 Class 정보를 가지고 있음 -> Actor BP를 하나의 파츠로 생각하면 됨 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<AActor> PartClass;

	/** Actor를 소환하여 붙이는데 특정 Bone에 붙이기 위한 정보 (따로 정의되어 있지 않으면 그냥 Root에 붙임) */
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName SocketName;
};