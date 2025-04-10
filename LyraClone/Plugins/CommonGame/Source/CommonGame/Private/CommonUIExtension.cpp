// Fill out your copyright notice in the Description page of Project Settings.


#include "CommonUIExtension.h"
#include "CommonActivatableWidget.h"
#include "CommonLocalPlayer.h"
#include "GameUIManagerSubsystem.h"
#include "GameUIPolicy.h"
#include "PrimaryGameLayout.h"

UCommonActivatableWidget* UCommonUIExtension::PushContentToLayer_ForPlayer(const ULocalPlayer* LocalPlayer, FGameplayTag LayerName, TSubclassOf<UCommonActivatableWidget> WidgetClass)
{
	// LocalPlayer를 통해, GameUIManagerSubsystem을 가져옴
	if (UGameUIManagerSubsystem* UIManager = LocalPlayer->GetGameInstance()->GetSubsystem<UGameUIManagerSubsystem>())
	{
		// UIManager에서 현재 활성화된 UIPolicy를 가져오고
		if (UGameUIPolicy* Policy = UIManager->GetCurrentUIPolicy())
		{
			// Policy에서 LocalPlayer에 맞는 PrimaryGameLayout을 가져온다
			if (UPrimaryGameLayout* RootLayout = Policy->GetRootLayout(CastChecked<UCommonLocalPlayer>(LocalPlayer)))
			{
				// 그리고 PrimaryGameLayout, W_OverallUILayout의 LayerName에 Stack으로 WidgetClass(UCommonActivatableWidget)를 넣어준다
				return RootLayout->PushWidgetToLayerStack(LayerName, WidgetClass);
			}
		}
	}
	return nullptr;
}

