// Fill out your copyright notice in the Description page of Project Settings.


#include "Equipment/LyraCloneQuickBarComponent.h"

ULyraCloneQuickBarComponent::ULyraCloneQuickBarComponent(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{

}

void ULyraCloneQuickBarComponent::BeginPlay()
{
	// NumSlots에 따라 미리 Slots을 할당한다
	if (Slots.Num() < NumSlots)
	{
		Slots.AddDefaulted(NumSlots - Slots.Num());
	}

	// 반드시 BeginPlay() 불러주는 것을 까먹지 말자!
	Super::BeginPlay();
}

void ULyraCloneQuickBarComponent::AddItemToSlot(int32 SlotIndex, ULyraCloneInventoryItemInstance* Item)
{
	// 해당 로직을 보면, Slosts는 Add로 동적 추가가 아닌, Index에 바로 넣는다:
	// - 그럼 미리 Pre-size 했다는 것인데 이는 BeginPlay()에서 진행한다
	if (Slots.IsValidIndex(SlotIndex) && (Item != nullptr))
	{
		if (Slots[SlotIndex] == nullptr)
		{
			Slots[SlotIndex] = Item;
		}
	}
}