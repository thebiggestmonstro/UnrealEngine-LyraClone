// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "LyraCloneCharacterPartTypes.h"
#include "LyraCloneCosmeticAnimationTypes.h"
#include "Components/PawnComponent.h"
#include "LyraClonePawnComponent_CharacterParts.generated.h"

class ULyraClonePawnComponent_CharacterParts;

/** 인스턴스화 된 Character Part의 단위 */
USTRUCT()
struct FLyraCloneAppliedCharacterPartEntry
{
	GENERATED_BODY()

	/** Character Part의 정의(메타 데이터) */
	UPROPERTY()
	FLyraCloneCharacterPart Part;

	/** LyraCloneCharacterPartList에서 할당 받은 Part 핸들값 (FLyraCloneControllerCharacterPartEntry의 Handle 값과 같아야 함 -> 같으면 같은 Part) */
	UPROPERTY()
	int32 PartHandle = INDEX_NONE;

	/** 인스턴스화 된 Character Part용 Actor */
	UPROPERTY()
	TObjectPtr<UChildActorComponent> SpawnedComponent = nullptr;
};

/** LyraClonePawnComponent_CharacterParts에서 실질적 Character Parts를 관리하는 클래스 */
USTRUCT(BlueprintType)
struct FLyraCloneCharacterPartList
{
	GENERATED_BODY()

	FLyraCloneCharacterPartList()
		: OwnerComponent(nullptr)
	{}

	FLyraCloneCharacterPartList(ULyraClonePawnComponent_CharacterParts* InOwnerComponent)
		: OwnerComponent(InOwnerComponent)
	{}

	bool SpawnActorForEntry(FLyraCloneAppliedCharacterPartEntry& Entry);
	void DestroyActorForEntry(FLyraCloneAppliedCharacterPartEntry& Entry);

	FLyraCloneCharacterPartHandle AddEntry(FLyraCloneCharacterPart NewPart);
	void RemoveEntry(FLyraCloneCharacterPartHandle Handle);

	FGameplayTagContainer CollectCombinedTags() const;

	/** 현재 인스턴스화된 Character Part */
	UPROPERTY()
	TArray<FLyraCloneAppliedCharacterPartEntry> Entries;

	/** 해당 LyraCloneCharacterPartList의 Owner인 PawnComponent */
	UPROPERTY()
	TObjectPtr<ULyraClonePawnComponent_CharacterParts> OwnerComponent;

	/** 앞서 보았던 PartHandle의 값을 할당 및 관리하는 변수 */
	int32 PartHandleCounter = 0;
};

/**
 * 
 */
UCLASS()
class LYRACLONE_API ULyraClonePawnComponent_CharacterParts : public UPawnComponent
{
	GENERATED_BODY()
public:
	ULyraClonePawnComponent_CharacterParts(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	USkeletalMeshComponent* GetParentMeshComponent() const;
	USceneComponent* GetSceneComponentToAttachTo() const;

	UFUNCTION(BlueprintCallable, BlueprintPure = false, Category = Cosmetics)
	FGameplayTagContainer GetCombinedTags(FGameplayTag RequiredPrefix) const;
	void BroadcastChanged();

	FLyraCloneCharacterPartHandle AddCharacterPart(const FLyraCloneCharacterPart& NewPart);
	void RemoveCharacterPart(FLyraCloneCharacterPartHandle Handle);

	/** 인스턴스화 된 Character Parts */
	UPROPERTY()
	FLyraCloneCharacterPartList CharacterPartList;

	/** 애니메이션 적용을 위한 메시와 연결고리 */
	UPROPERTY(EditAnywhere, Category = Cosmetics)
	FLyraCloneAnimBodyStyleSelectionSet BodyMeshes;
};
