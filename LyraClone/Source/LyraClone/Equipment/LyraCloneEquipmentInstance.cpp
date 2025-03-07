// Fill out your copyright notice in the Description page of Project Settings.


#include "Equipment/LyraCloneEquipmentInstance.h"
#include "LyraCloneEquipmentDefinition.h"
#include "GameFramework/Character.h"

ULyraCloneEquipmentInstance::ULyraCloneEquipmentInstance(const FObjectInitializer& ObjectInitializer)
	:  Super(ObjectInitializer)
{

}

APawn* ULyraCloneEquipmentInstance::GetPawn() const
{
	return Cast<APawn>(GetOuter());
}

void ULyraCloneEquipmentInstance::SpawnEquipmentActors(const TArray<FLyraCloneEquipmentActorToSpawn>& ActorsToSpawn)
{
	if (APawn* OwningPawn = GetPawn())
	{
		// 현재 Owner인 Pawn의 RootComponent를 AttachTarget 대상으로 한다
		USceneComponent* AttachTarget = OwningPawn->GetRootComponent();
		if (ACharacter* Char = Cast<ACharacter>(OwningPawn))
		{
			// 만약 캐릭터라면, SkeletalMeshComponent가 있으면 GetMesh로 반환하여, 여기에 붙인다
			AttachTarget = Char->GetMesh();
		}

		for (const FLyraCloneEquipmentActorToSpawn& SpawnInfo : ActorsToSpawn)
		{
			// SpawnActorDeferred는 FinishSpawning을 호출해야 Spawn이 완성된다 (즉, 작성자에게 코드로서 Ownership이 있다는 의미)
			AActor* NewActor = GetWorld()->SpawnActorDeferred<AActor>(SpawnInfo.ActorToSpawn, FTransform::Identity, OwningPawn);
			NewActor->FinishSpawning(FTransform::Identity, /*bIsDefaultTransform=*/true);

			// Actor의 RelativeTransform을 AttachTransform으로 설정
			NewActor->SetActorRelativeTransform(SpawnInfo.AttachTransform);

			// AttachTarget에 붙이자 (Actor -> Actor)
			NewActor->AttachToComponent(AttachTarget, FAttachmentTransformRules::KeepRelativeTransform, SpawnInfo.AttachSocket);

			SpawnedActors.Add(NewActor);
		}
	}
}

void ULyraCloneEquipmentInstance::DestroyEquipmentActors()
{
	// 참고로 장착물이 복수개의 Actor Mesh로 구성되어 있을 수도 있다
	// - 갑옷 Lv10이었지만, 상체와 하체로 같이 구성되어있을 수도 있으니깐?
	for (AActor* Actor : SpawnedActors)
	{
		if (Actor)
		{
			Actor->Destroy();
		}
	}
}

void ULyraCloneEquipmentInstance::OnEquipped()
{
	K2_OnEquipped();
}

void ULyraCloneEquipmentInstance::OnUnequipped()
{
	K2_OnUnequipped();
}