// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameplayTagContainer.h"
#include "Templates/SharedPointer.h"
#include "Subsystems/WorldSubsystem.h"
#include "UIExtensionSystem.generated.h"

class UUIExtensionSubsystem;

struct FUIExtension : TSharedFromThis<FUIExtension>
{
	/** UIExtension Widget의 Point Tag이다 (무슨 의미인지 하나씩 구현내가며 이해해보자) */
	FGameplayTag ExtensionPointTag;

	/** Widget Class를 가지고 있으며, UUIExtensionSubsystem에서 AddReferencedObjects에서 GC를 막는다 */
	UObject* Data = nullptr;

	/** 보통 LocalPlayer로 설정된다 */
	TWeakObjectPtr<UObject> ContextObject;

	int32 Priority = INDEX_NONE;
};

/** ExtensionPoint GameplayTag 비교 방법 */
UENUM(BlueprintType)
enum class EUIExtensionPointMatch : uint8
{
	/** A.B는 A.B.C를 ***허용하지*** 않는다 */
	ExactMatch,

	/** A.B는 A.B.C를 ***허용*** 한다 */
	PartialMatch,
};

DECLARE_DELEGATE_TwoParams(FExtendExtensionPointDelegate, EUIExtensionAction Action, const FUIExtensionRequest& Request);

struct FUIExtensionPoint : TSharedFromThis<FUIExtensionPoint>
{
public:
	/** Extension---ExtensionPoint와 매칭되는지 확인 */
	bool DoesExtensionPassContract(const FUIExtension* Extension) const;

	/** UIExtension의 Slot GameplayTag */
	FGameplayTag ExtensionPointTag;

	/** UIExtension을 생성/제거한 Instigator(주체) */
	TWeakObjectPtr<UObject> ContextObject;

	/** UIExtensionPointWidget에 허용된 Widget Class -> UIExtensionPointWidget's DataClasses */
	TArray<UClass*> AllowedDataClasses;

	/** Widget을 ExtensionPointWidget에 연결하기 위한 Callback 함수 */
	FExtendExtensionPointDelegate Callback;
	EUIExtensionPointMatch ExtensionPointTagMatchType = EUIExtensionPointMatch::ExactMatch;
};

USTRUCT(BlueprintType)
struct UIEXTENSION_API FUIExtensionPointHandle
{
	GENERATED_BODY()
	
public:
	FUIExtensionPointHandle() {}
	FUIExtensionPointHandle(UUIExtensionSubsystem* InExtensionSource, const TSharedPtr<FUIExtensionPoint>& InDataPtr)
		: ExtensionSource(InExtensionSource), DataPtr(InDataPtr)
	{}

	void Unregister();
	bool IsValid() const { return DataPtr.IsValid(); }
	bool operator==(const FUIExtensionPointHandle& Other) const { return DataPtr == Other.DataPtr; }
	bool operator!=(const FUIExtensionPointHandle& Other) const { return !operator==(Other); }

	TWeakObjectPtr<UUIExtensionSubsystem> ExtensionSource;
	TSharedPtr<FUIExtensionPoint> DataPtr;
};

USTRUCT(BlueprintType)
struct UIEXTENSION_API FUIExtensionHandle
{
	GENERATED_BODY()
	
public:
	FUIExtensionHandle() {}
	FUIExtensionHandle(UUIExtensionSubsystem* InExtensionSource, const TSharedPtr<FUIExtension>& InDataPtr)
		: ExtensionSource(InExtensionSource)
		, DataPtr(InDataPtr)
	{}

	void Unregister();
	bool IsValid() const { return DataPtr.IsValid(); }
	bool operator==(const FUIExtensionHandle& Other) const { return DataPtr == Other.DataPtr; }
	bool operator!=(const FUIExtensionHandle& Other) const { return !operator==(Other); }

	friend FORCEINLINE uint32 GetTypeHash(FUIExtensionHandle Handle)
	{
		return PointerHash(Handle.DataPtr.Get());
	}

	friend class UUIExtensionSubsystem;
	TWeakObjectPtr<UUIExtensionSubsystem> ExtensionSource;
	TSharedPtr<FUIExtension> DataPtr;
};

template <>
struct TStructOpsTypeTraits<FUIExtensionHandle> : public TStructOpsTypeTraitsBase2<FUIExtensionHandle>
{
	enum
	{
		WithCopy = true,
		WithIdenticalViaEquality = true,
	};
};

USTRUCT(BlueprintType)
struct FUIExtensionRequest
{
	GENERATED_BODY()
	public:
	/** UIExtensionPoint와 연동될 Extension */
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FUIExtensionHandle ExtensionHandle;

	/** Extension의 Slot GameplayTag */
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FGameplayTag ExtensionPointTag;

	/** WidgetClass로 FUIExtension과 같다 */
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TObjectPtr<UObject> Data = nullptr;

	/** FUIExtension의 ContextObject를 전달받음 (UCommonLocalPlayer, UModularPlayerState, ... == UIExtension을 실행한 Instigator) */
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TObjectPtr<UObject> ContextObject = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	int32 Priority = INDEX_NONE;
};

UENUM(BLueprintType)
enum class EUIExtensionAction : uint8
{
	Added,
	Removed,
};

UCLASS()
class UIEXTENSION_API UUIExtensionSubsystem : public UWorldSubsystem
{
	GENERATED_BODY()
	
public:
	void UnregisterExtension(const FUIExtensionHandle& ExtensionHandle);
	FUIExtensionHandle RegisterExtensionAsWidgetForContext(const FGameplayTag& ExtensionPointTag, UObject* ContextObject, TSubclassOf<UUserWidget> WidgetClass, int32 Priority);
	FUIExtensionHandle RegisterExtensionAsData(const FGameplayTag& ExtensionPointTag, UObject* ContextObject, UObject* Data, int32 Priority);

	void UnregisterExtensionPoint(const FUIExtensionPointHandle& ExtensionPointHandle);
	FUIExtensionPointHandle RegisterExtensionPointForContext(const FGameplayTag& ExtensionPointTag, UObject* ContextObject, EUIExtensionPointMatch ExtensionPointTagMatchType, const TArray<UClass*>& AllowedDataClasses, FExtendExtensionPointDelegate ExtensionCallback);
	FUIExtensionPointHandle RegisterExtensionPoint(const FGameplayTag& ExtensionPointTag, EUIExtensionPointMatch ExtensionPointTagMatchType, const TArray<UClass*>& AllowedDataClasses, FExtendExtensionPointDelegate ExtensionCallback);

	FUIExtensionRequest CreateExtensionRequest(const TSharedPtr<FUIExtension>& Extension);

	/** ExtensionPoint --(Broadcast)--> Extensions [ExtensionPoin가 추가/제거 되었을 경우, Extension에 알림: 참고로 Added만 함] */
	void NotifyExtensionPointOfExtensions(TSharedPtr<FUIExtensionPoint>& ExtensionPoint);
	/** Extension --(Broadcast) --> ExtensionPoints [Extension이 추가/제거 되었을 경우, Extension Points에 알림] */
	void NotifyExtensionPointsOfExtension(EUIExtensionAction Action, TSharedPtr<FUIExtension>& Extension);

	/** GameplayTag(Slot) --- FUIExtension(WidgetClass) */
	typedef TArray<TSharedPtr<FUIExtension>> FExtensionList;
	TMap<FGameplayTag, FExtensionList> ExtensionMap;

	/** GameplayTag(Slot) --- FUIExtensionPoint(WidgetClassWithContext) */
	typedef TArray<TSharedPtr<FUIExtensionPoint>> FExtensionPointList;
	TMap<FGameplayTag, FExtensionPointList> ExtensionPointMap;
};