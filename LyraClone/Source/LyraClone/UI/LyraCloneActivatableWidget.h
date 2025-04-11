// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CommonActivatableWidget.h"
#include "LyraCloneActivatableWidget.generated.h"

/**
 * Input 처리 방식 정의
 */
UENUM(BlueprintType)
enum class ELyraCloneWidgetInputMode : uint8
{
	Default,
	GameAndMenu,
	Game,
	Menu,
};

/**
 * CommonActivatableWidget의 주석을 보면, 복잡하게 설명되어 있다.
 * 필자가 이해한 CommonActivatableWidget은 두가지 특성을 가지고 있다:
 * 1. Widget Layout과 Widget Instance을 구분하기 위한 용도로 CommonActivatableWidget은 Layout 정의:
 *    - 런타임 Activate/Deactivate
 *    - WidgetTree에서 제거가 아닌 껐다/켰다(== Activate/Deactivate)
 * 2. Input(Mouse or Keyboard etc...) 처리 방법 정의
 */

/**
 * 
 */
UCLASS()
class LYRACLONE_API ULyraCloneActivatableWidget : public UCommonActivatableWidget
{
	GENERATED_BODY()
	
public:
	ULyraCloneActivatableWidget(const FObjectInitializer& ObjectInitializer);

	/**
	 * UCommonActivatableWidget's interfaces
	 */
	virtual TOptional<FUIInputConfig> GetDesiredInputConfig() const override;

	/** Input 처리 방식 */
	UPROPERTY(EditDefaultsOnly, Category = Input)
	ELyraCloneWidgetInputMode InputConfig = ELyraCloneWidgetInputMode::Default;

	/** Mouse 처리 방식 */
	UPROPERTY(EditDefaultsOnly, Category = Input)
	EMouseCaptureMode GameMouseCaptureMode = EMouseCaptureMode::CapturePermanently;
};
