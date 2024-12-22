// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/AssetManager.h"
#include "LyraCloneAssetManager.generated.h"

/**
 * 
 */
UCLASS()
class LYRACLONE_API ULyraCloneAssetManager : public UAssetManager
{
	GENERATED_BODY()
	
public:
	ULyraCloneAssetManager();

	/**
	 * UAssetManager interfaces
	 */
	virtual void StartInitialLoading() override;

	static ULyraCloneAssetManager& Get();

	/** whether it leaves log for asset loading */
	// 해당 함수는 FScopeLogTime을 사용할지 유무를 판단하기 위해 사용한다:
	// - 굳이 클론 코딩할 필요 없지만, FScopeLogTime은 있다는 것을 알아두면 좋으니깐 진행해보자
	static bool ShouldLogAssetLoads();

	/** static loading for AssetType */
	// 동기적으로 애셋을 로딩하는 방식
	// 인게임에서는 비동기 방식이 유리하지만, 
	// 게임이 시작되는 초창기에는 동기 방식이 유리함
	static UObject* SynchronousLoadAsset(const FSoftObjectPath& AssetPath);

	/*
	*  GetAsset은 정적 로딩으로 BP Class와 Object 로딩
	*  주목해야 할 부분은 bKeepInMemory
	*/
	template <typename AssetType>
	static AssetType* GetAsset(const TSoftObjectPtr<AssetType>& AssetPointer, bool bKeepInMemory = true);

	template <typename AssetType>
	static TSubclassOf<AssetType> GetSubclass(const TSoftClassPtr<AssetType>& AssetPointer, bool bKeepInMemory = true);

	/** thread-safe way of adding asset to keep in memory [Thread-Safe] */
	void AddLoadedAsset(const UObject* Asset);

	/**
	 * member variables
	 */
	 // Garbage Colllection의 대상
	UPROPERTY()
	TSet<TObjectPtr<const UObject>> LoadedAssets;

	// object 단위 locking
	FCriticalSection SyncObject;
};

template <typename AssetType>
AssetType* ULyraCloneAssetManager::GetAsset(const TSoftObjectPtr<AssetType>& AssetPointer, bool bKeepInMemory)
{
	AssetType* LoadedAsset = nullptr;
	const FSoftObjectPath& AssetPath = AssetPointer.ToSoftObjectPath();
	if (AssetPath.IsValid())
	{
		LoadedAsset = AssetPointer.Get();
		// 해당 요소가 로드되어 있지 않다면
		if (!LoadedAsset)
		{
			LoadedAsset = Cast<AssetType>(SynchronousLoadAsset(AssetPath));
			ensureAlwaysMsgf(LoadedAsset, TEXT("Failed to load asset [%s]"), *AssetPointer.ToString());
		}
		// 해당 요소가 로드되어 있다면
		if (LoadedAsset && bKeepInMemory)
		{
			// added to loaded asset list.
			// 여기서 AddLoadAsset은 메모리에 상주하기 위한 장치라고 생각하면 됨:
			// - 한번 등록되면 직접 내리지 않는한 Unload가 되지 않음 (== 캐싱)
			Get().AddLoadedAsset(Cast<UObject>(LoadedAsset));
		}
	}
	return LoadedAsset;
}

template <typename AssetType>
TSubclassOf<AssetType> ULyraCloneAssetManager::GetSubclass(const TSoftClassPtr<AssetType>& AssetPointer, bool bKeepInMemory)
{
	TSubclassOf<AssetType> LoadedSubclass;
	const FSoftObjectPath& AssetPath = AssetPointer.ToSoftObjectPath();
	if (AssetPath.IsValid())
	{
		LoadedSubclass = AssetPointer.Get();
		// 해당 Subclass 요소가 로드되어 있지 않다면
		if (!LoadedSubclass)
		{
			LoadedSubclass = Cast<UClass>(SynchronousLoadAsset(AssetPath));
			ensureAlwaysMsgf(LoadedSubclass, TEXT("Failed to load asset class [%s]"), *AssetPointer.ToString());
		}
		// 해당 Subclass 요소가 로드되어 있다면
		if (LoadedSubclass && bKeepInMemory)
		{
			// Added to loaded asset list.
			Get().AddLoadedAsset(Cast<UObject>(LoadedSubclass));
		}
	}
	return LoadedSubclass;
}