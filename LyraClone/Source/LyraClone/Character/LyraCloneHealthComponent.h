// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/GameFrameworkComponent.h"
#include "Delegates/Delegate.h"
#include "LyraCloneHealthComponent.generated.h"

/** forward declarations */
class ULyraCloneAbilitySystemComponent;
class ULyraCloneHealthSet;
class ULyraCloneHealthComponent;
class AActor;
struct FOnAttributeChangeData;

/** Health 변화 콜백을 위한 델레게이트 */
DECLARE_DYNAMIC_MULTICAST_DELEGATE_FourParams(FLyraCloneHealth_AttributeChanged, ULyraCloneHealthComponent*, HealthComponent, float, OldValue, float, NewValue, AActor*, Instigator);

/**
 * 
 */
UCLASS()
class LYRACLONE_API ULyraCloneHealthComponent : public UGameFrameworkComponent
{
	GENERATED_BODY()
	
public:
	ULyraCloneHealthComponent(const FObjectInitializer& ObjectInitializer);

	/** ASC와 HealthSet 초기화 */
	void InitializeWithAbilitySystem(ULyraCloneAbilitySystemComponent* InASC);
	void UninitializeWithAbilitySystem();

	/** ASC를 통해, HealthSet의 HealthAttribute 변경이 있을때 호출하는 메서드 (내부적으로 OnHealthChanged 호출) */
	void HandleHealthChanged(const FOnAttributeChangeData& ChangeData);

	/** Actor(보통 ACharacter/APawn)의 HealthComponent를 반환 */
	UFUNCTION(BlueprintPure, Category = "Hak|Health")
	static ULyraCloneHealthComponent* FindHealthComponent(const AActor* Actor);

	/** 아래의 UFUNCTION은 HealthSet의 Attribute에 접근하기 위한 BP Accessor 함수들 */
	UFUNCTION(BlueprintCallable, Category = "Hak|Health")
	float GetHealth() const;

	UFUNCTION(BlueprintCallable, Category = "Hak|Health")
	float GetMaxHealth() const;

	UFUNCTION(BlueprintCallable, Category = "Hak|Health")
	float GetHealthNormalized() const;

	/** HealthSet을 접근하기 위한 AbilitySystemComponent */
	UPROPERTY()
	TObjectPtr<ULyraCloneAbilitySystemComponent> AbilitySystemComponent;

	/** 캐싱된 HealthSet 레퍼런스 */
	UPROPERTY()
	TObjectPtr<const ULyraCloneHealthSet> HealthSet;

	/** health 변화에 따른 Delegate(Multicast) */
	UPROPERTY(BlueprintAssignable)
	FLyraCloneHealth_AttributeChanged OnHealthChanged;
};
