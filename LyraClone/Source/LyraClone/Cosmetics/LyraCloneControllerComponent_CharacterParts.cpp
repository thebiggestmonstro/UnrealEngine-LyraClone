// Fill out your copyright notice in the Description page of Project Settings.

#include "LyraCloneControllerComponent_CharacterParts.h"
#include "LyraClonePawnComponent_CharacterParts.h"

ULyraCloneControllerComponent_CharacterParts::ULyraCloneControllerComponent_CharacterParts(const FObjectInitializer& ObjectInitializer) 
	: Super(ObjectInitializer)
{

}

void ULyraCloneControllerComponent_CharacterParts::BeginPlay()
{
	Super::BeginPlay();

	if (HasAuthority())
	{
		if (AController* OwningController = GetController<AController>())
		{
			OwningController->OnPossessedPawnChanged.AddDynamic(this, &ThisClass::OnPossessedPawnChanged);
		}
	}
}

void ULyraCloneControllerComponent_CharacterParts::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	RemoveAllCharacterParts();
	Super::EndPlay(EndPlayReason);
}

PRAGMA_DISABLE_OPTIMIZATION
ULyraClonePawnComponent_CharacterParts* ULyraCloneControllerComponent_CharacterParts::GetPawnCustomizer() const
{
	if (APawn* ControlledPawn = GetPawn<APawn>())
	{
		// 생각해보면, 우리는 앞서 LyraClonePawnComponent_CharacterParts를 상속받는 B_MannequinPawnCosmetics를 이미 B_Hero_ShooterMannequin에 추가하였다.
		// B_MannequinPawnCosmetics를 반환되길 기대한다
		return ControlledPawn->FindComponentByClass<ULyraClonePawnComponent_CharacterParts>();
	}
	return nullptr;
}
PRAGMA_ENABLE_OPTIMIZATION

void ULyraCloneControllerComponent_CharacterParts::AddCharacterPart(const FLyraCloneCharacterPart& NewPart)
{
	AddCharacterPartInternal(NewPart);
}

void ULyraCloneControllerComponent_CharacterParts::AddCharacterPartInternal(const FLyraCloneCharacterPart& NewPart)
{
	FLyraCloneControllerCharacterPartEntry& NewEntry = CharacterParts.AddDefaulted_GetRef();
	NewEntry.Part = NewPart;

	if (ULyraClonePawnComponent_CharacterParts* PawnCustomizer = GetPawnCustomizer())
	{
		NewEntry.Handle = PawnCustomizer->AddCharacterPart(NewPart);
	}
}

void ULyraCloneControllerComponent_CharacterParts::RemoveAllCharacterParts()
{
	if (ULyraClonePawnComponent_CharacterParts* PawnCustomizer = GetPawnCustomizer())
	{
		for (FLyraCloneControllerCharacterPartEntry& Entry : CharacterParts)
		{
			PawnCustomizer->RemoveCharacterPart(Entry.Handle);
		}
	}
	CharacterParts.Reset();
}

void ULyraCloneControllerComponent_CharacterParts::OnPossessedPawnChanged(APawn* OldPawn, APawn* NewPawn)
{
	// 이전 OldPawn에 대해서는 Character Parts를 전부 제거해주자
	if (ULyraClonePawnComponent_CharacterParts* OldCustomizer = OldPawn ? OldPawn->FindComponentByClass<ULyraClonePawnComponent_CharacterParts>() : nullptr)
	{
		for (FLyraCloneControllerCharacterPartEntry& Entry : CharacterParts)
		{
			OldCustomizer->RemoveCharacterPart(Entry.Handle);
			Entry.Handle.Reset();
		}
	}

	// 새로운 Pawn에 대해서 기존 Controller가 가지고 있는 Character Parts를 추가해주자
	if (ULyraClonePawnComponent_CharacterParts* NewCustomizer = NewPawn ? NewPawn->FindComponentByClass<ULyraClonePawnComponent_CharacterParts>() : nullptr)
	{
		for (FLyraCloneControllerCharacterPartEntry& Entry : CharacterParts)
		{
			check(!Entry.Handle.IsValid());
			Entry.Handle = NewCustomizer->AddCharacterPart(Entry.Part);
		}
	}
}
