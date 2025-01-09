// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/LyraCloneCharacter.h"
#include "LyraClonePawnExtensionComponent.h"
#include "Camera/LyraCloneCameraComponent.h"

// Sets default values
ALyraCloneCharacter::ALyraCloneCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	// PawnExtComponent 생성
	PawnExtComponent = CreateDefaultSubobject<ULyraClonePawnExtensionComponent>(TEXT("PawnExtensionComponent"));
	
	// CameraComponent 생성
	{
		CameraComponent = CreateDefaultSubobject<ULyraCloneCameraComponent>(TEXT("CameraComponent"));
		CameraComponent->SetRelativeLocation(FVector(-300.0f, 0.0f, 75.0f));
	}
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

	// Pawn이 Possess로서, Controller와 PlayerState 정보 접근이 가능한 상태가 되었음:
	// - SetupPlayerInputComponent 확인
	PawnExtComponent->SetupPlayerInputComponent();
}