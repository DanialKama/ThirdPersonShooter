// All Rights Reserved.

#include "Characters/PlayerCharacter.h"
#include "Components/HealthComponent.h"
#include "Components/SlateWrapperTypes.h"
#include "Core/ShooterPlayerController.h"
#include "GameFramework/HUD.h"
#include "Interfaces/HUDInterface.h"
#include "Interfaces/PlayerControllerInterface.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/Classes/Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "UI/ShooterHUD.h"
#include "Components/TimelineComponent.h"

// Sets default values
APlayerCharacter::APlayerCharacter()
{
	// Create Components
	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("Camera Boom"));
	TPP = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));

	// Setup components attachment
	SpringArm->SetupAttachment(GetRootComponent());
	TPP->SetupAttachment(SpringArm);

	// Set component defaults
	SpringArm->SetRelativeLocation(FVector(0.0f, 0.0f, 75.0f));
	SpringArm->ProbeSize = 10.0;
	SpringArm->bUsePawnControlRotation = true;
	SpringArm->bEnableCameraLag = true;

	// Initialize variables
	bDoOnceCrouch = true;
}

// Called to bind functionality to input
void APlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	// Super::SetupPlayerInputComponent(PlayerInputComponent);
	if (InputComponent)
	{
		// Input Actions
		InputComponent->BindAction("Jump", IE_Pressed, this, &ABaseCharacter::StartJump);
		InputComponent->BindAction("Jump", IE_Released, this, &APlayerCharacter::StopJumping);
		InputComponent->BindAction("Sprint", IE_Pressed, this, &APlayerCharacter::StartSprinting);
		InputComponent->BindAction("Sprint", IE_Released, this, &APlayerCharacter::StopSprinting);
		InputComponent->BindAction("Crouch", IE_Pressed, this, &APlayerCharacter::TryToToggleCrouch);
		InputComponent->BindAction("Crouch", IE_Released, this, &APlayerCharacter::ResetCrouchByDelay);
		InputComponent->BindAction("Aim", IE_Pressed, this, &APlayerCharacter::TryToStartAiming);
		InputComponent->BindAction("Aim", IE_Released, this, &APlayerCharacter::ResetAim);
		InputComponent->BindAction("Fire", IE_Pressed, this, &ABaseCharacter::StartFireWeapon);
		InputComponent->BindAction("Fire", IE_Released, this, &ABaseCharacter::StopFireWeapon);
		InputComponent->BindAction("Reload", IE_Pressed, this, &ABaseCharacter::ReloadWeapon);
		InputComponent->BindAction("SwitchToPrimary", IE_Pressed, this, &ABaseCharacter::SwitchToPrimary);
		InputComponent->BindAction("SwitchToSecondary", IE_Pressed, this, &ABaseCharacter::SwitchToSecondary);
		InputComponent->BindAction("SwitchToSidearm", IE_Pressed, this, &ABaseCharacter::SwitchToSidearm);
		InputComponent->BindAction("SwitchToNext", IE_Pressed, this, &APlayerCharacter::SwitchToNextWeapon);
		InputComponent->BindAction("SwitchToPrevious", IE_Pressed, this, &APlayerCharacter::SwitchToPreviousWeapon);
		InputComponent->BindAction("DropItem", IE_Pressed, this, &ABaseCharacter::DropItem);
		// Input Axis
		InputComponent->BindAxis("MoveForward", this, &APlayerCharacter::AddToForwardMovement);
		InputComponent->BindAxis("MoveRight", this, &APlayerCharacter::AddToRightMovement);
		InputComponent->BindAxis("Turn", this, &APlayerCharacter::AddControllerYawInput);
		InputComponent->BindAxis("LookUp", this, &APlayerCharacter::AddControllerPitchInput);
		InputComponent->BindAxis("TurnRate", this, &APlayerCharacter::GamepadAddToYaw);
		InputComponent->BindAxis("LookUpRate", this, &APlayerCharacter::GamepadAddToPitch);
	}
}

void APlayerCharacter::BeginPlay()
{
	Super::BeginPlay();
	UGameplayStatics::GetPlayerCameraManager(GetWorld(), 0)->ViewPitchMax = 80.0f;
	UGameplayStatics::GetPlayerCameraManager(GetWorld(), 0)->ViewPitchMin = -80.0f;
	if (GetController() && GetController()->GetClass()->ImplementsInterface(UPlayerControllerInterface::StaticClass()))
	{
		PlayerController = IPlayerControllerInterface::Execute_GetPlayerControllerReference(GetController());
		if(PlayerController)
		{
			PlayerController->PlayerTransform = GetActorTransform();
			if (PlayerController->GetHUD()->GetClass()->ImplementsInterface(UPlayerControllerInterface::StaticClass()))
			{
				HUD = IHUDInterface::Execute_GetHUDReference(PlayerController->GetHUD());
				if (HUD)
				{
					HUD->SetHealth_Implementation(HealthComponent->DefaultHealth / HealthComponent->MaxHealth);
					HUD->SetUIVisibility_Implementation(ESlateVisibility::Visible);
				}
			}
		}
	}
}

void APlayerCharacter::StartSprinting()
{
	bTapHeld = true;
	TabNumber = FMath::Clamp(TabNumber + 1, 1, 2);
	PreviousTapNumber = TabNumber;
	FTimerHandle TimerHandle;
	GetWorld()->GetTimerManager().SetTimer(TimerHandle, this, &APlayerCharacter::DoubleTabHandler, TapThreshold);
	bDoubleTabGate = false;
}

void APlayerCharacter::StopSprinting()
{
	bTapHeld = false;
	FTimerHandle TimerHandle;
	GetWorld()->GetTimerManager().SetTimer(TimerHandle, this, &APlayerCharacter::DoubleTabGate, TapThreshold);
	bDoubleTabGate = true;
}

void APlayerCharacter::DoubleTabGate()
{
	// If gate is open
	if (bDoubleTabGate)
	{
		TabNumber = 0;
		PreviousTapNumber = 0;
	}
}

void APlayerCharacter::DoubleTabHandler()
{
	switch (TabNumber)
	{
		EMovementState NewMovement;
	default: case 0:
		// Completed
		SetMovementState_Implementation(PreviousMovementState, false, false);
		break;
	case 1:
		// Single Tap
		TabNumber = 0;
		if (bTapHeld)
		{
			// While aiming player can only walk 
			bIsAimed ? NewMovement = EMovementState::Walk : NewMovement = EMovementState::Run;
			// Only set movement state to run if the player is moving
			if (GetVelocity().Size() > 0.0f)
			{
				SetMovementState_Implementation(NewMovement, false, false);
			}
			else
			{
				MovementState = NewMovement;
			}
		}
		break;
	case 2:
		// Double Tap
		TabNumber = 0;
		if (bTapHeld)
		{
			// While aiming player can only walk 
			bIsAimed ? NewMovement = EMovementState::Walk : NewMovement = EMovementState::Sprint;
			// Only set movement state to sprint if the player is moving
			if (GetVelocity().Size() > 0.0f)
			{
				SetMovementState_Implementation(NewMovement, false, false);
			}
			else
			{
				MovementState = NewMovement;
			}
		}
		break;
	}
}

void APlayerCharacter::TryToToggleCrouch()
{
	if (bDoOnceCrouch)
	{
		bDoOnceCrouch = false;
		ToggleCrouch();
		
		FVector TargetRelativeLocation;
		FLatentActionInfo LatentInfo;
		LatentInfo.CallbackTarget = this;
		switch (MovementState)
		{
		case 0: case 1: case 2:
			// Walk, Run, Sprint
			TargetRelativeLocation = FVector(0.0f, 0.0f, 75.0f);
			break;
		case 3:
			// Crouch
			TargetRelativeLocation = FVector(0.0f, 0.0f, 50.0f);
			break;
		case 4:
			// Prone
			TargetRelativeLocation = FVector(0.0f, 0.0f, 25.0f);
			break;
		}
		UKismetSystemLibrary::MoveComponentTo(SpringArm, TargetRelativeLocation, FRotator::ZeroRotator, true, true, 0.2f, true, EMoveComponentAction::Type::Move, LatentInfo);
	}
}

void APlayerCharacter::ResetCrouchByDelay()
{
	FTimerHandle TimerHandle;
	GetWorld()->GetTimerManager().SetTimer(TimerHandle, this, &APlayerCharacter::ResetCrouch, 0.3f);
}

void APlayerCharacter::ResetCrouch()
{
	bDoOnceCrouch = true;
}

void APlayerCharacter::TryToStartAiming()
{
	const bool bAim = SetAimState(true);
	if (bAim)
	{
		UGameplayStatics::GetPlayerCameraManager(GetWorld(), 0)->ViewPitchMax = 50.0f;
		UGameplayStatics::GetPlayerCameraManager(GetWorld(), 0)->ViewPitchMin = -30.0f;

		if (AimFloatCurve)
		{
			FOnTimelineFloat TimeLineProgress;
			TimeLineProgress.BindUFunction(this, FName(TEXT("AimTimeLineUpdate")));
			AimTimeline->AddInterpFloat(AimFloatCurve, TimeLineProgress, FName(TEXT("LerpAlpha")));
			AimTimeline->SetTimelineLength(0.5);
			AimTimeline->Play();
			// FOnTimelineEvent TimelineFinishEvent;
			// TimelineFinishEvent.BindUFunction(this, FName("AimTimeLineFinished"));
			// AimTimeline->SetTimelineFinishedFunc(TimelineFinishEvent);
			Direction = ETimelineDirection::Forward;
		}
	}
}

void APlayerCharacter::ResetAim()
{
	SetAimState(false);
	if (HUD)
	{
		HUD->SetCrosshairVisibility_Implementation(ESlateVisibility::Hidden);
	}

	if (AimFloatCurve)
	{
		FOnTimelineEvent TimelineEvent;
		TimelineEvent.BindUFunction(this, FName("AimTimeLineFinished"));
		FOnTimelineFloat TimeLineProgress;
		TimeLineProgress.BindUFunction(this, FName("AimTimeLineUpdate"));
		// AimTimeline->AddInterpFloat(AimFloatCurve, TimeLineProgress, FName("LerpAlpha"));
		// AimTimeline->SetTimelineFinishedFunc(TimelineEvent);
		// AimTimeline->Reverse();
		Direction = ETimelineDirection::Backward;
	}
}

void APlayerCharacter::AimTimeLineUpdate(float Value)
{
	if (Direction == ETimelineDirection::Forward)
	{
		TPP->SetFieldOfView(FMath::Lerp(TPP->FieldOfView, 50.0f, Value));
		SpringArm->SocketOffset.Y = FMath::Lerp(SpringArm->SocketOffset.Y, 50.0f, Value);
		SpringArm->TargetArmLength = FMath::Lerp(SpringArm->TargetArmLength, 150.0f, Value);
	}
	else
	{
		TPP->SetFieldOfView(FMath::Lerp(90.0f, TPP->FieldOfView, Value));
		SpringArm->SocketOffset.Y = FMath::Lerp(0.0f, SpringArm->SocketOffset.Y, Value);
		SpringArm->TargetArmLength = FMath::Lerp(300.0f, SpringArm->TargetArmLength, Value);
	}
}

void APlayerCharacter::AimTimeLineFinished()
{
	if (Direction == ETimelineDirection::Forward)
	{
		if (HUD)
		{
			HUD->SetCrosshairVisibility_Implementation(ESlateVisibility::Visible);
		}
	}
	else
	{
		UGameplayStatics::GetPlayerCameraManager(GetWorld(), 0)->ViewPitchMax = 80.0f;
		UGameplayStatics::GetPlayerCameraManager(GetWorld(), 0)->ViewPitchMin = -80.0f;
	}
}

void APlayerCharacter::SwitchToNextWeapon()
{
	switch (CurrentHoldingWeapon)
	{
	case 0:
		// No Weapon
		if (PrimaryWeapon)
		{
			SwitchToPrimary();
		}
		else if (SecondaryWeapon)
		{
			SwitchToSecondary();
		}
		else if (SidearmWeapon)
		{
			SwitchToSidearm();
		}
		else
		{
			HolsterWeapon();
		}
		break;
	case 1:
		// Primary Weapon
		if (SecondaryWeapon)
		{
			SwitchToSecondary();
		}
		else if (SidearmWeapon)
		{
			SwitchToSidearm();
		}
		else
		{
			HolsterWeapon();
		}
		break;
	case 2:
		// Secondary Weapon
		if (SidearmWeapon)
		{
			SwitchToSidearm();
		}
		else
		{
			HolsterWeapon();
		}
		break;
	case 3:
		// Sidearm Weapon
		HolsterWeapon();
		break;
	}
}

void APlayerCharacter::SwitchToPreviousWeapon()
{
	switch (CurrentHoldingWeapon)
	{
	case 0:
		// No Weapon
		if (SidearmWeapon)
		{
			SwitchToSidearm();
		}
		else
		{
			HolsterWeapon();
		}
		break;
	case 1:
		// Primary Weapon
		HolsterWeapon();
		break;
	case 2:
		// Secondary Weapon
		if (SecondaryWeapon)
		{
			SwitchToPrimary();
		}
		else if (PrimaryWeapon)
		{
			SwitchToSecondary();
		}
		else if (SidearmWeapon)
		{
			
		}
		break;
	case 3:
		// Sidearm Weapon
		break;
	}
}

void APlayerCharacter::AddToForwardMovement(float AxisValue)
{
	// Zero out pitch and roll, only move on plane
	const FRotator NewRotation = FRotator(0.0f, GetControlRotation().Yaw, 0.0f);
	AddMovementInput(UKismetMathLibrary::GetForwardVector(NewRotation), AxisValue);
}

void APlayerCharacter::AddToRightMovement(float AxisValue)
{
	// Zero out pitch and roll, only move on plane
	const FRotator NewRotation = FRotator(0.0f, GetControlRotation().Yaw, 0.0f);
	AddMovementInput(UKismetMathLibrary::GetRightVector(NewRotation), AxisValue);
}

void APlayerCharacter::GamepadAddToYaw(float AxisValue)
{
	const float InputValue = AxisValue * BaseTurnRate * GetWorld()->GetDeltaSeconds();
	AddControllerYawInput(InputValue);
}

void APlayerCharacter::GamepadAddToPitch(float AxisValue)
{
	const float InputValue = AxisValue * BaseLookUpRate * GetWorld()->GetDeltaSeconds();
	AddControllerPitchInput(InputValue);
}

void APlayerCharacter::SetHealthState_Implementation(EHealthState HealthState)
{
	switch (HealthState)
	{
	case 0: case 1: case 2: case 3:
		// Full, Low, Recovery started, Recovery stopped
		break;
	case 4:
		// Death
		if(HUD)
		{
			HUD->SetUIVisibility(ESlateVisibility::Hidden);
		}
		break;
	}
	Super::SetHealthState_Implementation(HealthState);
}