// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Weapons/LyraCloneWeaponUserInterface.h"
#include "Equipment/LyraCloneEquipmentManagerComponent.h"
#include "Weapons/LyraCloneWeaponInstance.h"

ULyraCloneWeaponUserInterface::ULyraCloneWeaponUserInterface(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{

}

void ULyraCloneWeaponUserInterface::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	// Pawn을 가져오고
	if (APawn* Pawn = GetOwningPlayerPawn())
	{
		// EquipmentManaterComponent를 활용하여, WeaponInstance를 가져오자
		if (ULyraCloneEquipmentManagerComponent* EquipmentManager = Pawn->FindComponentByClass<ULyraCloneEquipmentManagerComponent>())
		{
			if (ULyraCloneWeaponInstance* NewInstance = EquipmentManager->GetFirstInstanceOfType<ULyraCloneWeaponInstance>())
			{
				if (NewInstance != CurrentInstance && NewInstance->GetInstigator() != nullptr)
				{
					// 새로 업데이트해주고, OnWeaponChanged 호출 진행
					ULyraCloneWeaponInstance* OldWeapon = CurrentInstance;
					CurrentInstance = NewInstance;
					OnWeaponChanged(OldWeapon, CurrentInstance);
				}
			}
		}
	}
}
