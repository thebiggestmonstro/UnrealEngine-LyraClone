// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Equipment/LyraCloneGameplayAbility_FromEquipment.h"
#include "LyraCloneGameplayAbility_RangedWeapon.generated.h"

class ULyraCloneRangedWeaponInstance;

/** 어디 대상으로 Taget으로 설정할지 옵션들 (Lyra의 경우, 다양한 옵션 존재) */
UENUM(BlueprintType)
enum class ELyraCloneAbilityTargetingSource : uint8
{
	/** Camera 기준으로 진행 */
	CameraTowardsFocus,
};

/**
 * 
 */
UCLASS()
class LYRACLONE_API ULyraCloneGameplayAbility_RangedWeapon : public ULyraCloneGameplayAbility_FromEquipment
{
	GENERATED_BODY()
	
public:
	struct FRangedWeaponFiringInput
	{
		FVector StartTrace;
		FVector EndAim;
		FVector AimDir;
		ULyraCloneRangedWeaponInstance* WeaponData = nullptr;
		bool bCanPlayBulletFX = false;

		FRangedWeaponFiringInput()
			: StartTrace(ForceInitToZero)
			, EndAim(ForceInitToZero)
			, AimDir(ForceInitToZero)
		{}
	};

	ULyraCloneGameplayAbility_RangedWeapon(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	UFUNCTION(BlueprintCallable)
	void StartRangedWeaponTargeting();

	FTransform GetTargetingTransform(APawn* SourcePawn, ELyraCloneAbilityTargetingSource Source);
	FVector GetWeaponTargetingSourceLocation() const;
	void PerformLocalTargeting(TArray<FHitResult>& OutHits);
	void TraceBulletsInCartridge(const FRangedWeaponFiringInput& InputData, TArray<FHitResult>& OutHits);
	FHitResult DoSingleBulletTrace(const FVector& StartTrace, const FVector& EndTrace, float SweepRadius, bool bIsSimulated, TArray<FHitResult>& OutHits) const;
	FHitResult WeaponTrace(const FVector& StartTrace, const FVector& EndTrace, float SweepRadius, bool bIsSimulated, TArray<FHitResult>& OutHitResults) const;
	void AddAdditionalTraceIgnoreActors(FCollisionQueryParams& TraceParams) const;
	ECollisionChannel DetermineTraceChannel(FCollisionQueryParams& TraceParams, bool bIsSimulated) const;
	void OnTargetDataReadyCallback(const FGameplayAbilityTargetDataHandle& InData, FGameplayTag ApplicationTag);

	/** called when target data is ready */
	UFUNCTION(BlueprintImplementableEvent)
	void OnRangeWeaponTargetDataReady(const FGameplayAbilityTargetDataHandle& TargetData);

	ULyraCloneRangedWeaponInstance* GetWeaponInstance();
};
