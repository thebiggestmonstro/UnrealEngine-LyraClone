// Fill out your copyright notice in the Description page of Project Settings.


#include "Equipment/LyraCloneEquipmentDefinition.h"
#include "LyracloneEquipmentInstance.h"

ULyraCloneEquipmentDefinition::ULyraCloneEquipmentDefinition(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	// 기본값으로, LyraCloneEquipmentInstance로 설정
	InstanceType = ULyraCloneEquipmentInstance::StaticClass();
}
