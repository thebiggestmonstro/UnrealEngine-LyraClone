// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Attributes/LyraCloneAttributeSet.h"
#include "LyraCloneHealthSet.generated.h"

/**
 * 
 */
UCLASS()
class LYRACLONE_API ULyraCloneHealthSet : public ULyraCloneAttributeSet
{
	GENERATED_BODY()
	
public:
	ULyraCloneHealthSet();

	/**
	* 앞서 LyraCloneAttributeSet에서 정의했던, ATTRIBUTE_ACCESSORS를 통해, 아래 정의함 멤벼변수와 똑같이 이름을 설정한다
	* - ATTRIBUTE_ACCESSORS의 Macro의 정의부분을 한번 실펴보자
	*/
	ATTRIBUTE_ACCESSORS(ULyraCloneHealthSet, Health);
	ATTRIBUTE_ACCESSORS(ULyraCloneHealthSet, MaxHealth);
	ATTRIBUTE_ACCESSORS(ULyraCloneHealthSet, Healing);

	/** 현재 체력 */
	UPROPERTY(BlueprintReadOnly, Category = "Hak|Health")
	FGameplayAttributeData Health;

	/** 체력 최대치 */
	UPROPERTY(BlueprintReadOnly, Category = "Hak|Health")
	FGameplayAttributeData MaxHealth;

	/** 체력 회복치 */
	UPROPERTY(BlueprintReadOnly, Category = "Hak|Health")
	FGameplayAttributeData Healing;
};
