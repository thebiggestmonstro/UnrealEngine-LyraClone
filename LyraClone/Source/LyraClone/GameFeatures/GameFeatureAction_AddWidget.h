// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFeatures/GameFeatureAction_WorldActionBase.h"
#include "CommonActivatableWidget.h"
#include "GameplayTagContainer.h"
#include "UIEXtensionSystem.h"
#include "GameFeatureAction_AddWidget.generated.h"

/** forward declarations */
struct FComponentRequestHandle;

/** HUD의 Layout 요청 */
USTRUCT()
struct FLyraCloneHUDLayoutRequest
{
	GENERATED_BODY()

	/** UI의 레이아웃으로 CommonActivatableWidget을 사용 */
	UPROPERTY(EditAnywhere, Category = UI, meta = (AssetBundles = "Client"))
	TSoftClassPtr<UCommonActivatableWidget> LayoutClass;

	/** 앞서 보았던 PrimaryGameLayout의 LayerID를 의미 */
	UPROPERTY(EditAnywhere, Category = UI)
	FGameplayTag LayerID;
};

USTRUCT()
struct FLyraCloneHUDElementEntry
{
	GENERATED_BODY()

	/** LyraCloneHUDLayout 위에 올릴 대상이 되는 Widget Class */
	UPROPERTY(EditAnywhere, Category = UI, meta = (AssetBundles = "Client"))
	TSoftClassPtr<UUserWidget> WidgetClass;

	/** SlotID는 LyraCloneHUDLayoutRequest에 올린 LayoutClass에 정의된 Slot(GameplayTag)를 의미 */
	UPROPERTY(EditAnywhere, Category = UI)
	FGameplayTag SlotID;
};

/**
 * 
 */
UCLASS()
class LYRACLONE_API UGameFeatureAction_AddWidget : public UGameFeatureAction_WorldActionBase
{
	GENERATED_BODY()
	
public:
	struct FPerContextData
	{
		TArray<TSharedPtr<FComponentRequestHandle>> ComponentRequests;
		TArray<TWeakObjectPtr<UCommonActivatableWidget>> LayoutsAdded;

		/** Lyra에서 HUDElement는 UIExtension으로 관리된다. */
		TArray<FUIExtensionHandle> ExtensionHandles;
	};

	void AddWidgets(AActor* Actor, FPerContextData& ActiveData);
	void RemoveWidgets(AActor* Actor, FPerContextData& ActiveData);

	/**
	 * UGameFeatureAction's interface
	 */
	virtual void OnGameFeatureDeactivating(FGameFeatureDeactivatingContext& Context) override;

	/**
	 * UGameFeatureAction_WorldActionBase's interface
	 */
	virtual void AddToWorld(const FWorldContext& WorldContext, const FGameFeatureStateChangeContext& ChangeContext) override;

	void HandleActorExtension(AActor* Actor, FName EventName, FGameFeatureStateChangeContext ChangeContext);

	/** GFA Add/Remove 상태 관리 */
	TMap<FGameFeatureStateChangeContext, FPerContextData> ContextData;

	/**
	 * GFA_AddWidget은 형태를 정의하는 Layout과 Layout 위에 올릴 Widget 객체로 Widgets으로 구성된다
	 */
	UPROPERTY(EditAnywhere, Category = UI)
	TArray<FLyraCloneHUDLayoutRequest> Layout;

	UPROPERTY(EditAnywhere, Category = UI)
	TArray<FLyraCloneHUDElementEntry> Widgets;
};
