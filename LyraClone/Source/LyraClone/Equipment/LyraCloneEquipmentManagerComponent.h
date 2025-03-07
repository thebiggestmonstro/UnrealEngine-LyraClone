// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/PawnComponent.h"
#include "LyraCloneEquipmentManagerComponent.generated.h"

/** forward declarations */
class ULyraCloneEquipmentDefinition;
class ULyraCloneEquipmentInstance;

USTRUCT(BlueprintType)
struct FLyraCloneAppliedEquipmentEntry
{
	GENERATED_BODY()

	/** 장착물에 대한 메타 데이터 */
	UPROPERTY()
	TSubclassOf<ULyraCloneEquipmentDefinition> EquipmentDefinition;

	/** EquipmentDefinition을 통해 생성된 인스턴스 */
	UPROPERTY()
	TObjectPtr<ULyraCloneEquipmentInstance> Instance = nullptr;
};

/**
 * 참고로 EquipmentInstance의 인스턴스를 Entry에서 관리하고 있다:
 * - LyraCloneEquipmentList는 생성된 객체를 관리한다고 보면 된다
 */
USTRUCT(BlueprintType)
struct FLyraCloneEquipmentList
{
	GENERATED_BODY()

	FLyraCloneEquipmentList(UActorComponent* InOwnerComponent = nullptr)
		: OwnerComponent(InOwnerComponent)
	{}

	ULyraCloneEquipmentInstance* AddEntry(TSubclassOf<ULyraCloneEquipmentDefinition> EquipmentDefinition);
	void RemoveEntry(ULyraCloneEquipmentInstance* Instance);

	/** 장착물에 대한 관리 리스트 */
	UPROPERTY()
	TArray<FLyraCloneAppliedEquipmentEntry> Entries;

	UPROPERTY()
	TObjectPtr<UActorComponent> OwnerComponent;
};

/**
 * 
 */
UCLASS()
class LYRACLONE_API ULyraCloneEquipmentManagerComponent : public UPawnComponent
{
	GENERATED_BODY()
	
public:
	ULyraCloneEquipmentManagerComponent(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	ULyraCloneEquipmentInstance* EquipItem(TSubclassOf<ULyraCloneEquipmentDefinition> EquipmentDefinition);
	void UnequipItem(ULyraCloneEquipmentInstance* ItemInstance);

	UPROPERTY()
	FLyraCloneEquipmentList EquipmentList;
};
