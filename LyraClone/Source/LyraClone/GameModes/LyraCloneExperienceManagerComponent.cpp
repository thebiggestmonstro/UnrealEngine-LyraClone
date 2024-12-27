// Fill out your copyright notice in the Description page of Project Settings.


#include "GameModes/LyraCloneExperienceManagerComponent.h"

void ULyraCloneExperienceManagerComponent::CallOrRegister_OnExperienceLoaded(FOnLyraCloneExperienceLoaded::FDelegate&& Delegate)
{
	// IsExperienceLoaded() 구현
	// 현재 프레임에 이미 Exeperience가 로드되어 있다면, 바로 델리게이트를 호출
	if (IsExperienceLoaded())
	{
		Delegate.Execute(CurrentExperience);
	}
	// 현재 프레임에 로드되어 있지 않다면,
	else
	{
		/**
		 * 참고로, 여러분들은 Delegate 객체를 자세히 살펴보면, 내부적으로 필요한 변수들은 메모리 할당해놓는다:
		 * TArray<int> a = {1, 2, 3, 4};
		 * delegate_type delegate = [a](){
		 *	return a.Num();
		 * }
		 * a는 delegate_type 내부에 new로 할당되어 있다. 복사 비용을 낮추기 위해 Move를 통해 하는 것을 잊지 말자!
		 */
		OnExperienceLoaded.Add(MoveTemp(Delegate));
	}
}