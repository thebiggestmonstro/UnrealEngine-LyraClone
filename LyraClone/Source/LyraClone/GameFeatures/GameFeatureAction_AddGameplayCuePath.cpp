#include "GameFeatureAction_AddGameplayCuePath.h"
#include UE_INLINE_GENERATED_CPP_BY_NAME(GameFeatureAction_AddGameplayCuePath)

UGameFeatureAction_AddGameplayCuePath::UGameFeatureAction_AddGameplayCuePath() 
	: Super()
{
	DirectoryPathsToAdd.Add(FDirectoryPath{ TEXT("/GameplayCues") });
}