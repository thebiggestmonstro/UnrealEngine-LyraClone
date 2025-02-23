// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "LyraCloneAnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class LYRACLONE_API ULyraCloneAnimInstance : public UAnimInstance
{
	GENERATED_BODY()
	
public:
	/** 해당 속성값은 Lyra의 AnimBP에서 사용되는 값이므로 정의해주자 */
	UPROPERTY(BlueprintReadOnly, Category = "Character State Data")
	float GroundDistance = -1.0f;
};
