// Copyright 2022-2023 Danial Kamali. All Rights Reserved.

#include "BaseCharacter.h"

#include "Actors/Interactable/PickupAmmo.h"
#include "Actors/Interactable/PickupWeapon.h"
#include "Actors/NonInteractive/Magazine.h"
#include "AIModule/Classes/AIController.h"
#include "Animation/AnimInstance_Shooter.h"
#include "Components/AmmoComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/HealthComponent.h"
#include "Components/StaminaComponent.h"
#include "Components/TimelineComponent.h"
#include "Core/Interfaces/CommonInterface.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Perception/AIPerceptionStimuliSourceComponent.h"
#include "Perception/AISense_Sight.h"

ABaseCharacter::ABaseCharacter()
{
	PrimaryActorTick.bCanEverTick = true;
	bUseControllerRotationYaw = false;
	
	GetMesh()->SetRelativeRotation(FRotator(0.0f, 270.0f, 0.0f));
	// TODO: Why?
	GetMesh()->SetGenerateOverlapEvents(true);

	// TODO: Change the values in the blueprint
	GetCharacterMovement()->BrakingFriction = 0.1f;
	GetCharacterMovement()->SetCrouchedHalfHeight(65.0f);
	GetCharacterMovement()->bUseSeparateBrakingFriction = true;
	GetCharacterMovement()->MaxWalkSpeed = 150.0f;
	GetCharacterMovement()->MaxWalkSpeedCrouched = 150.0f;
	GetCharacterMovement()->bCanWalkOffLedgesWhenCrouching = true;
	GetCharacterMovement()->JumpZVelocity = 300.0f;
	GetCharacterMovement()->AirControl = 0.2f;
	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->PushForceFactor = 1000.0f;
	GetCharacterMovement()->NavAgentProps.AgentRadius = 42.0f;
	GetCharacterMovement()->NavAgentProps.AgentHeight = 192.0f;
	GetCharacterMovement()->NavAgentProps.bCanCrouch = true;
	WalkSpeed = GetCharacterMovement()->MaxWalkSpeed;
	WalkJumpZVelocity = GetCharacterMovement()->JumpZVelocity;
	
	HealthComponent = CreateDefaultSubobject<UHealthComponent>(TEXT("Health Component"));
	
	StaminaComponent = CreateDefaultSubobject<UStaminaComponent>(TEXT("Stamina Component"));
	
	StimuliSource = CreateDefaultSubobject<UAIPerceptionStimuliSourceComponent>(TEXT("Stimuli Source"));
	// TODO: Move it to the BeginPlay
	StimuliSource->RegisterForSense(UAISense_Sight::StaticClass());
	
	DeathTimeline = CreateDefaultSubobject<UTimelineComponent>(TEXT("Death Timeline"));
	
	// Initialize variables
	bDoOnceStopped = true;
	bDoOnceMoving = true;
	bDoOnceReload = true;
	bCanHolster = false;
	bCanReload = true;
	bIsAiming = false;
	bIsAlive = true;
}

void ABaseCharacter::BeginPlay()
{
	Super::BeginPlay();
	
	CharacterTagContainer.AddTag(TeamTag);

	// Create material instances for every material on mesh, mainly used for death dither effect
	TArray<UMaterialInterface*> Materials = GetMesh()->GetMaterials();
	const uint8 Length = Materials.Num();
	for (uint8 i = 0; i < Length; ++i)
	{
		MaterialInstances.Add(GetMesh()->CreateDynamicMaterialInstance(i, Materials[i]));
	}
}

void ABaseCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	// Check if character stopped or moving
	if (GetVelocity().Size() == 0.0f)
	{
		if (bDoOnceStopped)
		{
			GetWorld()->GetTimerManager().SetTimer(IdleTimer, this, &ABaseCharacter::PlayIdleAnimation, FMath::FRandRange(45.0f, 60.0f), true);
			bDoOnceStopped = false;
			bDoOnceMoving = true;
		}
	}
	else if (bDoOnceMoving)
	{
		GetWorld()->GetTimerManager().ClearTimer(IdleTimer);
		StopAnimMontage();
		bCanHolster = false;
		bDoOnceMoving = false;
		bDoOnceStopped = true;
	}

	// Uncrouch if the character is falling
	if (bIsCrouched && GetCharacterMovement()->bWantsToCrouch && GetCharacterMovement()->MovementMode == MOVE_Falling)
	{
		UnCrouch();
	}
}

void ABaseCharacter::SetMovementState(const EMovementState NewState)
{
	float NewWalkSpeed;
	float NewJumpZVelocity;
	
	switch (NewState)
	{
		case EMovementState::Run:
			NewWalkSpeed = RunningSpeed;
			NewJumpZVelocity = RunningJumpZVelocity;
			break;
		case EMovementState::Sprint:
			NewWalkSpeed = SprintSpeed;
			NewJumpZVelocity = SprintJumpZVelocity;
			break;
		default:
			NewWalkSpeed = WalkSpeed;
			NewJumpZVelocity = WalkJumpZVelocity;
	}
	
	SetMovementState(NewWalkSpeed, NewJumpZVelocity);
}

void ABaseCharacter::SetMovementState(const float NewWalkSpeed, const float NewJumpZVelocity)
{
	if (NewWalkSpeed > WalkSpeed)
	{
		if (StaminaComponent->CurrentStamina <= 0.0f)
		{
			return;
		}
		
		UnCrouch();
	}
	
	GetCharacterMovement()->MaxWalkSpeed = NewWalkSpeed;
	GetCharacterMovement()->JumpZVelocity = NewJumpZVelocity;
	
	if (NewWalkSpeed == WalkSpeed)
	{
		StaminaComponent->StopStaminaDrain();
	}
	else if (NewWalkSpeed == RunningSpeed)
	{
		StaminaComponent->StartRunning();
	}
	else
	{
		StaminaComponent->StartSprinting();
	}
}

void ABaseCharacter::SetPickup_Implementation(const EItemType NewPickupType, APickup* NewPickup)
{
	PickupType = NewPickupType;
	Pickup = NewPickup;
}

// TODO: Use a line trace to interact
void ABaseCharacter::Interact_Implementation()
{
	if (Pickup == nullptr)
	{
		return;
	}
	
	switch (PickupType)
	{
		case EItemType::Weapon:
			PickupWeapon(Pickup);
			break;
		case EItemType::Ammo:
			PickupAmmo(Pickup);
			break;
		case EItemType::Health:
			// TODO
			break;
	}
}

void ABaseCharacter::PickupWeapon(APickup* NewWeapon)
{
	APickupWeapon* Weapon = Cast<APickupWeapon>(NewWeapon);
	if (Weapon == nullptr)
	{
		return;
	}
	
	switch (Weapon->WeaponInfo.WeaponType)
	{
		case EWeaponType::Pistol: case EWeaponType::SMG:
			if (SidearmWeapon)
			{
				DropWeapon(EWeaponToDo::SidearmWeapon);
			}
			AddWeapon(Weapon, EWeaponToDo::SidearmWeapon);
			break;
		default:
			if (PrimaryWeapon)
			{
				if (SecondaryWeapon)
				{
					// if character currently holding the primary weapon or no weapon then add the new weapon to the primary slot
					if (CurrentWeapon == nullptr || CurrentWeapon == PrimaryWeapon)
					{
						DropWeapon(EWeaponToDo::PrimaryWeapon);
						AddWeapon(Weapon, EWeaponToDo::PrimaryWeapon);
					}
					// if character currently holding the secondary weapon then add the new weapon to the secondary slot
					else
					{
						DropWeapon(EWeaponToDo::SecondaryWeapon);
						AddWeapon(Weapon, EWeaponToDo::SecondaryWeapon);
					}
				}
				// If the secondary weapon slot is empty then add it there
				else
				{
					AddWeapon(Weapon, EWeaponToDo::SecondaryWeapon);
				}
			}
			// If the primary weapon slot is empty then add it there
			else
			{
				AddWeapon(Weapon, EWeaponToDo::PrimaryWeapon);
			}
	}
}

void ABaseCharacter::AddWeapon(APickupWeapon* WeaponToEquip, const EWeaponToDo TargetSlot)
{
	APickupWeapon* NewWeapon;
	// If the controller is AI do not spawn weapon (AI by default spawn weapon and does not pick up)
	if (Cast<AAIController>(GetController()))
	{
		NewWeapon = WeaponToEquip;
	}
	else
	{
		NewWeapon = SpawnAndReplaceWeapon(WeaponToEquip);
	}

	if (NewWeapon == nullptr)
	{
		return;
	}
	
	// If weapon owner is the player then weapon use camera reference for line trace
	NewWeapon->CameraComponent = ChildCameraComponent;
	NewWeapon->SetInstigator(this);
	NewWeapon->SetOwner(this);
	NewWeapon->SetPickUpState(EPickupState::PickUp);
	
	if (CurrentWeapon)
	{
		NewWeapon->LowerWeapon();
		switch (TargetSlot)
		{
		case EWeaponToDo::PrimaryWeapon:
			AttachWeapon(NewWeapon, EWeaponToDo::PrimaryWeapon);
			PrimaryWeapon = NewWeapon;
			break;
		case EWeaponToDo::SecondaryWeapon:
			AttachWeapon(NewWeapon, EWeaponToDo::SecondaryWeapon);
			SecondaryWeapon = NewWeapon;
			break;
		case EWeaponToDo::SidearmWeapon:
			AttachWeapon(NewWeapon, EWeaponToDo::SidearmWeapon);
			SidearmWeapon = NewWeapon;
			break;
		default:
			checkNoEntry();
		}
	}
	else
	{
		NewWeapon->RaiseWeapon();
		const FAttachmentTransformRules AttachmentTransformRules = FAttachmentTransformRules(EAttachmentRule::SnapToTarget, EAttachmentRule::SnapToTarget, EAttachmentRule::KeepWorld, true);
		NewWeapon->AttachToComponent(GetMesh(), AttachmentTransformRules, FName("RightHandHoldSocket"));

		// Check if the new weapon is a sidearm then replace it with the old sidearm
		const EWeaponType NewWeaponType = NewWeapon->WeaponInfo.WeaponType;
		if (NewWeaponType == EWeaponType::Pistol || NewWeaponType == EWeaponType::SMG)
		{
			SidearmWeapon = NewWeapon;
			SetCurrentWeapon(SidearmWeapon);
		}
		// If the primary weapon is valid then the secondary weapon replace with the new weapon
		else if (PrimaryWeapon)
		{
			SecondaryWeapon = NewWeapon;
			SetCurrentWeapon(SecondaryWeapon);
		}
		// The primary weapon is only replaced when it is invalid
		else
		{
			PrimaryWeapon = NewWeapon;
			SetCurrentWeapon(PrimaryWeapon);
		}

		SetArmedState(true);
	}

	UAnimInstance_Shooter* AnimInstance = Cast<UAnimInstance_Shooter>(GetMesh()->AnimScriptInstance);
	AnimInstance->Movement = NewWeapon->Movement;
	AnimInstance->MovementAim = NewWeapon->MovementAim;
	AnimInstance->MovementCrouch = NewWeapon->MovementCrouch;
	AnimInstance->MovementCrouchAim = NewWeapon->MovementCrouchAim;
}

APickupWeapon* ABaseCharacter::SpawnAndReplaceWeapon(APickupWeapon* WeaponToSpawn)
{
	FActorSpawnParameters ActorSpawnParameters;
	ActorSpawnParameters.Owner = this;
	ActorSpawnParameters.Instigator = GetInstigator();
	ActorSpawnParameters.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	APickupWeapon* NewWeapon = GetWorld()->SpawnActor<APickupWeapon>(WeaponToSpawn->GetClass(), GetActorLocation(), FRotator::ZeroRotator, ActorSpawnParameters);

	// Transfer important info to spawned weapon
	NewWeapon->AmmoComponent->SetAmmoInfo(WeaponToSpawn->AmmoComponent->MaxAmmo, WeaponToSpawn->AmmoComponent->CurrentAmmo,
		WeaponToSpawn->AmmoComponent->MagazineSize, WeaponToSpawn->AmmoComponent->CurrentMagazineAmmo);

	WeaponToSpawn->Destroy();
	return NewWeapon;
}

void ABaseCharacter::DropWeapon(EWeaponToDo WeaponToDrop)
{
	const FDetachmentTransformRules DetachmentTransformRules = FDetachmentTransformRules(EDetachmentRule::KeepWorld, false);
	switch (WeaponToDrop)
	{
		case EWeaponToDo::PrimaryWeapon:
			if (PrimaryWeapon)
			{
				PrimaryWeapon->LowerWeapon();
				PrimaryWeapon->DetachFromActor(DetachmentTransformRules);
				PrimaryWeapon->SetPickUpState(EPickupState::Drop);
				if (CurrentWeapon == PrimaryWeapon)
				{
					SetCurrentWeapon(nullptr);
					StopAnimMontage();
				}
				PrimaryWeapon = nullptr;
			}
			break;
		case EWeaponToDo::SecondaryWeapon:
			if (SecondaryWeapon)
			{
				SecondaryWeapon->LowerWeapon();
				SecondaryWeapon->DetachFromActor(DetachmentTransformRules);
				SecondaryWeapon->SetPickUpState(EPickupState::Drop);
				if (CurrentWeapon == SecondaryWeapon)
				{
					SetCurrentWeapon(nullptr);
					StopAnimMontage();
				}
				SecondaryWeapon = nullptr;
			}
			break;
		case EWeaponToDo::SidearmWeapon:
			if (SidearmWeapon)
			{
				SidearmWeapon->LowerWeapon();
				SidearmWeapon->DetachFromActor(DetachmentTransformRules);
				SidearmWeapon->SetPickUpState(EPickupState::Drop);
				if (CurrentWeapon == SidearmWeapon)
				{
					SetCurrentWeapon(nullptr);
					StopAnimMontage();
				}
				SidearmWeapon = nullptr;
			}
			break;
		default:
			break;
	}
}

void ABaseCharacter::SetCurrentWeapon(APickupWeapon* NewWeapon)
{
	if (NewWeapon == nullptr)
	{
		SetAimState(false);
		ResetAim();
	}

	CurrentWeapon = NewWeapon;
}

void ABaseCharacter::PickupAmmo(APickup* NewAmmo)
{
	if (APickupAmmo* Ammo = Cast<APickupAmmo>(NewAmmo))
	{
		switch (CanPickupAmmo_Implementation(Ammo->AmmoType))
		{
			case EWeaponToDo::PrimaryWeapon:
				PrimaryWeapon->AmmoComponent->AddAmmo(Ammo->Amount);
				break;
			case EWeaponToDo::SecondaryWeapon:
				SecondaryWeapon->AmmoComponent->AddAmmo(Ammo->Amount);
				break;
			case EWeaponToDo::SidearmWeapon:
				SidearmWeapon->AmmoComponent->AddAmmo(Ammo->Amount);
				break;
			default:
				break;
		}

		Ammo->SetPickUpState(EPickupState::Remove);
	}
}

EWeaponToDo ABaseCharacter::CanPickupAmmo_Implementation(int32 AmmoType)
{
	return EWeaponToDo::NoWeapon;

	// TODO: Redesign
	// if (CurrentWeapon)
	// {
	// 	if (CurrentWeapon->CanPickupAmmo() && GameplayTools::HasFlag(CurrentWeapon->WeaponInfo.AmmoType, AmmoType))
	// 	{
	// 		return CurrentHoldingWeapon;
	// 	}
	// }
	// if (PrimaryWeapon && PrimaryWeapon->CanPickupAmmo() && AmmoType & PrimaryWeaponSupportedAmmo)
	// {
	// 	return EWeaponToDo::PrimaryWeapon;
	// }
	// if (SecondaryWeapon && SecondaryWeapon->CanPickupAmmo() && AmmoType & SecondaryWeaponSupportedAmmo)
	// {
	// 	return EWeaponToDo::SecondaryWeapon;
	// }
	// if (SidearmWeapon && SidearmWeapon->CanPickupAmmo() && AmmoType & SidearmWeaponSupportedAmmo)
	// {
	// 	return EWeaponToDo::SidearmWeapon;
	// }
	//
	// return EWeaponToDo::NoWeapon;
}

void ABaseCharacter::StartFireWeapon()
{
	if (CurrentWeapon && bIsAiming)
	{
		CurrentWeapon->StartFireWeapon();
	}
}

void ABaseCharacter::StopFireWeapon()
{
	if (CurrentWeapon)
	{
		CurrentWeapon->StopFireWeapon();
	}
}

void ABaseCharacter::ReloadWeapon()
{
	if (bDoOnceReload && CurrentWeapon && CurrentWeapon->AmmoComponent->CanReload())
	{
		bDoOnceReload = false;

		UAnimMontage* MontageToPlay;
		if (bIsCrouched)
		{
			MontageToPlay = CurrentWeapon->CrouchReloadMontage;
		}
		else
		{
			MontageToPlay = CurrentWeapon->StandUpReloadMontage;
		}
		
		const float MontageLength = GetMesh()->AnimScriptInstance->Montage_Play(MontageToPlay);
		if (MontageLength > 0.0f)
		{
			GetMesh()->AnimScriptInstance->Montage_JumpToSection(FName("Start"), MontageToPlay);
			
			FOnMontageEnded EndedDelegate;
			EndedDelegate.BindUObject(this, &ABaseCharacter::ReloadWeaponMontageHandler);
			GetMesh()->AnimScriptInstance->Montage_SetEndDelegate(EndedDelegate, MontageToPlay);

			if (CurrentWeapon->GetClass()->ImplementsInterface(UCommonInterface::StaticClass()))
			{
				Execute_SetWeaponState(CurrentWeapon, EWeaponState::Idle);
			}
		}
		else
		{
			bDoOnceReload = true;
		}
	}
}

void ABaseCharacter::ReloadWeaponMontageHandler(UAnimMontage* AnimMontage, bool bInterrupted)
{
	bCanReload = true;
	if (bInterrupted)
	{
		if (CurrentWeapon)
		{
			CurrentWeapon->SetMagazineVisibility(true);
			if (CurrentWeapon->GetClass()->ImplementsInterface(UCommonInterface::StaticClass()))
			{
				Execute_SetWeaponState(CurrentWeapon, EWeaponState::Idle);
			}
		}
		if (Magazine)
		{
			Magazine->Destroy();
		}
		ResetReload();
	}
	else
	{
		if (APlayerController* LocalPC = Cast<APlayerController>(Controller))
		{
			EnableInput(LocalPC);
			StopAnimMontage();
			ResetReload();
		}
	}
}

void ABaseCharacter::SetReloadNotify(const EReloadState ReloadState)
{
	if (CurrentWeapon)
	{
		const FAttachmentTransformRules AttachmentRules = FAttachmentTransformRules(EAttachmentRule::SnapToTarget, EAttachmentRule::SnapToTarget, EAttachmentRule::KeepWorld, true);

		switch (ReloadState)
		{
			case EReloadState::StartReload:
				bCanReload = false;
				break;
			case EReloadState::RemoveMag:
				SpawnMagazine(CurrentWeapon, false);
				CurrentWeapon->SetMagazineVisibility(false);
				Magazine->GetMesh()->AttachToComponent(GetMesh(), AttachmentRules, FName("LeftHandHoldSocket"));
				break;
			case EReloadState::DropMag:
				Magazine->GetMesh()->SetSimulatePhysics(true);
				break;
			case EReloadState::PickMag:
				SpawnMagazine(CurrentWeapon, true);
				CurrentWeapon->SetMagazineVisibility(false);
				Magazine->GetMesh()->AttachToComponent(GetMesh(), AttachmentRules, FName("LeftHandHoldSocket"));
				break;
			case EReloadState::InsertMag:
				bCanReload = true;
				Magazine->Destroy();
				CurrentWeapon->SetMagazineVisibility(true);
				break;
			case EReloadState::EndReload:
				if (CurrentWeapon && bCanReload)
				{
					CurrentWeapon->ReloadWeapon();
					if (CurrentWeapon->AmmoComponent->CurrentMagazineAmmo == CurrentWeapon->AmmoComponent->MagazineSize)
					{
						StopAnimMontage();
					}
				}
				break;
		}
	}
}

void ABaseCharacter::SpawnMagazine(const APickupWeapon* Weapon, bool bIsNew)
{
	if (IsValid(Weapon->WeaponDefaults.Magazine))
	{
		FTransform Transform;
		Transform.SetLocation(GetMesh()->GetSocketLocation(FName("LeftHandHoldSocket")));
		Magazine = GetWorld()->SpawnActorDeferred<AMagazine>(Weapon->WeaponDefaults.Magazine, Transform, this, GetInstigator(), ESpawnActorCollisionHandlingMethod::AlwaysSpawn);
		// If spawned mag is the used mag then change mesh based on current magazine ammo
		if (bIsNew == false && CurrentWeapon->AmmoComponent->CurrentMagazineAmmo <= 0)
		{
			Magazine->bIsEmpty = true;
		}
		UGameplayStatics::FinishSpawningActor(Magazine, Transform);
	}
}

void ABaseCharacter::ResetReload()
{
	bDoOnceReload = true;
}

void ABaseCharacter::HolsterWeapon()
{
	if (CurrentWeapon == nullptr)
	{
		SwitchIsEnded();
		return;
	}
	
	SetArmedState(false);
	bCanReload = false;
	CurrentWeapon->LowerWeapon();
	
	const float MontageLength = GetMesh()->AnimScriptInstance->Montage_Play(CurrentWeapon->HolsterWeaponMontage);
	if (MontageLength > 0.0f)
	{
		GetMesh()->AnimScriptInstance->Montage_JumpToSection(FName("Start"), CurrentWeapon->HolsterWeaponMontage);
		
		FOnMontageEnded EndedDelegate;
		EndedDelegate.BindUObject(this, &ABaseCharacter::HolsterWeaponMontageHandler);
		GetMesh()->AnimScriptInstance->Montage_SetEndDelegate(EndedDelegate, CurrentWeapon->HolsterWeaponMontage);
	}
}

void ABaseCharacter::HolsterWeaponMontageHandler(UAnimMontage* AnimMontage, bool bInterrupted)
{
	if (bInterrupted)
	{
		SwitchIsEnded();
		if (CurrentWeapon)
		{
			SetArmedState(true);
		}
		else
		{
			SetArmedState(false);
		}
	}
}

void ABaseCharacter::UpdateHolsterWeaponNotifyState(ENotifyState NotifyState)
{
	switch (NotifyState)
	{
		case ENotifyState::Begin:
			bCanReload = false;
			bCanHolster = true;
			break;
		case ENotifyState::End:
			if (bCanHolster)
			{
				// If the character is not trying to switch weapons then just holster the current weapon
				if (WeaponToGrab == EWeaponToDo::NoWeapon)
				{
					AttachWeapon(CurrentWeapon, CurrentWeapon->CurrentSocket);
					SetCurrentWeapon(nullptr);
					SetArmedState(false);
					bCanReload = true;
					return;
				}

				// If the character trying to switch weapons and neither of the weapons is not a sidearm
				// then attach the current weapon to behind the character and attach the target weapon to hand.
				if (WeaponToHolsterType != EWeaponType::Pistol && WeaponToHolsterType != EWeaponType::SMG && WeaponToSwitchType != EWeaponType::Pistol && WeaponToSwitchType != EWeaponType::SMG)
				{
					switch (WeaponToGrab)
					{
					case EWeaponToDo::PrimaryWeapon:
						SwitchWeaponHandler(PrimaryWeapon, EWeaponToDo::PrimaryWeapon, true);
						break;
					case EWeaponToDo::SecondaryWeapon:
						SwitchWeaponHandler(SecondaryWeapon, EWeaponToDo::SecondaryWeapon, true);
						break;
					case EWeaponToDo::SidearmWeapon:
						SwitchWeaponHandler(SidearmWeapon, EWeaponToDo::SidearmWeapon, true);
						break;
					default:
						// No Weapon = nothing to switch
						break;
					}
				}
				// Switching or holstering a sidearm weapon requires more steps to do
				// so if the character is trying to switch weapons and one of them is a sidearm
				// then attach the current weapon to its socket (behind or side) and start switching to the target weapon
				else
				{
					AttachWeapon(CurrentWeapon, CurrentWeapon->CurrentSocket);
					SetCurrentWeapon(nullptr);
					SetArmedState(false);
					
					switch (WeaponToGrab)
					{
					case 0:
						// No Weapon = nothing to switch
						break;
					case 1:
						// Primary Weapon
						SwitchToPrimary();
						break;
					case 2:
						// Secondary Weapon
						SwitchToSecondary();
						break;
					case 3:
						// Sidearm Weapon
						SwitchToSidearm();
						break;
					}
				}
				bCanReload = true;
			}
			break;
	}
}

void ABaseCharacter::SwitchToPrimary()
{
	if (PrimaryWeapon == nullptr)
	{
		return;
	}
	
	bCanReload = false;
	SetArmedState(false);

	// If currently holding no weapon then grab the primary weapon
	if (CurrentWeapon == nullptr)
	{
		if (GetMesh()->AnimScriptInstance->Montage_Play(PrimaryWeapon->GrabWeaponMontage) > 0.0f)
		{
			GrabbedWeapon = PrimaryWeapon;
			WeaponToGrab = EWeaponToDo::PrimaryWeapon;
			
			GetMesh()->AnimScriptInstance->Montage_JumpToSection(FName("Start"), PrimaryWeapon->GrabWeaponMontage);
		}
	}
	// If currently holding the primary weapon then holster it
	else if (CurrentWeapon == PrimaryWeapon)
	{
		HolsterWeapon();
	}
	// If currently holding the secondary or sidearm weapon then holster it and grab the primary weapon
	else
	{
		CurrentWeapon->LowerWeapon();
		GrabbedWeapon = PrimaryWeapon;

		if (GetMesh()->AnimScriptInstance->Montage_Play(CurrentWeapon->HolsterWeaponMontage) > 0.0f)
		{
			WeaponToGrab = EWeaponToDo::PrimaryWeapon;
			WeaponToHolsterType = CurrentWeapon->WeaponInfo.WeaponType;
			WeaponToSwitchType = PrimaryWeapon->WeaponInfo.WeaponType;
			
			GetMesh()->AnimScriptInstance->Montage_JumpToSection(FName("Start"), CurrentWeapon->HolsterWeaponMontage);
			
			FOnMontageEnded EndedDelegate;
			EndedDelegate.BindUObject(this, &ABaseCharacter::HolsterWeaponMontageHandler);
			GetMesh()->AnimScriptInstance->Montage_SetEndDelegate(EndedDelegate, CurrentWeapon->HolsterWeaponMontage);
		}
	}
}

void ABaseCharacter::SwitchToSecondary()
{
	if (SecondaryWeapon == nullptr)
	{
		return;
	}
	
	bCanReload = false;
	SetArmedState(false);

	// If currently holding no weapon then grab the secondary weapon
	if (CurrentWeapon == nullptr)
	{
		if (GetMesh()->AnimScriptInstance->Montage_Play(SecondaryWeapon->GrabWeaponMontage) > 0.0f)
		{
			GrabbedWeapon = SecondaryWeapon;
			WeaponToGrab = EWeaponToDo::SecondaryWeapon;
			
			GetMesh()->AnimScriptInstance->Montage_JumpToSection(FName("Start"), SecondaryWeapon->GrabWeaponMontage);
		}
	}
	// If currently holding the secondary weapon then holster it
	else if (CurrentWeapon == SecondaryWeapon)
	{
		HolsterWeapon();
	}
	// If currently holding the primary or sidearm weapon then holster it and grab the secondary weapon
	else
	{
		CurrentWeapon->LowerWeapon();
		GrabbedWeapon = SecondaryWeapon;
		
		if (GetMesh()->AnimScriptInstance->Montage_Play(CurrentWeapon->HolsterWeaponMontage) > 0.0f)
		{
			WeaponToGrab = EWeaponToDo::SecondaryWeapon;
			WeaponToHolsterType = CurrentWeapon->WeaponInfo.WeaponType;
			WeaponToSwitchType = SecondaryWeapon->WeaponInfo.WeaponType;
		
			GetMesh()->AnimScriptInstance->Montage_JumpToSection(FName("Start"), CurrentWeapon->HolsterWeaponMontage);
		
			FOnMontageEnded EndedDelegate;
			EndedDelegate.BindUObject(this, &ABaseCharacter::HolsterWeaponMontageHandler);
			GetMesh()->AnimScriptInstance->Montage_SetEndDelegate(EndedDelegate, CurrentWeapon->HolsterWeaponMontage);
		}
	}
}

void ABaseCharacter::SwitchToSidearm()
{
	if (SidearmWeapon == nullptr)
	{
		return;
	}

	bCanReload = false;
	SetArmedState(false);

	// If currently holding no weapon then grab the sidearm weapon
	if (CurrentWeapon == nullptr)
	{
		if (GetMesh()->AnimScriptInstance->Montage_Play(SidearmWeapon->GrabWeaponMontage) > 0.0f)
		{
			GrabbedWeapon = SidearmWeapon;
			WeaponToGrab = EWeaponToDo::SidearmWeapon;
			
			GetMesh()->AnimScriptInstance->Montage_JumpToSection(FName("Start"), SidearmWeapon->GrabWeaponMontage);
		}
	}
	// If currently holding the sidearm weapon then holster it
	else if (CurrentWeapon == SidearmWeapon)
	{
		HolsterWeapon();
	}
	// If currently holding the primary or secondary weapon then holster it and grab the sidearm weapon
	else
	{
		CurrentWeapon->LowerWeapon();
		if (GetMesh()->AnimScriptInstance->Montage_Play(CurrentWeapon->HolsterWeaponMontage) > 0.0f)
		{
			WeaponToGrab = EWeaponToDo::SidearmWeapon;
			WeaponToHolsterType = CurrentWeapon->WeaponInfo.WeaponType;
			WeaponToSwitchType = SidearmWeapon->WeaponInfo.WeaponType;
			
			GetMesh()->AnimScriptInstance->Montage_JumpToSection(FName("Start"), CurrentWeapon->HolsterWeaponMontage);
			
			FOnMontageEnded EndedDelegate;
			EndedDelegate.BindUObject(this, &ABaseCharacter::HolsterWeaponMontageHandler);
			GetMesh()->AnimScriptInstance->Montage_SetEndDelegate(EndedDelegate, CurrentWeapon->HolsterWeaponMontage);
		}
	}
}

void ABaseCharacter::SwitchWeaponHandler(APickupWeapon* WeaponToSwitch, const EWeaponToDo TargetWeapon, const bool bSwitchWeapon)
{
	if (WeaponToSwitch)
	{
		WeaponToSwitch->SkeletalMesh->SetSimulatePhysics(false);
		WeaponToSwitch->SkeletalMesh->SetCollisionProfileName(FName("NoCollision"), false);

		// If character try to switch, attach the current weapon to skeletal mesh socket and set target weapon as current weapon
		if (bSwitchWeapon)
		{
			AttachWeapon(CurrentWeapon, CurrentWeapon->CurrentSocket);
		}

		switch (TargetWeapon)
		{
			case EWeaponToDo::PrimaryWeapon:
				SetCurrentWeapon(WeaponToSwitch);
				break;
			case EWeaponToDo::SecondaryWeapon:
				SetCurrentWeapon(WeaponToSwitch);
				break;
			case EWeaponToDo::SidearmWeapon:
				SetCurrentWeapon(WeaponToSwitch);
				break;
			default:
				// No weapon to switch to
				break;
		}

		const FAttachmentTransformRules AttachmentTransformRules = FAttachmentTransformRules(EAttachmentRule::SnapToTarget, EAttachmentRule::SnapToTarget, EAttachmentRule::KeepWorld, true);
		WeaponToSwitch->AttachToComponent(GetMesh(), AttachmentTransformRules, FName("RightHandHoldSocket"));
		SetArmedState(true);
	}
	
	WeaponToGrab = EWeaponToDo::NoWeapon;
}

void ABaseCharacter::UpdateGrabWeaponNotifyState(const ENotifyState NotifyState)
{
	switch (NotifyState)
	{
		case ENotifyState::Begin:
			if (GrabbedWeapon)
			{
				SwitchWeaponHandler(GrabbedWeapon, WeaponToGrab, false);
				GrabbedWeapon->RaiseWeapon();
				
				if (GetMesh()->AnimScriptInstance->Montage_Play(GrabbedWeapon->GrabWeaponMontage) > 0.0f)
				{
					GetMesh()->AnimScriptInstance->Montage_JumpToSection(FName("Grab"), GrabbedWeapon->GrabWeaponMontage);
				}
				
				GrabbedWeapon = nullptr;
			}
		
			bCanReload = false;
			break;
		case ENotifyState::End:
			StopAnimMontage();
			SwitchIsEnded();
			bCanReload = true;
			break;
	}
}

void ABaseCharacter::AttachWeapon(APickupWeapon* WeaponToAttach, const EWeaponToDo TargetSocket) const
{
	WeaponToAttach->CurrentSocket = TargetSocket;
	
	const FAttachmentTransformRules AttachmentRules = FAttachmentTransformRules(EAttachmentRule::SnapToTarget,
		EAttachmentRule::SnapToTarget, EAttachmentRule::KeepWorld, true);
	
	switch (TargetSocket)
	{
	case EWeaponToDo::NoWeapon:
		// No weapon to attach
		checkNoEntry();
		break;
	case EWeaponToDo::PrimaryWeapon:
		WeaponToAttach->AttachToComponent(GetMesh(), AttachmentRules, FName("Weapon1Socket"));
		break;
	case EWeaponToDo::SecondaryWeapon:
		WeaponToAttach->AttachToComponent(GetMesh(), AttachmentRules, FName("Weapon2Socket"));
		break;
	case EWeaponToDo::SidearmWeapon:
		WeaponToAttach->AttachToComponent(GetMesh(), AttachmentRules, FName("Weapon3Socket"));
		break;
	}
}

void ABaseCharacter::AddRecoil_Implementation(const FRotator RotationIntensity, const float ControlTime, const float CrosshairRecoil, const float ControllerPitch)
{
	UAnimInstance_Shooter* AnimInstance = Cast<UAnimInstance_Shooter>(GetMesh()->AnimScriptInstance);
	AnimInstance->RecoilRotation = RotationIntensity;
	AnimInstance->RecoilControlTime = ControlTime;
	AnimInstance->RecoilAlpha = 1.0f;
}

void ABaseCharacter::SetArmedState(const bool bArmedState)
{
	if (bArmedState == false)
	{
		SetAimState(false);
	}

	// TODO: Update anim instance
	// if (bCharacterAnimationInterface)
	// {
	// 	if (CurrentWeapon)
	// 	{
	// 		ICharacterAnimationInterface::Execute_SetArmedState(AnimInstance, bArmedState, CurrentWeapon);
	// 	}
	// 	else
	// 	{
	// 		ICharacterAnimationInterface::Execute_SetArmedState(AnimInstance, false, CurrentWeapon);
	// 	}
	// }
}

bool ABaseCharacter::SetAimState(bool bNewState)
{
	bIsAiming = bNewState;
	
	if (bNewState && CurrentWeapon)
	{
		GetCharacterMovement()->bUseControllerDesiredRotation = true;
		GetCharacterMovement()->bOrientRotationToMovement = false;

		// Force the character to walk while aiming
		GetCharacterMovement()->MaxWalkSpeed = WalkSpeed;
		
		StopAnimMontage();
		return true;
	}
	
	GetCharacterMovement()->bUseControllerDesiredRotation = false;
	GetCharacterMovement()->bOrientRotationToMovement = true;
	StopFireWeapon();

	UAnimInstance_Shooter* AnimInstance = Cast<UAnimInstance_Shooter>(GetMesh()->AnimScriptInstance);
	AnimInstance->RecoilAlpha = 0.0f;
	
	return false;
}

void ABaseCharacter::DropCurrentObject()
{
	if (CurrentWeapon)
	{
		DropWeapon(CurrentWeapon->CurrentSocket);
		SetArmedState(false);
	}
}

void ABaseCharacter::MeleeAttack()
{
	UAnimMontage* MontageToPlay = MeleeAttackMontages[FMath::RandRange(0, MeleeAttackMontages.Num() - 1)];
	if (GetMesh()->AnimScriptInstance->Montage_Play(MontageToPlay) > 0.0f)
	{
		GetCharacterMovement()->DisableMovement();

		//TODO: Capsule trace
		
		FOnMontageEnded EndedDelegate;
		EndedDelegate.BindUObject(this, &ABaseCharacter::MeleeMontageHandler);
		GetMesh()->AnimScriptInstance->Montage_SetEndDelegate(EndedDelegate, MontageToPlay);
	}
}

void ABaseCharacter::MeleeMontageHandler(UAnimMontage* AnimMontage, bool bInterrupted)
{
	GetCharacterMovement()->SetMovementMode(MOVE_Walking);
}

// TODO: Improve melee attack
	// if (bIsAlive && OtherActor != this)
	// {
	// 	UGameplayStatics::ApplyDamage(OtherActor, MeleeDamage, GetInstigatorController(), this, nullptr);
	// }

void ABaseCharacter::SetHealthState_Implementation(EHealthState HealthState)
{
	switch (HealthState)
	{
		case EHealthState::Death:
			KillCharacter();
			break;
		default:
			break;
	}
}

void ABaseCharacter::SetStaminaLevel_Implementation(float Stamina, const bool bIsFull)
{
	if (HealthComponent)
	{
		if (bIsFull)
		{
			HealthComponent->StopHealthRecovery();
		}
		else
		{
			HealthComponent->StartHealthRecovery();
		}
	}
}

void ABaseCharacter::KillCharacter()
{
	DismembermentInitiate(HealthComponent->ShotOrigin, HealthComponent->HitBoneName);
	SetArmedState(false);
	bIsAlive = false;
	HealthComponent->Deactivate();
	StaminaComponent->Deactivate();
	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	
	// Don't play the death montage if character is falling 
	if (GetCharacterMovement()->IsFalling())
	{
		// TODO: Enable ragdoll
		DeathMontageHandler(nullptr, false);
	}
	else
	{
		UAnimMontage* MontageToPlay;
		
		// Select a random montage based on the current movement state
		if (bIsCrouched)
		{
			MontageToPlay = CrouchDeathMontages[FMath::RandRange(0, CrouchDeathMontages.Num() - 1)].LoadSynchronous();
		}
		else
		{
			MontageToPlay = StandUpDeathMontages[FMath::RandRange(0, StandUpDeathMontages.Num() - 1)].LoadSynchronous();
		}
		
		if (GetMesh()->AnimScriptInstance->Montage_Play(MontageToPlay) > 0.0f)
		{
			FOnMontageEnded EndedDelegate;
			EndedDelegate.BindUObject(this, &ABaseCharacter::DeathMontageHandler);
			GetMesh()->AnimScriptInstance->Montage_SetEndDelegate(EndedDelegate, MontageToPlay);
		}
	}
	
	GetCharacterMovement()->DisableMovement();
	StimuliSource->UnregisterFromSense(UAISense_Sight::StaticClass());
	StimuliSource->UnregisterFromPerceptionSystem();
	
	// If character is AI, detach it from controller
	if (Cast<AAIController>(GetController()))
	{
		DetachFromControllerPendingDestroy();
	}
}

void ABaseCharacter::DismembermentInitiate(FVector ShotOrigin, FName HitBone)
{
	// TODO: Redesign
	// if (HitBone != NAME_None && HitBone != "pelvis" && HitBone != "spine_01" && HitBone != "spine_02" && HitBone != "spine_03" && HitBone != "clavicle_l" && HitBone != "clavicle_r")
	// {
	// 	USkeletalMeshComponent* AmputatedLimb = nullptr;
	// 	if (HitBone == "head" || HitBone == "neck_01")
	// 	{
	// 		constexpr EPhysBodyOp PhysBodyOption = PBO_Term;
	// 		GetMesh()->HideBoneByName(FName("neck_01"), PhysBodyOption);
	// 		AmputatedLimb = SpawnBodyPart("Head", BodyParts.Head);
	// 	}
	// 	else if (HitBone == "Thigh_L" || GetMesh()->BoneIsChildOf(HitBone, FName("Thigh_L")))
	// 	{
	// 		AmputatedLimb = DismembermentLeftLeg(HitBone);
	// 	}
	// 	else if (HitBone == "Thigh_R" || GetMesh()->BoneIsChildOf(HitBone, FName("Thigh_R")))
	// 	{
	// 		AmputatedLimb = DismembermentRightLeg(HitBone);
	// 	}
	// 	else if (GetMesh()->BoneIsChildOf(HitBone, FName("clavicle_l")))
	// 	{
	// 		AmputatedLimb = DismembermentLeftHand(HitBone);
	// 		DropWeapon(CurrentHoldingWeapon);
	// 	}
	// 	else if (GetMesh()->BoneIsChildOf(HitBone, FName("clavicle_r")))
	// 	{
	// 		AmputatedLimb = DismembermentRightHand(HitBone);
	// 		DropWeapon(CurrentHoldingWeapon);
	// 	}
	// 	
	// 	if (AmputatedLimb)
	// 	{
	// 		AmputatedLimb->AddRadialImpulse(ShotOrigin, 400.0f, 500.0f, RIF_Linear, true);
	// 	}
	// }
}

USkeletalMeshComponent* ABaseCharacter::DismembermentLeftLeg(FName HitBone)
{
	USkeletalMeshComponent* NewBodyPart = nullptr;
	// constexpr EPhysBodyOp PhysBodyOption = PBO_Term;
	// if (HitBone == "Thigh_L" || HitBone == "thigh_twist_01_l")
	// {
	// 	if (GetMesh()->IsBoneHiddenByName(FName("calf_l")) && BodyParts.ThighLeft)
	// 	{
	// 		GetMesh()->HideBoneByName(FName("Thigh_L"), PhysBodyOption);
	// 		NewBodyPart = SpawnBodyPart("Left Thigh", BodyParts.ThighLeft);
	// 	}
	// 	else if (GetMesh()->IsBoneHiddenByName(FName("Foot_L")) && BodyParts.ThighAndCalfLeft)
	// 	{
	// 		GetMesh()->HideBoneByName(FName("Thigh_L"), PhysBodyOption);
	// 		NewBodyPart = SpawnBodyPart("Left Thigh and Calf", BodyParts.ThighAndCalfLeft);
	// 	}
	// 	else if (BodyParts.LegLeft)
	// 	{
	// 		GetMesh()->HideBoneByName(FName("Thigh_L"), PhysBodyOption);
	// 		NewBodyPart = SpawnBodyPart("Left Leg", BodyParts.LegLeft);
	// 	}
	// }
	// else if (HitBone == "calf_l" || HitBone == "calf_twist_01_l")
	// {
	// 	if (GetMesh()->IsBoneHiddenByName(FName("Foot_L")) && BodyParts.CalfLeft)
	// 	{
	// 		GetMesh()->HideBoneByName(FName("calf_l"), PhysBodyOption);
	// 		NewBodyPart = SpawnBodyPart("Left Calf", BodyParts.CalfLeft);
	// 	}
	// 	else if (BodyParts.CalfAndFootLeft)
	// 	{
	// 		GetMesh()->HideBoneByName(FName("calf_l"), PhysBodyOption);
	// 		NewBodyPart = SpawnBodyPart("Left Calf and Foot", BodyParts.CalfAndFootLeft);
	// 	}
	// }
	// else if (HitBone == "Foot_L" || HitBone == "ball_l" && BodyParts.FootLeft)
	// {
	// 	GetMesh()->HideBoneByName(FName("Foot_L"), PhysBodyOption);
	// 	NewBodyPart = SpawnBodyPart("Left Foot", BodyParts.FootLeft);
	// }
	return NewBodyPart;
}

USkeletalMeshComponent* ABaseCharacter::DismembermentRightLeg(FName HitBone)
{
	USkeletalMeshComponent* NewBodyPart = nullptr;
	// constexpr EPhysBodyOp PhysBodyOption = PBO_Term;
	// if (HitBone == "Thigh_R" || HitBone == "thigh_twist_01_r")
	// {
	// 	if (GetMesh()->IsBoneHiddenByName(FName("calf_r")) && BodyParts.ThighRight)
	// 	{
	// 		GetMesh()->HideBoneByName(FName("Thigh_R"), PhysBodyOption);
	// 		NewBodyPart = SpawnBodyPart("Right Thigh", BodyParts.ThighRight);
	// 	}
	// 	else if (GetMesh()->IsBoneHiddenByName(FName("Foot_R")) && BodyParts.ThighAndCalfRight)
	// 	{
	// 		GetMesh()->HideBoneByName(FName("Thigh_R"), PhysBodyOption);
	// 		NewBodyPart = SpawnBodyPart("Right Thigh and Calf", BodyParts.ThighAndCalfRight);
	// 	}
	// 	else if (BodyParts.LegRight)
	// 	{
	// 		GetMesh()->HideBoneByName(FName("Thigh_R"), PhysBodyOption);
	// 		NewBodyPart = SpawnBodyPart("Right Leg", BodyParts.LegRight);
	// 	}
	// }
	// else if (HitBone == "calf_r" || HitBone == "calf_twist_01_r")
	// {
	// 	if (GetMesh()->IsBoneHiddenByName(FName("Foot_R")) && BodyParts.CalfRight)
	// 	{
	// 		GetMesh()->HideBoneByName(FName("calf_r"), PhysBodyOption);
	// 		NewBodyPart = SpawnBodyPart("Right Calf", BodyParts.CalfRight);
	// 	}
	// 	else if (BodyParts.CalfAndFootRight)
	// 	{
	// 		GetMesh()->HideBoneByName(FName("calf_r"), PhysBodyOption);
	// 		NewBodyPart = SpawnBodyPart("Right Calf and Foot", BodyParts.CalfAndFootRight);
	// 	}
	// }
	// else if (HitBone == "Foot_R" || HitBone == "ball_r" && BodyParts.FootRight)
	// {
	// 	GetMesh()->HideBoneByName(FName("calf_r"), PhysBodyOption);
	// 	NewBodyPart = SpawnBodyPart("Right Foot", BodyParts.FootRight);
	// }
	return NewBodyPart;
}

USkeletalMeshComponent* ABaseCharacter::DismembermentLeftHand(FName HitBone)
{
	USkeletalMeshComponent* NewBodyPart = nullptr;
	// constexpr EPhysBodyOp PhysBodyOption = PBO_Term;
	// if (HitBone == "UpperArm_L")
	// {
	// 	if (GetMesh()->IsBoneHiddenByName(FName("lowerarm_l")) && BodyParts.UpperArmLeft)
	// 	{
	// 		GetMesh()->HideBoneByName(FName("UpperArm_L"), PhysBodyOption);
	// 		NewBodyPart = SpawnBodyPart("Left Upper Arm", BodyParts.UpperArmLeft);
	// 	}
	// 	else if (GetMesh()->IsBoneHiddenByName(FName("Hand_L")) && BodyParts.UpperArmAndLowerArmLeft)
	// 	{
	// 		GetMesh()->HideBoneByName(FName("UpperArm_L"), PhysBodyOption);
	// 		NewBodyPart = SpawnBodyPart("Left Upper Arm and Lower Arm", BodyParts.UpperArmAndLowerArmLeft);
	// 	}
	// 	else if (BodyParts.ArmLeft)
	// 	{
	// 		GetMesh()->HideBoneByName(FName("UpperArm_L"), PhysBodyOption);
	// 		NewBodyPart = SpawnBodyPart("Left Arm", BodyParts.ArmLeft);
	// 	}
	// }
	// else if (HitBone == "lowerarm_l" || HitBone == "lowerarm_twist_01_l")
	// {
	// 	if (GetMesh()->IsBoneHiddenByName(FName("Hand_L")) && BodyParts.LowerArmLeft)
	// 	{
	// 		GetMesh()->HideBoneByName(FName("lowerarm_l"), PhysBodyOption);
	// 		NewBodyPart = SpawnBodyPart("Left Lower Arm", BodyParts.LowerArmLeft);
	// 	}
	// 	else if (BodyParts.LowerArmAndHandLeft)
	// 	{
	// 		GetMesh()->HideBoneByName(FName("lowerarm_l"), PhysBodyOption);
	// 		NewBodyPart = SpawnBodyPart("Left Lower Arm and Hand", BodyParts.LowerArmAndHandLeft);
	// 	}
	// }
	// else if (HitBone == "Hand_L" || GetMesh()->BoneIsChildOf(HitBone, FName("Hand_L")) && BodyParts.HandLeft)
	// {
	// 	GetMesh()->HideBoneByName(FName("Hand_L"), PhysBodyOption);
	// 	NewBodyPart = SpawnBodyPart("Left Hand", BodyParts.HandLeft);
	// }
	return NewBodyPart;
}

USkeletalMeshComponent* ABaseCharacter::DismembermentRightHand(FName HitBone)
{
	USkeletalMeshComponent* NewBodyPart = nullptr;
	// constexpr EPhysBodyOp PhysBodyOption = PBO_Term;
	// if (HitBone == "UpperArm_R")
	// {
	// 	if (GetMesh()->IsBoneHiddenByName(FName("lowerarm_r")) && BodyParts.UpperArmRight)
	// 	{
	// 		GetMesh()->HideBoneByName(FName("UpperArm_R"), PhysBodyOption);
	// 		NewBodyPart = SpawnBodyPart("Right Upper Arm", BodyParts.UpperArmRight);
	// 	}
	// 	else if (GetMesh()->IsBoneHiddenByName(FName("Hand_R")) && BodyParts.UpperArmAndLowerArmRight)
	// 	{
	// 		GetMesh()->HideBoneByName(FName("UpperArm_R"), PhysBodyOption);
	// 		NewBodyPart = SpawnBodyPart("Right Upper Arm and Lower Arm", BodyParts.UpperArmAndLowerArmRight);
	// 	}
	// 	else if (BodyParts.ArmRight)
	// 	{
	// 		GetMesh()->HideBoneByName(FName("UpperArm_R"), PhysBodyOption);
	// 		NewBodyPart = SpawnBodyPart("Right Arm", BodyParts.ArmRight);
	// 	}
	// }
	// else if (HitBone == "lowerarm_r" || HitBone == "lowerarm_twist_01_r")
	// {
	// 	if (GetMesh()->IsBoneHiddenByName(FName("Hand_R")) && BodyParts.LowerArmRight)
	// 	{
	// 		GetMesh()->HideBoneByName(FName("lowerarm_r"), PhysBodyOption);
	// 		NewBodyPart = SpawnBodyPart("Right Lower Arm", BodyParts.LowerArmRight);
	// 	}
	// 	else if (BodyParts.LowerArmAndHandRight)
	// 	{
	// 		GetMesh()->HideBoneByName(FName("lowerarm_r"), PhysBodyOption);
	// 		NewBodyPart = SpawnBodyPart("Right Lower Arm and Hand", BodyParts.LowerArmAndHandRight);
	// 	}
	// }
	// else if (HitBone == "Hand_R" || GetMesh()->BoneIsChildOf(HitBone, FName("Hand_R")) && BodyParts.HandRight)
	// {
	// 	GetMesh()->HideBoneByName(FName("Hand_R"), PhysBodyOption);
	// 	NewBodyPart = SpawnBodyPart("Right Hand", BodyParts.HandRight);
	// }
	return NewBodyPart;
}

USkeletalMeshComponent* ABaseCharacter::SpawnBodyPart(FName Name, USkeletalMesh* SkeletalMesh)
{
	USkeletalMeshComponent* NewBodyPart = NewObject<USkeletalMeshComponent>(this, USkeletalMeshComponent::StaticClass(), Name, RF_NoFlags);
	AddInstanceComponent(NewBodyPart);
	NewBodyPart->SetupAttachment(GetRootComponent());
	NewBodyPart->SetSkeletalMesh(SkeletalMesh);
	NewBodyPart->SetRelativeTransform(GetMesh()->GetRelativeTransform());
	NewBodyPart->SetCollisionEnabled(ECollisionEnabled::PhysicsOnly);
	NewBodyPart->SetCollisionObjectType(ECC_PhysicsBody);
	NewBodyPart->SetCollisionResponseToAllChannels(ECR_Ignore);
	NewBodyPart->SetCollisionResponseToChannel(ECC_WorldStatic, ECR_Block);
	NewBodyPart->SetSimulatePhysics(true);
	NewBodyPart->RegisterComponent();
	return NewBodyPart;
}

void ABaseCharacter::DeathMontageHandler(UAnimMontage* AnimMontage, bool bInterrupted)
{
	GetCharacterMovement()->DisableMovement();
	GetMesh()->SetConstraintProfileForAll(FName("Ragdoll"), true);
	GetMesh()->SetCollisionProfileName(FName("Ragdoll"));
	GetMesh()->SetSimulatePhysics(true);

	// TODO: Stop anim instance
	// if (bCharacterAnimationInterface)
	// {
	// 	ICharacterAnimationInterface::Execute_SetCompletelyStopMoving(AnimInstance, true);
	// }
	
	GetMesh()->bPauseAnims = true;
	GetMesh()->SetCollisionResponseToChannel(ECC_PhysicsBody, ECR_Ignore);
	// Drop all weapons
	DropWeapon(EWeaponToDo::PrimaryWeapon);
	DropWeapon(EWeaponToDo::SecondaryWeapon);
	DropWeapon(EWeaponToDo::SidearmWeapon);
	// Timer to start fade effect
	FTimerHandle TimerHandle;
	GetWorld()->GetTimerManager().SetTimer(TimerHandle, this, &ABaseCharacter::StartDeathLifeSpan, DeathLifeSpan);
}

// TODO: Switch to a translucent material when fading
void ABaseCharacter::StartDeathLifeSpan()
{
	if (UCurveFloat* FloatCurve = FadeFloatCurve.LoadSynchronous())
	{
		FOnTimelineFloat DeathTimeLineProgress{};
		DeathTimeLineProgress.BindUFunction(this, FName("DeathTimeLineUpdate"));
		DeathTimeline->AddInterpFloat(FloatCurve, DeathTimeLineProgress, FName("Fade"));
		FOnTimelineEvent DeathTimelineEvent{};
		DeathTimelineEvent.BindUFunction(this, FName("StartDestroy"));
		DeathTimeline->SetTimelineFinishedFunc(DeathTimelineEvent);
		DeathTimeline->Play();
	}
}

void ABaseCharacter::DeathTimeLineUpdate(float Value)
{
	// Set fade value for the character all materials
	const uint8 Length = MaterialInstances.Num();
	for (uint8 i = 0; i < Length; ++i)
	{
		MaterialInstances[i]->SetScalarParameterValue(FName("Fade"), Value);
	}
}

void ABaseCharacter::StartDestroy()
{
	Destroy();
}

void ABaseCharacter::Destroyed()
{
	if (GetWorld()->HasBegunPlay())
	{
		// Death handler does not call and weapons never detach if character get killed by world
		// Call drop weapon again in case if character killed by the world
		DropWeapon(EWeaponToDo::PrimaryWeapon);
		DropWeapon(EWeaponToDo::SecondaryWeapon);
		DropWeapon(EWeaponToDo::SidearmWeapon);
	}
	
	Super::Destroyed();
}

void ABaseCharacter::PlayIdleAnimation()
{
	// Play montage only when the character is stationary
	if (bIsAiming == false && GetCharacterMovement()->Velocity.Length() == 0.0f)
	{
		UAnimMontage* MontageToPlay;
		
		// If the character is holding a weapon but not aiming it, play idle montage based on weapon type
		if (CurrentWeapon)
		{
			MontageToPlay = CurrentWeapon->IdleMontage;
		}
		// If the character is not holding a weapon play a random idle animation
		else
		{
			MontageToPlay = IdleMontages[FMath::RandRange(0, IdleMontages.Num() - 1)];
		}
		
		if (GetMesh()->AnimScriptInstance->Montage_Play(MontageToPlay) > 0.0f)
		{
			bCanHolster = false;
		}
	}
}

void ABaseCharacter::Landed(const FHitResult& Hit)
{
	Super::Landed(Hit);
	
	// TODO: Use CMC crouch
	// if (MovementState == EMovementState::Crouch)
	// {
	// 	Crouch();
	// }
}

void ABaseCharacter::SwitchIsEnded()
{
	bCanReload = true;
}
