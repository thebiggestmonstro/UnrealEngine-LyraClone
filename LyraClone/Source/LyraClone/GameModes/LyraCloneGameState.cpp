// Fill out your copyright notice in the Description page of Project Settings.


#include "GameModes/LyraCloneGameState.h"
#include "LyraCloneExperienceManagerComponent.h"

ALyraCloneGameState::ALyraCloneGameState()
{
	ExperienceManagerComponent = CreateDefaultSubobject<ULyraCloneExperienceManagerComponent>(TEXT("ExperienceManagerComponent"));
}