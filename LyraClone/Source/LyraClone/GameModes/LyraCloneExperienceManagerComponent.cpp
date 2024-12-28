// Fill out your copyright notice in the Description page of Project Settings.


#include "GameModes/LyraCloneExperienceManagerComponent.h"
#include "System/LyraCloneAssetManager.h"
#include "GameFeaturesSubsystemSettings.h"
#include "GameModes/LyraCloneExperienceDefinition.h"

void ULyraCloneExperienceManagerComponent::CallOrRegister_OnExperienceLoaded(FOnLyraCloneExperienceLoaded::FDelegate&& Delegate)
{
	// IsExperienceLoaded() 구현
	// 현재 프레임에 이미 Exeperience가 로드되어 있다면, 바로 델리게이트를 호출
	if (IsExperienceLoaded())
	{
		Delegate.Execute(CurrentExperience);
	}
	// 현재 프레임에 로드되어 있지 않다면,
	else
	{
		/**
		 * 참고로, 여러분들은 Delegate 객체를 자세히 살펴보면, 내부적으로 필요한 변수들은 메모리 할당해놓는다:
		 * TArray<int> a = {1, 2, 3, 4};
		 * delegate_type delegate = [a](){
		 *	return a.Num();
		 * }
		 * a는 delegate_type 내부에 new로 할당되어 있다. 복사 비용을 낮추기 위해 Move를 통해 하는 것을 잊지 말자!
		 */
		OnExperienceLoaded.Add(MoveTemp(Delegate));
	}
}

void ULyraCloneExperienceManagerComponent::ServerSetCurrentExperience(FPrimaryAssetId ExperienceId)
{
	ULyraCloneAssetManager& AssetManager = ULyraCloneAssetManager::Get();

	// ExperienceDefinition의 Subclass를 가져온다
	TSubclassOf<ULyraCloneExperienceDefinition> AssetClass;
	{
		FSoftObjectPath AssetPath = AssetManager.GetPrimaryAssetPath(ExperienceId);
		AssetClass = Cast<UClass>(AssetPath.TryLoad());
	}

	// 왜 CDO를 가져오는 걸까?
	const ULyraCloneExperienceDefinition* Experience = GetDefault<ULyraCloneExperienceDefinition>(AssetClass);
	check(Experience != nullptr);
	check(CurrentExperience == nullptr);
	{
		// 그리고 CDO로 CurrentExperience를 설정한다!
		// 어떤 의도로 이렇게 코드를 작성한지는 코드를 쭉 읽어보고(StartExperienceLoad까지 읽어보자) 다시 생각해보자:
		CurrentExperience = Experience;
	}

	StartExperienceLoad();
}

void ULyraCloneExperienceManagerComponent::StartExperienceLoad()
{
	check(CurrentExperience);
	check(LoadState == ELyraCloneExperienceLoadState::Unloaded);

	LoadState = ELyraCloneExperienceLoadState::Loading;

	ULyraCloneAssetManager& AssetManager = ULyraCloneAssetManager::Get();

	TSet<FPrimaryAssetId> BundleAssetList;

	// 이미 앞서, ServerSetCurrentExperience에서 우리는 ExperienceId를 넘겨주었는데, 여기서 CDO를 활용하여, GetPrimaryAssetId를 로딩할 대상으로 넣는다!
	// - 왜 이렇게 하는걸까?
	// - GetPrimaryAssetId를 좀 더 자세히보자:
	// - GetPrimaryAssetId를 살펴봄으로써, 아래의 두가지를 알 수 있다:
	//   1. 우리는 B_LyraCloneDefaultExperience를 BP로 만든 이유
	//   2. CDO를 가져와서, GetPrimaryAssetId를 호출한 이유

	// 우리는 앞서 이미 CDO로 로딩하여, CDO를 사용하지 않고 CDO를 사용하여 로딩할 에셋을 지정하여, BundleAssetList에 추가해준다!
	BundleAssetList.Add(CurrentExperience->GetPrimaryAssetId());

	// load assets associated with the experience
	// 아래는 우리가 후일 GameFeature를 사용하여, Experience에 바인딩된 GameFeature Plugin을 로딩할 Bundle 이름을 추가한다:
	// - Bundle이라는게 후일 우리가 로딩할 에셋의 카테고리 이름이라고 생각하면 된다 (일단 지금은 넘어가자 후일, 또 다룰 것이다!)
	// 서버와 클라이언트가 필요한 모든 Bundle 애셋을 전부 불러옴
	TArray<FName> BundlesToLoad;
	{
		// 여기서 주목해야 할 부분은 OwnerNetMode가 NM_Standalone이면? Client/Server 둘다 로딩에 추가된다!
		const ENetMode OwnerNetMode = GetOwner()->GetNetMode();
		bool bLoadClient = GIsEditor || (OwnerNetMode != NM_DedicatedServer);
		bool bLoadServer = GIsEditor || (OwnerNetMode != NM_Client);

		// 오늘! 아래의 의미를 알게 될 것이다:
		// - LoadStateClient = "Client", LoadStateServer = "Server"
		if (bLoadClient)
		{
			BundlesToLoad.Add(UGameFeaturesSubsystemSettings::LoadStateClient);
		}
		if (bLoadServer)
		{
			BundlesToLoad.Add(UGameFeaturesSubsystemSettings::LoadStateServer);
		}
	}

	FStreamableDelegate OnAssetsLoadedDelegate = FStreamableDelegate::CreateUObject(this, &ThisClass::OnExperienceLoadComplete);

	// 아래도, 후일 Bundle을 우리가 GameFeature에 연동하면서 더 깊게 알아보기로 하고, 지금은 앞서 B_LyraCloneDefaultExperience를 로딩해주는 함수로 생각하자
	// 아래의 ChangeBundleStateForPrimaryAssets 함수는 BundleAssetList를 순회라면서 BundlsToLoad와 동일한 애셋을 비동기로 로딩한다
	// 이는 모든 애셋을 전부 불러오지 않고, 필요한 애셋만 불러오는 이점이 존재한다
	TSharedPtr<FStreamableHandle> Handle = AssetManager.ChangeBundleStateForPrimaryAssets(
		BundleAssetList.Array(),
		BundlesToLoad,
		{}, false, FStreamableDelegate(), FStreamableManager::AsyncLoadHighPriority);

	// 핸들이 존재하지 않거나, 핸들이 로드할 애셋의 로딩이 완료되었다면 OnAssetsLoadedDelegate를 호출한다
	if (!Handle.IsValid() || Handle->HasLoadCompleted())
	{
		// 로딩이 완료되었으면, ExecuteDelegate를 통해 OnAssetsLoadedDelegate를 호출하자:
		// - 아래의 함수를 확인해보자:
		FStreamableHandle::ExecuteDelegate(OnAssetsLoadedDelegate);
	}
	// 위의 경우에 해당하지 않는다면 일단 OnAssetsLoadedDelegate에 묶어만 둔다
	else
	{
		Handle->BindCompleteDelegate(OnAssetsLoadedDelegate);
		Handle->BindCancelDelegate(FStreamableDelegate::CreateLambda([OnAssetsLoadedDelegate]()
			{
				OnAssetsLoadedDelegate.ExecuteIfBound();
			}));
	}

	// FrameNumber를 주목해서 보자
	static int32 StartExperienceLoad_FrameNumber = GFrameNumber;
}

void ULyraCloneExperienceManagerComponent::OnExperienceLoadComplete()
{
	// FrameNumber를 주목해서 보자
	static int32 OnExperienceLoadComplete_FrameNumber = GFrameNumber;

	OnExperienceFullLoadCompleted();
}

void ULyraCloneExperienceManagerComponent::OnExperienceFullLoadCompleted()
{
	check(LoadState != ELyraCloneExperienceLoadState::Loaded);

	LoadState = ELyraCloneExperienceLoadState::Loaded;
	OnExperienceLoaded.Broadcast(CurrentExperience);
	OnExperienceLoaded.Clear();
}