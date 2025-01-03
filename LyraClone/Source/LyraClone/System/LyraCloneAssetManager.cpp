// Fill out your copyright notice in the Description page of Project Settings.


#include "LyraCloneAssetManager.h"
#include "LyraClone/LyraCloneLogChannels.h"
#include "LyraCloneGameplayTags.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(LyraCloneAssetManager)

ULyraCloneAssetManager::ULyraCloneAssetManager()
	: UAssetManager()
{

}

UE_DISABLE_OPTIMIZATION_SHIP
void ULyraCloneAssetManager::StartInitialLoading()
{
	// 오버라이드할 경우, Super의 호출은 꼭 까먹지 말자
	Super::StartInitialLoading();

	FLyraCloneGameplayTags::InitializeNativeTags();
}
UE_ENABLE_OPTIMIZATION_SHIP

ULyraCloneAssetManager& ULyraCloneAssetManager::Get()
{
	check(GEngine);

	// 우리는 AssetManager를 UEngine의 GEngine의 AssetManager의 Class를 오버라이드했기 때문에, 이미 GEngine에 Asset Manager가 있음
	if (ULyraCloneAssetManager* Singleton = Cast<ULyraCloneAssetManager>(GEngine->AssetManager))
	{
		return *Singleton;
	}

	// Fatal 로그의 경우, 해당 부분에서 크래시를 발생시킴
	UE_LOG(LogLyraClone, Fatal, TEXT("invalid AssetManagerClassName in DefaultEngine.ini(project settings); it must be LyraCloneAssetManager"));

	// 위의 UE_LOG의 Fatal의 Verbose가 물론 Crash를 통해, 여기까지 도달하지는 않지만, Code Completness를 위해 더미로 리턴 진행
	return *NewObject<ULyraCloneAssetManager>();
}

bool ULyraCloneAssetManager::ShouldLogAssetLoads()
{
	// CommandLineContent는 실제 cmd 명령어와 이에 사용하는 인자들을 의미함
	const TCHAR* CommandLineContent = FCommandLine::Get();
	static bool bLogAssetLoads = FParse::Param(CommandLineContent, TEXT("LogAssetLoads"));
	return bLogAssetLoads;
}

UObject* ULyraCloneAssetManager::SynchronousLoadAsset(const FSoftObjectPath& AssetPath)
{
	// 해당 함수를 만든 이유는 'synchronous load asset을 불필요하게 하는 것이 있는지 체크하기 위함!'
	if (AssetPath.IsValid())
	{
		// 잠깐 FScopeLogTime을 확인해보자:
		TUniquePtr<FScopeLogTime> LogTimePtr;
		if (ShouldLogAssetLoads())
		{
			// 단순히 로깅하면서, 초단위로 로깅 진행 - ShouldLogAssetLoads을 통해 cmd 명령어의 인자를 확인하고 존재한다면
			// 확인된 인자의 경로를 만들고 경로를 사용하여 동기적으로 해당 인자를 로딩하는 시간을 측정함
			LogTimePtr = MakeUnique<FScopeLogTime>(*FString::Printf(TEXT("synchronously loaded assets [%s]"), *AssetPath.ToString()), nullptr, FScopeLogTime::ScopeLog_Seconds);
		}

		// 여기서 두가지 분기:
		// 1. AssetManager가 있으면, AssetManager의 StreamableManager를 통해 정적 로딩
		// 2. 아니면, FSoftObjectPath를 통해 바로 정적 로딩
		if (UAssetManager::IsValid())
		{
			return UAssetManager::GetStreamableManager().LoadSynchronous(AssetPath);
		}

		// if asset manager is not ready, use LoadObject()
		// - 슥 보면, StaticLoadObject가 보인다: 
		// - 참고로, 항상 StaticLoadObject하기 전에 StaticFindObject를 통해 확인하고 실패하면 진짜 로딩함
		// 이때 TryLoad의 경우 처리 속도가 굉장히 느림
		return AssetPath.TryLoad();
	}

	return nullptr;
}

void ULyraCloneAssetManager::AddLoadedAsset(const UObject* Asset)
{
	if (ensureAlways(Asset))
	{
		// for thread-safety, use lock
		FScopeLock Lock(&SyncObject);
		LoadedAssets.Add(Asset);
	}
}