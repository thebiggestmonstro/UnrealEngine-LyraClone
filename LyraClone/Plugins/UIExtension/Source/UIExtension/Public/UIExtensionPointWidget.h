// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "UIExtensionSystem.h"
#include "Components/DynamicEntryBoxBase.h"
#include "UIExtensionPointWidget.generated.h"

/**
 * 
 */
UCLASS()
class UIEXTENSION_API UUIExtensionPointWidget : public UDynamicEntryBoxBase
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "UI Extension")
	FGameplayTag ExtensionPointTag;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "UI Extension")
	EUIExtensionPointMatch ExtensionPointTagMatch = EUIExtensionPointMatch::ExactMatch;

	/** UIExtensionPointWidget에 결합이 허용된 UWidget Classes */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "UI Extension")
	TArray<TObjectPtr<UClass>> DataClasses;

	/** UIExtension --- Widget 관계 매핑 */
	UPROPERTY(Transient)
	TMap<FUIExtensionHandle, TObjectPtr<UUserWidget>> ExtensionMapping;

	/** UIExtensionPoint 객체 관리 */
	TArray<FUIExtensionPointHandle> ExtensionPointHandles;
};
