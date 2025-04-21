// Fill out your copyright notice in the Description page of Project Settings.


#include "UIExtensionSystem.h"
#include "Blueprint/UserWidget.h"

bool FUIExtensionPoint::DoesExtensionPassContract(const FUIExtension* Extension) const
{
	if (UObject* DataPtr = Extension->Data)
	{
		const bool bMatchContext =
			// ContextObject와 Extension->ContextObject가 둘다 nullptr로 세팅되었는지?
			(ContextObject.IsExplicitlyNull() && Extension->ContextObject.IsExplicitlyNull()) ||
			// 아님 실제 ContextObject와 Extension->ContextObject가 같은지? (LocalPlayer? PlayerState?)
			(ContextObject == Extension->ContextObject);

		if (bMatchContext)
		{
			// DataClass (WidgetClass 추출)
			const UClass* DataClass = DataPtr->IsA(UClass::StaticClass()) ? Cast<UClass>(DataPtr) : DataPtr->GetClass();
			for (const UClass* AllowedDataClass : AllowedDataClasses)
			{
				// AllowedDataClasses를 순회하며 Child/Interface인가 확인
				if (DataClass->IsChildOf(AllowedDataClass) || DataClass->ImplementsInterface(AllowedDataClass))
				{
					return true;
				}
			}
		}
	}
	return false;
}

void FUIExtensionPointHandle::Unregister()
{
	if (UUIExtensionSubsystem* ExtensionSourcePtr = ExtensionSource.Get())
	{
		ExtensionSourcePtr->UnregisterExtensionPoint(*this);
	}
}

void FUIExtensionHandle::Unregister()
{
	if (UUIExtensionSubsystem* ExtensionSourcePtr = ExtensionSource.Get())
	{
		ExtensionSourcePtr->UnregisterExtension(*this);
	}
}

void UUIExtensionSubsystem::UnregisterExtension(const FUIExtensionHandle& ExtensionHandle)
{
	if (ExtensionHandle.IsValid())
	{
		// 반드시 해당 ExtensionHandle이 UUIExtensionSubsystem과 같은지 확인해야 함!
		checkf(ExtensionHandle.ExtensionSource == this, TEXT("Trying to unregister an extension that's not from this extension subsystem"));

		TSharedPtr<FUIExtension> Extension = ExtensionHandle.DataPtr;

		// Extension의 PointTag를 통해 ExtensionMap에서 해당 Slot에 있는지 찾아서 제거
		if (FExtensionList* ListPtr = ExtensionMap.Find(Extension->ExtensionPointTag))
		{
			// Extension 제거, NotifyExtensionPointsOfExtension 호출
			NotifyExtensionPointsOfExtension(EUIExtensionAction::Removed, Extension);

			ListPtr->RemoveSwap(Extension);
			if (ListPtr->Num() == 0)
			{
				// Num() == 0이면 Map에서도 제거 진행
				ExtensionMap.Remove(Extension->ExtensionPointTag);
			}
		}
	}
}

FUIExtensionHandle UUIExtensionSubsystem::RegisterExtensionAsWidgetForContext(const FGameplayTag& ExtensionPointTag, UObject* ContextObject, TSubclassOf<UUserWidget> WidgetClass, int32 Priority)
{
	return RegisterExtensionAsData(ExtensionPointTag, ContextObject, WidgetClass, Priority);
}

FUIExtensionHandle UUIExtensionSubsystem::RegisterExtensionAsData(const FGameplayTag& ExtensionPointTag, UObject* ContextObject, UObject* Data, int32 Priority)
{
	// ExtensionPointTag(Slot)이 Invalid
	if (!ExtensionPointTag.IsValid())
	{
		return FUIExtensionHandle();
	}

	// WidgetClass가 없으면
	if (!Data)
	{
		return FUIExtensionHandle();
	}

	// ExtensionPointTag를 기반하여, ExtensionList를 추출
	FExtensionList& List = ExtensionMap.FindOrAdd(ExtensionPointTag);

	// 새로운 UIExtension을 추가 진행
	TSharedPtr<FUIExtension>& Entry = List.Add_GetRef(MakeShared<FUIExtension>());
	Entry->ExtensionPointTag = ExtensionPointTag;
	Entry->ContextObject = ContextObject;
	Entry->Data = Data;
	Entry->Priority = Priority;

	// Extension이 추가되었으니 Added로 NotifyExtensionPointsOfExtension 실행
	NotifyExtensionPointsOfExtension(EUIExtensionAction::Added, Entry);

	return FUIExtensionHandle(this, Entry);
}

void UUIExtensionSubsystem::UnregisterExtensionPoint(const FUIExtensionPointHandle& ExtensionPointHandle)
{
	// 이전 UnregisterExtension()과 거의 흡사하니 스킵하장
	if (ExtensionPointHandle.IsValid())
	{
		check(ExtensionPointHandle.ExtensionSource == this);

		const TSharedPtr<FUIExtensionPoint> ExtensionPoint = ExtensionPointHandle.DataPtr;
		if (FExtensionPointList* ListPtr = ExtensionPointMap.Find(ExtensionPoint->ExtensionPointTag))
		{
			ListPtr->RemoveSwap(ExtensionPoint);
			if (ListPtr->Num() == 0)
			{
				ExtensionPointMap.Remove(ExtensionPoint->ExtensionPointTag);
			}
		}
	}
}

FUIExtensionPointHandle UUIExtensionSubsystem::RegisterExtensionPointForContext(const FGameplayTag& ExtensionPointTag, UObject* ContextObject, EUIExtensionPointMatch ExtensionPointTagMatchType, const TArray<UClass*>& AllowedDataClasses, FExtendExtensionPointDelegate ExtensionCallback)
{
	if (!ExtensionPointTag.IsValid())
	{
		return FUIExtensionPointHandle();
	}

	// ExtensionCallback이 바인딩되어 있지 않으면 Widget을 추가할 수가 없다.. 의미가 없으니 그냥 리턴한다
	if (!ExtensionCallback.IsBound())
	{
		return FUIExtensionPointHandle();
	}

	// 허용된 Widget Class가 없으므로, 리턴
	if (AllowedDataClasses.Num() == 0)
	{
		return FUIExtensionPointHandle();
	}

	// ExtensionPointMap에 새로운 Entry를 등록한다
	// - List 내에 중복 체크는 따로 하지 않는다! (Register/Unregister 쌍을 잘 맞추어야 한다는 의미이다)
	FExtensionPointList& List = ExtensionPointMap.FindOrAdd(ExtensionPointTag);
	TSharedPtr<FUIExtensionPoint>& Entry = List.Add_GetRef(MakeShared<FUIExtensionPoint>());
	Entry->ExtensionPointTag = ExtensionPointTag;
	Entry->ContextObject = ContextObject;
	Entry->ExtensionPointTagMatchType = ExtensionPointTagMatchType;
	Entry->AllowedDataClasses = AllowedDataClasses;
	Entry->Callback = MoveTemp(ExtensionCallback);

	// ExtensionPoint가 추가되었으니, NotifyExtensionPointOfExtensions 호출
	NotifyExtensionPointOfExtensions(Entry);

	return FUIExtensionPointHandle(this, Entry);
}

FUIExtensionPointHandle UUIExtensionSubsystem::RegisterExtensionPoint(const FGameplayTag& ExtensionPointTag, EUIExtensionPointMatch ExtensionPointTagMatchType, const TArray<UClass*>& AllowedDataClasses, FExtendExtensionPointDelegate ExtensionCallback)
{
	return RegisterExtensionPointForContext(ExtensionPointTag, nullptr, ExtensionPointTagMatchType, AllowedDataClasses, ExtensionCallback);
}

FUIExtensionRequest UUIExtensionSubsystem::CreateExtensionRequest(const TSharedPtr<FUIExtension>& Extension)
{
	FUIExtensionRequest Request;
	Request.ExtensionHandle = FUIExtensionHandle(this, Extension);
	Request.ExtensionPointTag = Extension->ExtensionPointTag;
	Request.Priority = Extension->Priority;
	Request.Data = Extension->Data;
	Request.ContextObject = Extension->ContextObject.Get();
	return Request;
}

void UUIExtensionSubsystem::NotifyExtensionPointOfExtensions(TSharedPtr<FUIExtensionPoint>& ExtensionPoint)
{
	// 헷갈리지 말장: 현재 ExtensionPoint 변화점이 왔다!

	// ExtensionPoint의 ExtensionPointTag(Slot)을 순회하자
	for (FGameplayTag Tag = ExtensionPoint->ExtensionPointTag; Tag.IsValid(); Tag = Tag.RequestDirectParent())
	{
		// ExtensionMap을 통해 ExtensionPointTag에 매칭되는 ExtensionList를 가져오자
		if (const FExtensionList* ListPtr = ExtensionMap.Find(Tag))
		{
			// ExtensionList를 순회하며!
			FExtensionList ExtensionArray(*ListPtr);
			for (const TSharedPtr<FUIExtension>& Extension : ExtensionArray)
			{
				// 현재 ***ExtentionPoint***가 순회하는 Extension에 허용되는지 확인
				if (ExtensionPoint->DoesExtensionPassContract(Extension.Get()))
				{
					// UIExtensionRequest를 만들고, ExtensionPoint의 UIExtensionPointWidget의 Add를 실행
					FUIExtensionRequest Request = CreateExtensionRequest(Extension);
					ExtensionPoint->Callback.ExecuteIfBound(EUIExtensionAction::Added, Request);
				}
			}
		}

		// ExactMatch라면 굳이 GameplayTag의 Parent를 순회할 필요가 없다
		if (ExtensionPoint->ExtensionPointTagMatchType == EUIExtensionPointMatch::ExactMatch)
		{
			break;
		}
	}
}

void UUIExtensionSubsystem::NotifyExtensionPointsOfExtension(EUIExtensionAction Action, TSharedPtr<FUIExtension>& Extension)
{
	// 헷갈리지 말자: 현재 Extension 변경점이 왔다!

	// bOnInitialTag를 활용하는 이유? ExactMatch 로직을 위해서!
	bool bOnInitialTag = true;
	for (FGameplayTag Tag = Extension->ExtensionPointTag; Tag.IsValid(); Tag = Tag.RequestDirectParent())
	{
		if (const FExtensionPointList* ListPtr = ExtensionPointMap.Find(Tag))
		{
			// ExtensionPoint를 순회
			FExtensionPointList ExtensionPointArray(*ListPtr);
			for (const TSharedPtr<FUIExtensionPoint>& ExtensionPoint : ExtensionPointArray)
			{
				/** ExactMatch용 + PartialMatch용 */
				if (bOnInitialTag || (ExtensionPoint->ExtensionPointTagMatchType == EUIExtensionPointMatch::PartialMatch))
				{
					if (ExtensionPoint->DoesExtensionPassContract(Extension.Get()))
					{
						FUIExtensionRequest Request = CreateExtensionRequest(Extension);
						ExtensionPoint->Callback.ExecuteIfBound(Action, Request);
					}
				}
			}
		}

		// 흠.. 근데 왜 다 순회할까?
		// - ExtensionPointTagMatchType이 UIExtensionPoint 안에 있으므로!
		bOnInitialTag = false;
	}
}