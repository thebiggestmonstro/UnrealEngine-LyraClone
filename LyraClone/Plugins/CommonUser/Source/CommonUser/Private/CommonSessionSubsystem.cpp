// Fill out your copyright notice in the Description page of Project Settings.


#include "CommonSessionSubsystem.h"

#include "CommonSessionSubsystem.h"
#include "Engine/AssetManager.h"
#include "Engine/LocalPlayer.h"
#include UE_INLINE_GENERATED_CPP_BY_NAME(CommonSessionSubsystem)

FString UCommonSession_HostSessionRequest::GetMapName() const
{
	// AssetManager를 통해, MapID를 통해, FAssetData를 반환하고, 이를 통해 PackageName를 통해 Map 경로를 반환한다
	FAssetData MapAssetData;
	if (UAssetManager::Get().GetPrimaryAssetData(MapID, MapAssetData))
	{
		return MapAssetData.PackageName.ToString();
	}
	return FString();
}

FString UCommonSession_HostSessionRequest::ConstructTravelURL() const
{
	FString CombinedExtraArgs;

	for (const auto& ExtraArg : ExtraArgs)
	{
		if (ExtraArg.Key.IsEmpty())
		{
			continue;
		}

		/**
		 * ?를 separate로 복수개의 ExtraArg를 추가함:
		 * - Key 값 유무에 따라, =(assignment)를 통해 알맞는 CmdArg를 생성
		 */
		if (ExtraArg.Value.IsEmpty())
		{
			CombinedExtraArgs += FString::Printf(TEXT("?%s"), *ExtraArg.Key);
		}
		else
		{
			CombinedExtraArgs += FString::Printf(TEXT("?%s=%s"), *ExtraArg.Key, *ExtraArg.Value);
		}
	}

	// Map 경로 앞에 추가하여, 최종 TravelURL 생성
	return FString::Printf(TEXT("%s%s"), *GetMapName(), *CombinedExtraArgs);
}

void UCommonSessionSubsystem::HostSession(APlayerController* HostingPlayer, UCommonSession_HostSessionRequest* Request)
{
	ULocalPlayer* LocalPlayer = (HostingPlayer != nullptr) ? HostingPlayer->GetLocalPlayer() : nullptr;
	if (!LocalPlayer)
	{
		return;
	}

	// HostSessionRequest에서 MapID와 ExtraArgs를 통해 URL을 생성하여, MapLoad를 시작한다
	GetWorld()->ServerTravel(Request->ConstructTravelURL());
}