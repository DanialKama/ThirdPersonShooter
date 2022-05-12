// Copyright 2022 Danial Kamali. All Rights Reserved.

#include "Characters/PlayerCharacter.h"
#include "Components/HealthComponent.h"
#include "Components/SlateWrapperTypes.h"
#include "Core/ShooterGameModeBase.h"
#include "Core/ShooterPlayerController.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/Classes/Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "UI/ShooterHUD.h"

APlayerCharacter::APlayerCharacter()
{
	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("Camera Boom"));
	SpringArm->SetupAttachment(GetRootComponent());
	SpringArm->SetComponentTickEnabled(false);
	SpringArm->SetRelativeLocation(FVector(0.0f, 0.0f, 75.0f));
	SpringArm->ProbeSize = 10.0;
	SpringArm->bUsePawnControlRotation = true;
	SpringArm->bEnableCameraLag = true;
	
	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Third Person Perspective"));
	Camera->SetupAttachment(SpringArm);
	Camera->SetComponentTickEnabled(false);
	
	AimTimeline = CreateDefaultSubobject<UTimelineComponent>(TEXT("Aim Timeline"));
	
	// Initialize variables
	BaseLookUpRate = 45.0f;
	BaseTurnRate = 45.0f;
	TapThreshold = 0.2f;
	PreviousTapNumber = TabNumber = 0;
	Direction = ETimelineDirection::Forward;
	bDoubleTabGate = false;
	bDoOnceCrouch = true;
	bTapHeld = false;
	
}

void APlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	// Set up gameplay key bindings
	check(PlayerInputComponent);
	// Input Actions
	InputComponent->BindAction("Jump", IE_Pressed, this, &ABaseCharacter::StartJump);
	InputComponent->BindAction("Jump", IE_Released, this, &APlayerCharacter::StopJumping);
		
	InputComponent->BindAction("Sprint", IE_Pressed, this, &APlayerCharacter::StartSprinting);
	InputComponent->BindAction("Sprint", IE_Released, this, &APlayerCharacter::StopSprinting);
		
	InputComponent->BindAction("Crouch", IE_Pressed, this, &APlayerCharacter::TryToToggleCrouch);
	InputComponent->BindAction("Crouch", IE_Released, this, &APlayerCharacter::ResetCrouchByDelay);
		
	InputComponent->BindAction("Aim", IE_Pressed, this, &APlayerCharacter::TryToStartAiming);
	InputComponent->BindAction("Aim", IE_Released, this, &APlayerCharacter::ResetAim);
		
	InputComponent->BindAction("FireWeapon", IE_Pressed, this, &ABaseCharacter::StartFireWeapon);
	InputComponent->BindAction("FireWeapon", IE_Released, this, &ABaseCharacter::StopFireWeapon);
		
	InputComponent->BindAction("Reload", IE_Pressed, this, &ABaseCharacter::ReloadWeapon);
		
	InputComponent->BindAction("SwitchToPrimary", IE_Pressed, this, &ABaseCharacter::SwitchToPrimary);
	InputComponent->BindAction("SwitchToSecondary", IE_Pressed, this, &ABaseCharacter::SwitchToSecondary);
	InputComponent->BindAction("SwitchToSidearm", IE_Pressed, this, &ABaseCharacter::SwitchToSidearm);
	InputComponent->BindAction("SwitchToNext", IE_Pressed, this, &APlayerCharacter::SwitchToNextWeapon);
	InputComponent->BindAction("SwitchToPrevious", IE_Pressed, this, &APlayerCharacter::SwitchToPreviousWeapon);

	InputComponent->BindAction("MeleeAttack", IE_Pressed, this, &ABaseCharacter::MeleeAttack);
		
	InputComponent->BindAction("DropItem", IE_Pressed, this, &ABaseCharacter::DropCurrentObject);
		
	// Input Axis
	InputComponent->BindAxis("MoveForward", this, &APlayerCharacter::AddToForwardMovement);
	InputComponent->BindAxis("MoveRight", this, &APlayerCharacter::AddToRightMovement);
		
	InputComponent->BindAxis("Turn", this, &APlayerCharacter::AddControllerYawInput);
	InputComponent->BindAxis("TurnRate", this, &APlayerCharacter::GamepadAddToYaw);

	InputComponent->BindAxis("LookUp", this, &APlayerCharacter::AddControllerPitchInput);
	InputComponent->BindAxis("LookUpRate", this, &APlayerCharacter::GamepadAddToPitch);
}

void APlayerCharacter::BeginPlay()
{
	Super::BeginPlay();

	ChildCameraComponent = Camera;
	UGameplayStatics::GetPlayerCameraManager(GetWorld(), 0)->ViewPitchMin = -80.0f;
	UGameplayStatics::GetPlayerCameraManager(GetWorld(), 0)->ViewPitchMax = 50.0f;

	// Try to get the player controller in the next frame to access a valid reference
	FTimerDelegate TimerDelegate;
	TimerDelegate.BindUObject(this, &APlayerCharacter::NextFrameBeginPlay);
	GetWorld()->GetTimerManager().SetTimerForNextTick(TimerDelegate);

	if (AimFloatCurve)
	{
		FOnTimelineFloat AimTimeLineProgress{};
		AimTimeLineProgress.BindUFunction(this, FName("AimTimeLineUpdate"));
		AimTimeline->AddInterpFloat(AimFloatCurve, AimTimeLineProgress, FName("Alpha"));
		FOnTimelineEvent AimTimelineFinishEvent{};
		AimTimelineFinishEvent.BindUFunction(this, FName("AimTimeLineFinished"));
		AimTimeline->SetTimelineFinishedFunc(AimTimelineFinishEvent);
	}
}

void APlayerCharacter::NextFrameBeginPlay()
{
	PlayerController = Cast<AShooterPlayerController>(GetController());
	HUDRef = Cast<AShooterHUD>(PlayerController->GetHUD());
	if (HUDRef)
	{
		HUDRef->SetHealth(GetHealthComponent()->DefaultHealth / GetHealthComponent()->MaxHealth);
		HUDRef->SetUIVisibility(ESlateVisibility::Visible);
	}
}

void APlayerCharacter::StartSprinting()
{
	bTapHeld = true;
	TabNumber = FMath::Clamp(TabNumber + 1, 1, 2);
	PreviousTapNumber = TabNumber;
	GetWorld()->GetTimerManager().ClearTimer(StartSprintingTimer);
	GetWorld()->GetTimerManager().SetTimer(StartSprintingTimer, this, &APlayerCharacter::DoubleTabHandler, TapThreshold);
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
		PreviousTapNumber = TabNumber = 0;
		DoubleTabHandler();
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
		
		FLatentActionInfo LatentInfo;
		LatentInfo.CallbackTarget = this;
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
	// If the result is true
	if (SetAimState(true))
	{
		UGameplayStatics::GetPlayerCameraManager(GetWorld(), 0)->ViewPitchMin = -35.0f;
		UGameplayStatics::GetPlayerCameraManager(GetWorld(), 0)->ViewPitchMax = 55.0f;

		AimTimeline->Play();
		Direction = ETimelineDirection::Forward;
	}
}

void APlayerCharacter::ResetAim()
{
	SetAimState(false);
	if (HUDRef)
	{
		HUDRef->SetCrosshairVisibility(ESlateVisibility::Hidden);
	}
	
	AimTimeline->Reverse();
	Direction = ETimelineDirection::Backward;
}

void APlayerCharacter::AimTimeLineUpdate(float Value)
{
	if (Direction == ETimelineDirection::Forward)
	{
		Camera->SetFieldOfView(FMath::Lerp(Camera->FieldOfView, 50.0f, Value));
		SpringArm->SocketOffset.Y = FMath::Lerp(SpringArm->SocketOffset.Y, 50.0f, Value);
		SpringArm->TargetArmLength = FMath::Lerp(SpringArm->TargetArmLength, 150.0f, Value);
	}
	else
	{
		Camera->SetFieldOfView(FMath::Lerp(90.0f, Camera->FieldOfView, Value));
		SpringArm->SocketOffset.Y = FMath::Lerp(0.0f, SpringArm->SocketOffset.Y, Value);
		SpringArm->TargetArmLength = FMath::Lerp(300.0f, SpringArm->TargetArmLength, Value);
	}
}

void APlayerCharacter::AimTimeLineFinished()
{
	if (Direction == ETimelineDirection::Forward)
	{
		HUDRef->SetCrosshairVisibility(ESlateVisibility::Visible);
	}
	else
	{
		UGameplayStatics::GetPlayerCameraManager(GetWorld(), 0)->ViewPitchMin = -80.0f;
		UGameplayStatics::GetPlayerCameraManager(GetWorld(), 0)->ViewPitchMax = 50.0f;
	}
}

void APlayerCharacter::SwitchToNextWeapon()
{
	// Order of switch to next weapon: Holster -> Primary -> Secondary -> Sidearm -> Holster
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
	// Order of switching to previous weapon: Holster -> Sidearm -> Secondary -> Primary -> Holster
	switch (CurrentHoldingWeapon)
	{
	case 0:
		if (SidearmWeapon)
		{
			SwitchToSidearm();
		}
		else if (SecondaryWeapon)
		{
			SwitchToSecondary();
		}
		else if (PrimaryWeapon)
		{
			SwitchToPrimary();
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
		if (PrimaryWeapon)
		{
			SwitchToPrimary();
		}
		else
		{
			HolsterWeapon();
		}
		break;
	case 3:
		// Sidearm Weapon
		if (SecondaryWeapon)
		{
			SwitchToSecondary();
		}
		else if (PrimaryWeapon)
		{
			SwitchToPrimary();
		}
		else
		{
			HolsterWeapon();
		}
		break;
	}
}

void APlayerCharacter::SetCurrentWeapon(APickupWeapon* NewCurrentWeapon, EWeaponToDo WeaponSlot)
{
	Super::SetCurrentWeapon(NewCurrentWeapon, WeaponSlot);
	
	// Hide ammo info on player UI widget when the player holding no weapon
	if (CurrentHoldingWeapon == EWeaponToDo::NoWeapon)
	{
		HUDRef->SetAmmoInfoVisibility(ESlateVisibility::Hidden);
		HUDRef->ToggleCommandMessage(FText::FromString(""), ESlateVisibility::Hidden, false);
	}
	else
	{
		HUDRef->SetAmmoInfoVisibility(ESlateVisibility::Visible);
	}
}

void APlayerCharacter::AddToForwardMovement(float AxisValue)
{
	if (Controller && AxisValue != 0.0f)
	{
		// Zero out pitch and roll, only move on plane, find out which way is forward
		const FRotator YawRotation(0, Controller->GetControlRotation().Yaw, 0);
		// Get forward vector
		const FVector NewDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		AddMovementInput(NewDirection, AxisValue);
	}
}

void APlayerCharacter::AddToRightMovement(float AxisValue)
{
	if (Controller && AxisValue != 0.0f)
	{
		// Zero out pitch and roll, only move on plane, find out which way is right
		const FRotator YawRotation(0, Controller->GetControlRotation().Yaw, 0);
		// Get right vector 
		const FVector NewDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
		// Add movement in that direction
		AddMovementInput(NewDirection, AxisValue);
	}
}

void APlayerCharacter::GamepadAddToYaw(float AxisValue)
{
	// Calculate delta for this frame from the Axis Value information
	AddControllerYawInput(AxisValue * BaseTurnRate * GetWorld()->GetDeltaSeconds());
}

void APlayerCharacter::GamepadAddToPitch(float AxisValue)
{
	// Calculate delta for this frame from the Axis Value information
	AddControllerPitchInput(AxisValue * BaseLookUpRate * GetWorld()->GetDeltaSeconds());
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
		if (HUDRef)
		{
			HUDRef->SetUIVisibility(ESlateVisibility::Hidden);
		}
		break;
	}
	
	Super::SetHealthState_Implementation(HealthState);
}

void APlayerCharacter::SetHealthLevel_Implementation(float Health)
{
	if (HUDRef)
	{
		HUDRef->SetHealth(Health);
	}	
}

void APlayerCharacter::SetStaminaLevel_Implementation(float Stamina, bool bIsFull)
{
	Super::SetStaminaLevel_Implementation(Stamina, bIsFull);

	if (HUDRef)
	{
		HUDRef->SetStamina(Stamina);
	}
}

void APlayerCharacter::Destroyed()
{
	// Get player controller reference before destroying the player
	AController* ControllerRef = GetController();
	
	Super::Destroyed();

	// Get the World and GameMode in the world to invoke its restart player function.
	if (const UWorld* World = GetWorld())
	{
		if (const AShooterGameModeBase* GameMode = Cast<AShooterGameModeBase>(World->GetAuthGameMode()))
		{
			GameMode->OnPlayerDied.Broadcast(ControllerRef);
		}
	}
}

void APlayerCharacter::AddRecoil_Implementation(const FRotator RotationIntensity, const float ControlTime, const float CrosshairRecoil, const float ControllerPitch)
{
	Super::AddRecoil_Implementation(RotationIntensity, ControlTime, CrosshairRecoil, ControllerPitch);
	
	AddControllerPitchInput(ControllerPitch);
	HUDRef->AddRecoil(CrosshairRecoil, ControlTime);
}

bool APlayerCharacter::IsPlayer_Implementation()
{
	return true;	
}