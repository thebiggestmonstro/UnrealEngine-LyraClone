// Fill out your copyright notice in the Description page of Project Settings.


#include "GameUIManagerSubsystem.h"
#include "GameUIPolicy.h"

UGameUIManagerSubsystem::UGameUIManagerSubsystem()
{
}

void UGameUIManagerSubsystem::SwitchToPolicy(UGameUIPolicy* InPolicy)
{
	if (CurrentPolicy != InPolicy)
	{
		CurrentPolicy = InPolicy;
	}
}

void UGameUIManagerSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);

	// CurrentPolicy가 설정되어 있지 않고, DefaultUIPolicyClass가 제대로 설정되어 있을 경우 (우리는 HakUIPolicy겠지?)
	if (!CurrentPolicy && !DefaultUIPolicyClass.IsNull())
	{
		// UIPolicyClass는 BP에셋이기에, 로딩해야 함
		TSubclassOf<UGameUIPolicy> PolicyClass = DefaultUIPolicyClass.LoadSynchronous();

		// UIPolicyClass를 통해 NewObject로 인스턴싱해서 CurrentPolicy에 설정
		SwitchToPolicy(NewObject<UGameUIPolicy>(this, PolicyClass));
	}
}

void UGameUIManagerSubsystem::Deinitialize()
{
	Super::Deinitialize();
	SwitchToPolicy(nullptr);
}

bool UGameUIManagerSubsystem::ShouldCreateSubsystem(UObject* Outer) const
{
	// 우선 DedicatedServer의 경우, GameUIManagerSubsystem을 활성화하지 않음
	if (!CastChecked<UGameInstance>(Outer)->IsDedicatedServerInstance())
	{
		// 만약 어떠한 Subsystem도 UGameUIManagerSubsystem을 상속받지 않았다면, 해당 GameInstanceSubsystem은 활성화 안함
		TArray<UClass*> ChildClasses;
		GetDerivedClasses(GetClass(), ChildClasses, false);

		return ChildClasses.Num() == 0;
	}

	return false;
}

void UGameUIManagerSubsystem::NotifyPlayerAdded(UCommonLocalPlayer* LocalPlayer)
{
	if (ensure(LocalPlayer) && CurrentPolicy)
	{
		CurrentPolicy->NotifyPlayerAdded(LocalPlayer);
	}
}

void UGameUIManagerSubsystem::NotifyPlayerRemoved(UCommonLocalPlayer* LocalPlayer)
{
	if (LocalPlayer && CurrentPolicy)
	{
		CurrentPolicy->NotifyPlayerRemoved(LocalPlayer);
	}
}

void UGameUIManagerSubsystem::NotifyPlayerDestroyed(UCommonLocalPlayer* LocalPlayer)
{
	if (LocalPlayer && CurrentPolicy)
	{
		CurrentPolicy->NotifyPlayerDestroyed(LocalPlayer);
	}
}