// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/LyraCloneHealthComponent.h"
#include "LyraClone/LyraCloneLogChannels.h"
#include "GameplayEffect.h"
#include "GameplayEffectExtension.h"
#include "AbilitySystem/LyraCloneAbilitySystemComponent.h"
#include "AbilitySystem/Attributes/LyraCloneHealthSet.h"

ULyraCloneHealthComponent::ULyraCloneHealthComponent(const FObjectInitializer& ObjectInitializer) 
	: Super(ObjectInitializer)
{
	// HealthComponent는 PlayerState의 HealthSet과 Character(Pawn)간 Bridge 역활이라고 생각하면 된다:
	// - 따로 로직이 업데이트 될 필요가 없는 이벤트 기반으로 동작하는 컴포넌트로 이해하면 된다
	PrimaryComponentTick.bStartWithTickEnabled = false;
	PrimaryComponentTick.bCanEverTick = false;

	// InitializeWithAbilitySystem으로 ASC가 초기화되기 전까지 HealthSet과 ASC는 null이다:
	AbilitySystemComponent = nullptr;
	HealthSet = nullptr;
}

void ULyraCloneHealthComponent::InitializeWithAbilitySystem(ULyraCloneAbilitySystemComponent* InASC)
{
	// AActor는 LyraCloneCharacter를 상속받고 있는 클래스일 것이다
	AActor* Owner = GetOwner();
	check(Owner);

	if (AbilitySystemComponent)
	{
		UE_LOG(LogLyraClone, Error, TEXT("LyraCloneHealthComponent: Health component for owner [%s] has already been initialized with an ability system."), *GetNameSafe(Owner));
		return;
	}

	// ASC 캐싱
	AbilitySystemComponent = InASC;
	if (!AbilitySystemComponent)
	{
		UE_LOG(LogLyraClone, Error, TEXT("LyraCloneHealthComponent: Cannot initialize health component for owner [%s] with NULL ability system."), *GetNameSafe(Owner));
		return;
	}

	// AbilitySystemComponent::GetSet은 SpawnedAttributes에서 가져온다:
	// - 근데 PlayerState에서 Subobject로 생성하고 따로 ASC에 등록한적이 없는데 어떻게 등록되어있을까?
	//   - AbilitySystemComponent::InitializeComponent()에서 GetObjectsWithOuter로 SpawnedAttributes에 추가된다:
	//   - 잘 생각해보자 HealthSet은 PlayerState에 Subobject로 있고, ASC 또한 PlayerState에 있다:
	//     -> 이는 ASC에서 GetObjectsWithOuter로 HealthSet이 접근된다!!!
	// - 한번 AbilitySystemComponent::InitializeComponent()을 보자
	HealthSet = AbilitySystemComponent->GetSet<ULyraCloneHealthSet>();
	if (!HealthSet)
	{
		UE_LOG(LogLyraClone, Error, TEXT("LyraCloneHealthComponent: Cannot initialize health component for owner [%s] with NULL health set on the ability system."), *GetNameSafe(Owner));
		return;
	}

	// HealthSet의 HealthAttribute의 업데이트가 일어날때마다 호출할 콜백으로 멤버메서드 HandleHealthChanged를 등록하자:
	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(ULyraCloneHealthSet::GetHealthAttribute()).AddUObject(this, &ThisClass::HandleHealthChanged);

	// 초기화 한번 해줬으니깐 Broadcast 해주자
	OnHealthChanged.Broadcast(this, 0, HealthSet->GetHealth(), nullptr);
}

void ULyraCloneHealthComponent::UninitializeWithAbilitySystem()
{
	AbilitySystemComponent = nullptr;
	HealthSet = nullptr;
}

static AActor* GetInstigatorFromAttrChangeData(const FOnAttributeChangeData& ChangeData)
{
	// GameEffectModifier에 Data가 있을 경우만 호출되는가보다 (사실 우리는 크게 관심없음)
	if (ChangeData.GEModData != nullptr)
	{
		const FGameplayEffectContextHandle& EffectContext = ChangeData.GEModData->EffectSpec.GetEffectContext();
		return EffectContext.GetOriginalInstigator();
	}
	return nullptr;
}

void ULyraCloneHealthComponent::HandleHealthChanged(const FOnAttributeChangeData& ChangeData)
{
	OnHealthChanged.Broadcast(this, ChangeData.OldValue, ChangeData.NewValue, GetInstigatorFromAttrChangeData(ChangeData));
}

ULyraCloneHealthComponent* ULyraCloneHealthComponent::FindHealthComponent(const AActor* Actor)
{
	if (!Actor)
	{
		return nullptr;
	}

	ULyraCloneHealthComponent* HealthComponent = Actor->FindComponentByClass<ULyraCloneHealthComponent>();
	return HealthComponent;
}

float ULyraCloneHealthComponent::GetHealth() const
{
	return (HealthSet ? HealthSet->GetHealth() : 0.0f);
}

float ULyraCloneHealthComponent::GetMaxHealth() const
{
	return (HealthSet ? HealthSet->GetMaxHealth() : 0.0f);
}

float ULyraCloneHealthComponent::GetHealthNormalized() const
{
	if (HealthSet)
	{
		const float Health = HealthSet->GetHealth();
		const float MaxHealth = HealthSet->GetMaxHealth();
		return ((MaxHealth > 0.0f) ? (Health / MaxHealth) : 0.0f);
	}
	return 0.0f;
}