// Fill out your copyright notice in the Description page of Project Settings.


#include "GameModes/LyraCloneExperienceActionSet.h"
#include "GameFeatureAction.h"

ULyraCloneExperienceActionSet::ULyraCloneExperienceActionSet()
{

}

#if WITH_EDITORONLY_DATA
void ULyraCloneExperienceActionSet::UpdateAssetBundleData()
{
	Super::UpdateAssetBundleData();

	for (UGameFeatureAction* Action : Actions)
	{
		if (Action)
		{
			Action->AddAdditionalAssetBundleData(AssetBundleData);
		}
	}
}
#endif