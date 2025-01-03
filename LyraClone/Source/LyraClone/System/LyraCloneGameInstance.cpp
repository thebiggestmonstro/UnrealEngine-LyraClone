#include "System/LyraCloneGameInstance.h"
#include "LyraCloneGameplayTags.h"
#include "Components/GameFrameworkComponentManager.h"
#include UE_INLINE_GENERATED_CPP_BY_NAME(LyraCloneGameInstance)

ULyraCloneGameInstance::ULyraCloneGameInstance(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
}

void ULyraCloneGameInstance::Init()
{
	Super::Init();

	// 앞서 정의한 InitState의 GameplayTags 등록:
	UGameFrameworkComponentManager* ComponentManager = GetSubsystem<UGameFrameworkComponentManager>(this);
	if (ensure(ComponentManager))
	{
		const FLyraCloneGameplayTags& GameplayTags = FLyraCloneGameplayTags::Get();

		ComponentManager->RegisterInitState(GameplayTags.InitState_Spawned, false, FGameplayTag());
		ComponentManager->RegisterInitState(GameplayTags.InitState_DataAvailable, false, GameplayTags.InitState_Spawned);
		ComponentManager->RegisterInitState(GameplayTags.InitState_DataInitialized, false, GameplayTags.InitState_DataAvailable);
		ComponentManager->RegisterInitState(GameplayTags.InitState_GameplayReady, false, GameplayTags.InitState_DataInitialized);
	}
}

void ULyraCloneGameInstance::Shutdown()
{
	Super::Shutdown();
}