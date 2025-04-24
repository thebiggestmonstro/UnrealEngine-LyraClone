// Fill out your copyright notice in the Description page of Project Settings.


#include "UIExtensionPointWidget.h"
#include "CommonLocalPlayer.h"
#include "GameFramework/PlayerState.h"

#define LOCTEXT_NAMESPACE "UIExtension"

UUIExtensionPointWidget::UUIExtensionPointWidget(const FObjectInitializer& ObjectInitializer) 
	: Super(ObjectInitializer)
{

}

void UUIExtensionPointWidget::ResetExtensionPoint()
{
	// UDynamicEntryBoxBase::ResetInternal() 호출
	ResetInternal();

	// UUserWidget은 알아서 GC 될 것이니 그냥 컨테이너만 Reset()
	ExtensionMapping.Reset();

	// ExtensionPointHandle을 순회하며, UUIExtensionSystem에서 제거(Unregister)
	for (FUIExtensionPointHandle& Handle : ExtensionPointHandles)
	{
		Handle.Unregister();
	}
	ExtensionPointHandles.Reset();
}

void UUIExtensionPointWidget::RegisterExtensionPoint()
{
	if (UUIExtensionSubsystem* ExtensionSubsystem = GetWorld()->GetSubsystem<UUIExtensionSubsystem>())
	{
		// UUserWidget을 포함하여, AllowedDataClasses를 생성
		TArray<UClass*> AllowedDataClasses;
		AllowedDataClasses.Add(UUserWidget::StaticClass());
		AllowedDataClasses.Append(DataClasses);

		// nullptr용 (ContextObject) ExtensionPoint 생성
		ExtensionPointHandles.Add(ExtensionSubsystem->RegisterExtensionPoint(
			ExtensionPointTag, ExtensionPointTagMatch, AllowedDataClasses,
			FExtendExtensionPointDelegate::CreateUObject(this, &ThisClass::OnAddOrRemoveExtension)
		));

		// LocalPlayer용 (ContextObject) ExtensionPoint 생성
		ExtensionPointHandles.Add(ExtensionSubsystem->RegisterExtensionPointForContext(
			ExtensionPointTag, GetOwningLocalPlayer(), ExtensionPointTagMatch, AllowedDataClasses,
			FExtendExtensionPointDelegate::CreateUObject(this, &ThisClass::OnAddOrRemoveExtension)
		));
	}
}

void UUIExtensionPointWidget::RegisterExtensionPointForPlayerState(UCommonLocalPlayer* LocalPlayer, APlayerState* PlayerState)
{
	// RegisterExtensionPoint랑 거의 같으니 생략
	if (UUIExtensionSubsystem* ExtensionSubsystem = GetWorld()->GetSubsystem<UUIExtensionSubsystem>())
	{
		TArray<UClass*> AllowedDataClasses;
		AllowedDataClasses.Add(UUserWidget::StaticClass());
		AllowedDataClasses.Append(DataClasses);

		ExtensionPointHandles.Add(ExtensionSubsystem->RegisterExtensionPointForContext(
			ExtensionPointTag, PlayerState, ExtensionPointTagMatch, AllowedDataClasses,
			FExtendExtensionPointDelegate::CreateUObject(this, &ThisClass::OnAddOrRemoveExtension)
		));
	}
}

void UUIExtensionPointWidget::OnAddOrRemoveExtension(EUIExtensionAction Action, const FUIExtensionRequest& Request)
{
	if (Action == EUIExtensionAction::Added)
	{
		UObject* Data = Request.Data;

		// Data는 앞서 이야기했듯이 WidgetClass이다
		TSubclassOf<UUserWidget> WidgetClass(Cast<UClass>(Data));
		if (WidgetClass)
		{
			// WidgetClass를 활용하여, UDynamicEntryBoxBase::CreateEntryInternal 호출로 Child Widget을 만든다
			UUserWidget* Widget = CreateEntryInternal(WidgetClass);

			// 제거할 경우, Tracking을 위해 ExtensionMapping 추가
			ExtensionMapping.Add(Request.ExtensionHandle, Widget);
		}
		// DataClasses 처리 (생략)
	}
	else
	{
		// ExtensionMapping을 활용하여, 안정하게 UDynamicEntryBoxBase::RemoveEntryInternal로 제거
		if (UUserWidget* Extension = ExtensionMapping.FindRef(Request.ExtensionHandle))
		{
			RemoveEntryInternal(Extension);
			ExtensionMapping.Remove(Request.ExtensionHandle);
		}
	}
}

TSharedRef<SWidget> UUIExtensionPointWidget::RebuildWidget()
{
	// 실제 InGame 렌더링될 때, 실행된다
	if (!IsDesignTime() && ExtensionPointTag.IsValid())
	{
		// UIExtensionPointWidget 내부 UDynamicEntryBoxBase::Child 모두 제거
		ResetExtensionPoint();

		// 다시 ExtensionPointWidget을 등록하기 위한 Delegate 및 Handle 추가
		RegisterExtensionPoint();

		// PlayerState 설정에 대한 Delegate를 통해 ExtensionPointWidget 추가를 위한 호출 확보
		FDelegateHandle Handle = GetOwningLocalPlayer<UCommonLocalPlayer>()->CallAndRegister_OnPlayerStateSet(
			UCommonLocalPlayer::FPlayerStateSetDelegate::FDelegate::CreateUObject(this, &UUIExtensionPointWidget::RegisterExtensionPointForPlayerState)
		);
	}

	// 여기는 UMG Editor에서 실행될때 실행되는 구간이다 (이를 IsDesignTime()으로 구분 가능)
	if (IsDesignTime())
	{
		auto GetExtensionPointText = [this]()
			{
				// ExtensionPoint
				// {ExtensionPointTag} 
				// - 위와 같은 형태로 표현될거다
				return FText::Format(LOCTEXT("DesignTime_ExtensionPointLabel", "ExtensionPoint\n{0}"), FText::FromName(ExtensionPointTag.GetTagName()));
			};

		// MessageBox라고 하지만, 그냥 Editor HUD라고 생각하면 된다 (SOverlay)
		TSharedRef<SOverlay> MessageBox = SNew(SOverlay);
		MessageBox->AddSlot()
			.Padding(0.5f)
			.HAlign(HAlign_Center)
			.VAlign(VAlign_Center)
			[
				// 가운데 정렬로 GetExtensionPointText 넣어준다
				SNew(STextBlock)
					.Justification(ETextJustify::Center)
					.Text_Lambda(GetExtensionPointText)
			];
		return MessageBox;
	}
	else
	{
		return Super::RebuildWidget();
	}
}