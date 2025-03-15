// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/LyraCloneHeroComponent.h"
#include "LyraCloneLogChannels.h"
#include "LyraClonePawnExtensionComponent.h"
#include "LyraCloneGameplayTags.h"
#include "Player/LyraClonePlayerState.h"
#include "Player/LyraClonePlayerController.h"
#include "Components/GameFrameworkComponentManager.h"
#include "LyraClonePawnData.h"
#include "Camera/LyraCloneCameraMode.h"
#include "Camera/LyraCloneCameraComponent.h"
#include "Input/LyraCloneMappableConfigPair.h"
#include "Input/LyraCloneInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "PlayerMappableInputConfig.h"
#include "AbilitySystem/LyraCloneAbilitySystemComponent.h"


#include UE_INLINE_GENERATED_CPP_BY_NAME(LyraCloneHeroComponent)

/** FeatureName 정의: static member variable 초기화 */
const FName ULyraCloneHeroComponent::NAME_ActorFeatureName("Hero");

/** InputConfig의 GameFeatureAction 활성화 ExtensioEvent 이름 */
const FName ULyraCloneHeroComponent::NAME_BindInputsNow("BindInputsNow");

ULyraCloneHeroComponent::ULyraCloneHeroComponent(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	PrimaryComponentTick.bStartWithTickEnabled = false;
	PrimaryComponentTick.bCanEverTick = false;
}

void ULyraCloneHeroComponent::OnRegister()
{
	Super::OnRegister();

	// 올바른 Actor에 등록되었는지 확인:
	{
		if (!GetPawn<APawn>())
		{
			UE_LOG(LogLyraClone, Error, TEXT("this component has been added to a BP whose base class is not a Pawn!"));
			return;
		}
	}

	RegisterInitStateFeature();
}

void ULyraCloneHeroComponent::BeginPlay()
{
	Super::BeginPlay();

	// PawnExtensionComponent에 대해서 (PawnExtension Feature) OnActorInitStateChanged() 관찰하도록 (Observing)
	BindOnActorInitStateChanged(ULyraClonePawnExtensionComponent::NAME_ActorFeatureName, FGameplayTag(), false);

	// InitState_Spawned로 초기화
	ensure(TryToChangeInitState(FLyraCloneGameplayTags::Get().InitState_Spawned));

	// ForceUpdate 진행
	CheckDefaultInitialization();
}

void ULyraCloneHeroComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	UnregisterInitStateFeature();

	Super::EndPlay(EndPlayReason);
}

void ULyraCloneHeroComponent::OnActorInitStateChanged(const FActorInitStateChangedParams& Params)
{
	const FLyraCloneGameplayTags& InitTags = FLyraCloneGameplayTags::Get();
	if (Params.FeatureName == ULyraClonePawnExtensionComponent::NAME_ActorFeatureName)
	{
		// LyraClonePawnExtensionComponent의 DataInitialized 상태 변화 관찰 후, LyraCloneHeroComponent도 DataInitialized 상태로 변경
		// - CanChangeInitState 확인
		if (Params.FeatureState == InitTags.InitState_DataInitialized)
		{
			CheckDefaultInitialization();
		}
	}
}

bool ULyraCloneHeroComponent::CanChangeInitState(UGameFrameworkComponentManager* Manager, FGameplayTag CurrentState, FGameplayTag DesiredState) const
{
	check(Manager);

	const FLyraCloneGameplayTags& InitTags = FLyraCloneGameplayTags::Get();
	APawn* Pawn = GetPawn<APawn>();
	ALyraClonePlayerState* LyraClonePS = GetPlayerState<ALyraClonePlayerState>();

	// Spawned 초기화
	// Onwer(=Pawn)이 잘 스폰되어 있으면 true
	if (!CurrentState.IsValid() && DesiredState == InitTags.InitState_Spawned)
	{
		if (Pawn)
		{
			return true;
		}
	}

	// StateChain에서 Spanwed의 다음 단계인 DataAvailable
	// Spawned -> DataAvailable
	if (CurrentState == InitTags.InitState_Spawned && DesiredState == InitTags.InitState_DataAvailable)
	{
		if (!LyraClonePS)
		{
			return false;
		}

		return true;
	}

	// StateChain에서 DataAvailable의 다음 단계인 DataInitialized
	// DataAvailable -> DataInitialized
	if (CurrentState == InitTags.InitState_DataAvailable && DesiredState == InitTags.InitState_DataInitialized)
	{
		// PawnExtensionComponent가 DataInitialized될 때까지 기다림 (== 모든 Feature Component가 DataAvailable인 상태)
		return LyraClonePS && Manager->HasFeatureReachedInitState(Pawn, ULyraClonePawnExtensionComponent::NAME_ActorFeatureName, InitTags.InitState_DataInitialized);
	}

	// StateChain에서 DataInitialized의 다음 단계인 GameplayReady
	// DataInitialized -> GameplayReady
	if (CurrentState == InitTags.InitState_DataInitialized && DesiredState == InitTags.InitState_GameplayReady)
	{
		return true;
	}

	return false;
}

void ULyraCloneHeroComponent::HandleChangeInitState(UGameFrameworkComponentManager* Manager, FGameplayTag CurrentState, FGameplayTag DesiredState)
{
	const FLyraCloneGameplayTags& InitTags = FLyraCloneGameplayTags::Get();

	// DataAvailable -> DataInitialized 단계
	if (CurrentState == InitTags.InitState_DataAvailable && DesiredState == InitTags.InitState_DataInitialized)
	{
		APawn* Pawn = GetPawn<APawn>();
		ALyraClonePlayerState* LyraClonePS = GetPlayerState<ALyraClonePlayerState>();
		if (!ensure(Pawn && LyraClonePS))
		{
			return;
		}

		const bool bIsLocallyControlled = Pawn->IsLocallyControlled();
		const ULyraClonePawnData* PawnData = nullptr;
		if (ULyraClonePawnExtensionComponent* PawnExtComp = ULyraClonePawnExtensionComponent::FindPawnExtensionComponent(Pawn))
		{
			PawnData = PawnExtComp->GetPawnData<ULyraClonePawnData>();

			// DataInitialized 단계까지 오면, Pawn이 Controller에 Possess되어 준비된 상태이다:
			// - InitAbilityActorInfo 호출로 AvatarActor 재설정이 필요하다
			PawnExtComp->InitializeAbilitySystem(LyraClonePS->GetLyraCloneAbilitySystemComponent(), LyraClonePS);
		}

		if (ULyraClonePawnExtensionComponent* PawnExtComp = ULyraClonePawnExtensionComponent::FindPawnExtensionComponent(Pawn))
		{
			PawnData = PawnExtComp->GetPawnData<ULyraClonePawnData>();
		}

		// Camera에 대한 핸들링
		if (bIsLocallyControlled && PawnData)
		{
			// 현재 LyraCloneCharacter에 Attach된 CameraComponent를 찾음
			if (ULyraCloneCameraComponent* CameraComponent = ULyraCloneCameraComponent::FindCameraComponent(Pawn))
			{
				CameraComponent->DetermineCameraModeDelegate.BindUObject(this, &ThisClass::DetermineCameraMode);
			}
		}

		// Input에 대한 핸들링
		if (ALyraClonePlayerController* LyraClonePC = GetController<ALyraClonePlayerController>())
		{
			if (Pawn->InputComponent != nullptr)
			{
				InitializePlayerInput(Pawn->InputComponent);
			}
		}
	}
}

void ULyraCloneHeroComponent::CheckDefaultInitialization()
{
	// 앞서 BindOnActorInitStateChanged에서 보았듯이 Hero Feature는 Pawn Extension Feature에 종속되어 있으므로, CheckDefaultInitializationForImplementers 호출하지 않음:

	// ContinueInitStateChain은 앞서 PawnExtComponent와 같음
	const FLyraCloneGameplayTags& InitTags = FLyraCloneGameplayTags::Get();
	static const TArray<FGameplayTag> StateChain = { InitTags.InitState_Spawned, InitTags.InitState_DataAvailable, InitTags.InitState_DataInitialized, InitTags.InitState_GameplayReady };
	ContinueInitStateChain(StateChain);
}

UE_DISABLE_OPTIMIZATION_SHIP
TSubclassOf<ULyraCloneCameraMode> ULyraCloneHeroComponent::DetermineCameraMode() const
{
	const APawn* Pawn = GetPawn<APawn>();
	if (!Pawn)
	{
		return nullptr;
	}

	// PawnExtensionComponent를 가져와서 캐싱된 PawnData를 가져와 CameraMode를 리턴 
	if (ULyraClonePawnExtensionComponent* PawnExtComp = ULyraClonePawnExtensionComponent::FindPawnExtensionComponent(Pawn))
	{
		if (const ULyraClonePawnData* PawnData = PawnExtComp->GetPawnData<ULyraClonePawnData>())
		{
			return PawnData->DefaultCameraMode;
		}
	}

	return nullptr;
}
UE_ENABLE_OPTIMIZATION_SHIP

void ULyraCloneHeroComponent::InitializePlayerInput(UInputComponent* PlayerInputComponent)
{
	check(PlayerInputComponent);

	const APawn* Pawn = GetPawn<APawn>();
	if (!Pawn)
	{
		return;
	}

	// LocalPlayer를 가져오기 위해
	const APlayerController* PC = GetController<APlayerController>();
	check(PC);

	// EnhancedInputLocalPlayerSubsystem 가져오기 위해
	const ULocalPlayer* LP = PC->GetLocalPlayer();
	check(LP);

	UEnhancedInputLocalPlayerSubsystem* Subsystem = LP->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>();
	check(Subsystem);

	// EnhancedInputLocalPlayerSubsystem에 MappingContext를 비워준다:
	Subsystem->ClearAllMappings();

	// PawnExtensionComponent -> PawnData -> InputConfig 존재 유무 판단:
	if (const ULyraClonePawnExtensionComponent* PawnExtComp = ULyraClonePawnExtensionComponent::FindPawnExtensionComponent(Pawn))
	{
		if (const ULyraClonePawnData* PawnData = PawnExtComp->GetPawnData<ULyraClonePawnData>())
		{
			if (const ULyraCloneInputConfig* InputConfig = PawnData->InputConfig)
			{
				const FLyraCloneGameplayTags& GameplayTags = FLyraCloneGameplayTags::Get();

				// HeroComponent 가지고 있는 Input Mapping Context를 순회하며, EnhancedInputLocalPlayerSubsystem에 추가한다
				for (const FLyraCloneMappableConfigPair& Pair : DefaultInputConfigs)
				{
					if (Pair.bShouldActivateAutomatically)
					{
						FModifyContextOptions Options = {};
						Options.bIgnoreAllPressedKeysUntilRelease = false;

						// 내부적으로 Input Mapping Context를 추가한다:
						// - AddPlayerMappableConfig를 간단히 보는 것을 추천
						Subsystem->AddPlayerMappableConfig(Pair.Config.LoadSynchronous(), Options);
					}
				}

				ULyraCloneInputComponent* LyraCloneIC = CastChecked<ULyraCloneInputComponent>(PlayerInputComponent);
				{
					// InputTag_Move와 InputTag_Look_Mouse에 대해 각각 Input_Move()와 Input_LookMouse() 멤버 함수에 바인딩시킨다:
					// - 바인딩한 이후, Input 이벤트에 따라 멤버 함수가 트리거된다
					{
						TArray<uint32> BindHandles;
						LyraCloneIC->BindAbilityActions(InputConfig, this, &ThisClass::Input_AbilityInputTagPressed, &ThisClass::Input_AbilityInputTagReleased, BindHandles);
					}

					// InputTag_Move와 InputTag_Look_Mouse에 대해 각각 Input_Move()와 Input_LookMouse() 멤버 함수에 바인딩시킨다:
					// - 바인딩한 이후, Input 이벤트에 따라 멤버 함수가 트리거된다
					LyraCloneIC->BindNativeAction(InputConfig, GameplayTags.InputTag_Move, ETriggerEvent::Triggered, this, &ThisClass::Input_Move, false);
					LyraCloneIC->BindNativeAction(InputConfig, GameplayTags.InputTag_Look_Mouse, ETriggerEvent::Triggered, this, &ThisClass::Input_LookMouse, false);
				}
			}
		}
	}

	// GameFeatureAction_AddInputConfig의 HandlePawnExtension 콜백 함수 전달
	UGameFrameworkComponentManager::SendGameFrameworkComponentExtensionEvent(const_cast<APawn*>(Pawn), NAME_BindInputsNow);
}

void ULyraCloneHeroComponent::Input_Move(const FInputActionValue& InputActionValue)
{
	APawn* Pawn = GetPawn<APawn>();
	AController* Controller = Pawn ? Pawn->GetController() : nullptr;

	if (Controller)
	{
		const FVector2D Value = InputActionValue.Get<FVector2D>();

		bool bLogging = true;
		if (bLogging)
		{
			UE_LOG(LogLyraClone, Log, TEXT("Input_Move[X=%.2f][Y=%.2f]"), Value.X, Value.Y);
		}

		const FRotator MovementRotation(0.0f, Controller->GetControlRotation().Yaw, 0.0f);

		if (Value.X != 0.0f)
		{
			// Left/Right -> X 값에 들어있음:
			// MovementDirection은 현재 카메라의 RightVector를 의미함 (World-Space)
			const FVector MovementDirection = MovementRotation.RotateVector(FVector::RightVector);

			// AddMovementInput 함수를 한번 보자:
			// - 내부적으로 MovementDirection * Value.X를 MovementComponent에 적용(더하기)해준다
			Pawn->AddMovementInput(MovementDirection, Value.X);
		}

		if (Value.Y != 0.0f) // 앞서 우리는 Forward 적용을 위해 swizzle input modifier를 사용했다~
		{
			// 앞서 Left/Right와 마찬가지로 Forward/Backward를 적용한다
			const FVector MovementDirection = MovementRotation.RotateVector(FVector::ForwardVector);
			Pawn->AddMovementInput(MovementDirection, Value.Y);
		}
	}
}

void ULyraCloneHeroComponent::Input_LookMouse(const FInputActionValue& InputActionValue)
{
	APawn* Pawn = GetPawn<APawn>();
	if (!Pawn)
	{
		return;
	}

	const FVector2D Value = InputActionValue.Get<FVector2D>();
	if (Value.X != 0.0f)
	{
		// X에는 Yaw 값이 있음:
		// - Camera에 대해 Yaw 적용
		Pawn->AddControllerYawInput(Value.X);
	}

	if (Value.Y != 0.0f)
	{
		// Y에는 Pitch 값!
		double AimInversionValue = -Value.Y;
		Pawn->AddControllerPitchInput(AimInversionValue);
	}
}

void ULyraCloneHeroComponent::Input_AbilityInputTagPressed(FGameplayTag InputTag)
{
	if (const APawn* Pawn = GetPawn<APawn>())
	{
		if (const ULyraClonePawnExtensionComponent* PawnExtComp = ULyraClonePawnExtensionComponent::FindPawnExtensionComponent(Pawn))
		{
			if (ULyraCloneAbilitySystemComponent* LyraCloneASC = PawnExtComp->GetLyraCloneAbilitySystemComponent())
			{
				LyraCloneASC->AbilityInputTagPressed(InputTag);
			}
		}
	}
}

void ULyraCloneHeroComponent::Input_AbilityInputTagReleased(FGameplayTag InputTag)
{
	if (const APawn* Pawn = GetPawn<APawn>())
	{
		if (const ULyraClonePawnExtensionComponent* PawnExtComp = ULyraClonePawnExtensionComponent::FindPawnExtensionComponent(Pawn))
		{
			if (ULyraCloneAbilitySystemComponent* LyraCloneASC = PawnExtComp->GetLyraCloneAbilitySystemComponent())
			{
				LyraCloneASC->AbilityInputTagReleased(InputTag);
			}
		}
	}
}
