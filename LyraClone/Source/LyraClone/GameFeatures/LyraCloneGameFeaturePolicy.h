#pragma once

#include "Containers/Array.h"
#include "GameFeatureStateChangeObserver.h"
#include "GameFeaturesProjectPolicies.h"
#include "UObject/Object.h"
#include "UObject/ObjectPtr.h"
#include "UObject/UObjectGlobals.h"
#include "LyraCloneGameFeaturePolicy.generated.h"

/**
 * ULyraCloneGameplayFeaturePolicy는 GameFeature Plugin이 Memory에 로딩되거나 Active(활성화)를 관리하는 StateMachine을 모니터링 가능하다
 */
UCLASS()
class ULyraCloneGameplayFeaturePolicy : public UDefaultGameFeaturesProjectPolicies
{
	GENERATED_BODY()
	
public:
	ULyraCloneGameplayFeaturePolicy(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	/**
	* GameFeaturesProjectPolicies interfaces
	*/
	virtual void InitGameFeatureManager() override;
	virtual void ShutdownGameFeatureManager() override;

	/** Observer로서 등록한 객체를 관리 (아래 LyraCloneGameFeature_AddGameplayCuePaths가 하나의 예시) */
	UPROPERTY(Transient)
	TArray<TObjectPtr<UObject>> Observers;
};

/**
 * GameFeature Plugin이 Register/Unregister 단계에서 GameplayCuePath가 등록되어 GameplayCueManager가 관련 GameplayCue 에셋을 스캔할 수 있도록 해야 한다:
 * 이를 위해 GameFeatureStateChangeObserver를 활용하여, 옵저버 패턴으로 이를 가능하게 하도록 한다:
 * - 아래에 우리가 Override할 메서드에서 유추할 수 있다시피 GameFeatureStateChangeObserver를 활용하면 가능하다!
 * - 이를 위해 추가적 구현이 필요한데 이는 ULyraCloneGameplayFeaturePolicy를 참고하자.
 *
 * 참고로, GameFeatureAction에도 Registering/Unregistering이 있긴하다:
 * - 허나, 우리가 앞서 구현했다시피, 직접 Registering/Unregistering 호출해줘야 한다
 *   - Registering/Unregistering 구현해봐야 호출하지 않으면 아무 의미 없다...
 * - Lyra에서는 GameplayFeaturePolicy를 오버라이딩하는 방식으로 이를 진행하였다
 */
UCLASS()
class ULyraCloneGameFeature_AddGameplayCuePaths : public UObject, public IGameFeatureStateChangeObserver
{
	GENERATED_BODY()
	
public:
	/**
	 * IGameFeatureStateChangeObserver interface
	 */
	virtual void OnGameFeatureRegistering(const UGameFeatureData* GameFeatureData, const FString& PluginName, const FString& PluginURL) override;
	virtual void OnGameFeatureUnregistering(const UGameFeatureData* GameFeatureData, const FString& PluginName, const FString& PluginURL) override;
};