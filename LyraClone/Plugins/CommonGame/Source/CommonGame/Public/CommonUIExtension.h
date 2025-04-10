// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "GameplayTagContainer.h"
#include "CommonUIExtension.generated.h"

class UCommonActivatableWidget;
class ULocalPlayer;

/**
 * 
 */
UCLASS()
class COMMONGAME_API UCommonUIExtension : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
	
public:
	UCommonUIExtension() {}

	static UCommonActivatableWidget* PushContentToLayer_ForPlayer(const ULocalPlayer* LocalPlayer, FGameplayTag LayerName, TSubclassOf<UCommonActivatableWidget> WidgetClass);
};
