// Fill out your copyright notice in the Description page of Project Settings.


#include "Input/LyraCloneInputConfig.h"
#include "LyraClone/LyraCloneLogChannels.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(LyraCloneInputConfig)

ULyraCloneInputConfig::ULyraCloneInputConfig(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{

}

const UInputAction* ULyraCloneInputConfig::FindNativeInputActionForTag(const FGameplayTag& InputTag, bool bLogNotFound) const
{
	// NativeInputActions을 순회하며, Input으로 들어온 InputTag가 있는지 체크한다:
	// - 있으면, 그에 따른 InputAction을 반환하지만, 없다면, 그냥 nullptr을 반환한다.
	for (const FLyraCloneInputAction& Action : NativeInputActions)
	{
		if (Action.InputAction && (Action.InputTag == InputTag))
		{
			return Action.InputAction;
		}
	}

	if (bLogNotFound)
	{
		UE_LOG(LogLyraClone, Error, TEXT("can't find NativeInputAction for InputTag [%s] on InputConfig [%s]."), *InputTag.ToString(), *GetNameSafe(this));
	}

	return nullptr;
}

const UInputAction* ULyraCloneInputConfig::FindAbilityInputActionForTag(const FGameplayTag& InputTag, bool bLogNotFound) const
{
	for (const FLyraCloneInputAction& Action : AbilityInputActions)
	{
		if (Action.InputAction && (Action.InputTag == InputTag))
		{
			return Action.InputAction;
		}
	}

	if (bLogNotFound)
	{
		UE_LOG(LogLyraClone, Error, TEXT("Can't find AbilityInputAction for InputTag [%s] on InputConfig [%s]."), *InputTag.ToString(), *GetNameSafe(this));
	}

	return nullptr;
}
