// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/LyraCloneCharacter.h"

// Sets default values
ALyraCloneCharacter::ALyraCloneCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ALyraCloneCharacter::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ALyraCloneCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ALyraCloneCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

