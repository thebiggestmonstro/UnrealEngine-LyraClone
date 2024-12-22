// Copyright Epic Games, Inc. All Rights Reserved.

#include "LyraClone.h"
#include "Modules/ModuleManager.h"
#include "LyraCloneLogChannels.h"

/**
 * FLyraCloneModule
 */
class FLyraCloneModule : public FDefaultGameModuleImpl
{
public:
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;
};

void FLyraCloneModule::StartupModule()
{
	FDefaultGameModuleImpl::StartupModule();
	UE_LOG(LogLyraClone, Warning, TEXT("Custom Module StartUp - LyraClone"));
}

void FLyraCloneModule::ShutdownModule()
{
	FDefaultGameModuleImpl::ShutdownModule();
}

IMPLEMENT_PRIMARY_GAME_MODULE(FLyraCloneModule, LyraClone, "LyraClone" );