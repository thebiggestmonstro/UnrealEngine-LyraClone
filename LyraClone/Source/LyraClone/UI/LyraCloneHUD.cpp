// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/LyraCloneHUD.h"
#include "Components/GameFrameworkComponentManager.h"
#include UE_INLINE_GENERATED_CPP_BY_NAME(LyraCloneHUD)

ALyraCloneHUD::ALyraCloneHUD(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{}

void ALyraCloneHUD::PreInitializeComponents()
{
	Super::PreInitializeComponents();

	// LyraCloneHUD를 Receiver로 Actor를 추가하자
	UGameFrameworkComponentManager::AddGameFrameworkComponentReceiver(this);
}

void ALyraCloneHUD::BeginPlay()
{
	UGameFrameworkComponentManager::SendGameFrameworkComponentExtensionEvent(this, UGameFrameworkComponentManager::NAME_GameActorReady);
	Super::BeginPlay();
}

void ALyraCloneHUD::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	UGameFrameworkComponentManager::RemoveGameFrameworkComponentReceiver(this);
	Super::EndPlay(EndPlayReason);
}
