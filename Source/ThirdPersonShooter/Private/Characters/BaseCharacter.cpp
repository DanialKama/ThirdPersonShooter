// Copyright Epic Games, Inc. All Rights Reserved.

#include "Characters/BaseCharacter.h"
#include "Components/CapsuleComponent.h"
#include "PhysicsEngine/PhysicsConstraintComponent.h"
#include "Components/HealthComponent.h"
#include "Components/StaminaComponent.h"
#include "Interfaces/CommonInterface.h"
#include "Interfaces/CharacterInterface.h"
#include "Interfaces/CharacterAnimationInterface.h"
#include "Interfaces/PickupWeaponInterface.h"
#include "Actors/PickupWeapon.h"
#include "Actors/PickupAmmo.h"
#include "Components/AmmoComponent.h"
#include "GameFramework/CharacterMovementComponent.h"

// Sets default values
ABaseCharacter::ABaseCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Create components
	FallCapsule = CreateDefaultSubobject<UCapsuleComponent>(TEXT("Fall Capsule"));
	Root1 = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Root 1"));
	Hinge1 = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Hinge 1"));
	PhysicsConstraint1 = CreateDefaultSubobject<UPhysicsConstraintComponent>(TEXT("Physics Constraint 1"));
	Root2 = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Root 2"));
	Hinge2 = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Hinge 2"));
	PhysicsConstraint2 = CreateDefaultSubobject<UPhysicsConstraintComponent>(TEXT("Physics Constraint 2"));
	Root3 = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Root 3"));
	Hinge3 = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Hinge 3"));
	PhysicsConstraint3 = CreateDefaultSubobject<UPhysicsConstraintComponent>(TEXT("Physics Constraint 3"));
	HealthComponent = CreateDefaultSubobject<UHealthComponent>(TEXT("Health Component"));
	StaminaComponent = CreateDefaultSubobject<UStaminaComponent>(TEXT("Stamina Component"));
	
	// Setup components attachment
	FallCapsule->SetupAttachment(GetMesh());
	Root1->SetupAttachment(GetMesh(), TEXT("Weapon1Socket"));
	Hinge1->SetupAttachment(Root1);
	PhysicsConstraint1->SetupAttachment(Hinge1);
	Root2->SetupAttachment(GetMesh(), TEXT("Weapon2Socket"));
	Hinge2->SetupAttachment(Root2);
	PhysicsConstraint2->SetupAttachment(Hinge2);
	Root3->SetupAttachment(GetMesh(), TEXT("Weapon3Socket"));
	Hinge3->SetupAttachment(Root3);
	PhysicsConstraint3->SetupAttachment(Hinge3);

	// Set component defaults
	static ConstructorHelpers::FObjectFinder<UStaticMesh>CubeAsset(TEXT("StaticMesh'/Engine/BasicShapes/Cube.Cube'"));
	UStaticMesh* Cube = CubeAsset.Object;
	static ConstructorHelpers::FObjectFinder<UStaticMesh>PlaneAsset(TEXT("StaticMesh'/Engine/BasicShapes/Plane.Plane'"));
	UStaticMesh* Plane = PlaneAsset.Object;
	
	FallCapsule->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	
	Root1->SetStaticMesh(Cube);
	Root1->SetGenerateOverlapEvents(false);
	Root1->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	Root1->SetCollisionObjectType(ECC_WorldDynamic);
	Root1->SetCollisionResponseToAllChannels(ECR_Ignore);
	Root1->SetVisibility(false);
	Root1->SetHiddenInGame(true);

	Hinge1->SetStaticMesh(Plane);
	Hinge1->SetSimulatePhysics(true);
	Hinge1->SetEnableGravity(false);
	Hinge1->SetGenerateOverlapEvents(false);
	Hinge1->SetCollisionEnabled(ECollisionEnabled::PhysicsOnly);
	Hinge1->SetCollisionObjectType(ECC_WorldDynamic);
	Hinge1->SetCollisionResponseToAllChannels(ECR_Ignore);
	Hinge1->SetVisibility(false);
	Hinge1->SetHiddenInGame(true);

	PhysicsConstraint1->bUseAttachParentBound = true;
	PhysicsConstraint1->ComponentName1.ComponentName = TEXT("Hinge1");
	PhysicsConstraint1->ComponentName2.ComponentName = TEXT("Root1");
	PhysicsConstraint1->SetDisableCollision(true);
	PhysicsConstraint1->SetAngularSwing1Limit(ACM_Limited, 10.0f);
	PhysicsConstraint1->SetAngularSwing2Limit(ACM_Limited, 5.0f);
	PhysicsConstraint1->SetAngularTwistLimit(ACM_Limited, 10.0f);

	Root2->SetStaticMesh(Cube);
	Root2->SetGenerateOverlapEvents(false);
	Root2->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	Root2->SetCollisionObjectType(ECC_WorldDynamic);
	Root2->SetCollisionResponseToAllChannels(ECR_Ignore);
	Root2->SetVisibility(false);
	Root2->SetHiddenInGame(true);
	
	Hinge2->SetStaticMesh(Plane);
	Hinge2->SetSimulatePhysics(true);
	Hinge2->SetEnableGravity(false);
	Hinge2->SetGenerateOverlapEvents(false);
	Hinge2->SetCollisionEnabled(ECollisionEnabled::PhysicsOnly);
	Hinge2->SetCollisionObjectType(ECC_WorldDynamic);
	Hinge2->SetCollisionResponseToAllChannels(ECR_Ignore);
	Hinge2->SetVisibility(false);
	Hinge2->SetHiddenInGame(true);

	PhysicsConstraint2->bUseAttachParentBound = true;
	PhysicsConstraint2->ComponentName1.ComponentName = TEXT("Hinge1");
	PhysicsConstraint2->ComponentName2.ComponentName = TEXT("Root1");
	PhysicsConstraint2->SetDisableCollision(true);
	PhysicsConstraint2->SetAngularSwing1Limit(ACM_Limited, 10.0f);
	PhysicsConstraint2->SetAngularSwing2Limit(ACM_Limited, 5.0f);
	PhysicsConstraint2->SetAngularTwistLimit(ACM_Limited, 10.0f);

	Root3->SetStaticMesh(Cube);
	Root3->SetGenerateOverlapEvents(false);
	Root3->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	Root3->SetCollisionObjectType(ECC_WorldDynamic);
	Root3->SetCollisionResponseToAllChannels(ECR_Ignore);
	Root3->SetVisibility(false);
	Root3->SetHiddenInGame(true);
	
	Hinge3->SetStaticMesh(Plane);
	Hinge3->SetSimulatePhysics(true);
	Hinge3->SetEnableGravity(false);
	Hinge3->SetGenerateOverlapEvents(false);
	Hinge3->SetCollisionEnabled(ECollisionEnabled::PhysicsOnly);
	Hinge3->SetCollisionObjectType(ECC_WorldDynamic);
	Hinge3->SetCollisionResponseToAllChannels(ECR_Ignore);
	Hinge3->SetVisibility(false);
	Hinge3->SetHiddenInGame(true);

	PhysicsConstraint3->bUseAttachParentBound = true;
	PhysicsConstraint3->ComponentName1.ComponentName = TEXT("Hinge1");
	PhysicsConstraint3->ComponentName2.ComponentName = TEXT("Root1");
	PhysicsConstraint3->SetDisableCollision(true);
	PhysicsConstraint3->SetAngularSwing1Limit(ACM_Limited, 10.0f);
	PhysicsConstraint3->SetAngularSwing2Limit(ACM_Limited, 5.0f);
	PhysicsConstraint3->SetAngularTwistLimit(ACM_Limited, 10.0f);
	
	// Overlap functions
	FallCapsule->OnComponentBeginOverlap.AddDynamic(this, &ABaseCharacter::OnFallCapsuleBeginOverlap);

	// Initialize variables
	bIsAlive = true;
	bDoOnceMoving = true;
	bDoOnceStopped = true;
	bDoOnceReload = true;
}

// Called when the game starts or when spawned
void ABaseCharacter::BeginPlay()
{
	Super::BeginPlay();

	AnimInstance = GetMesh()->GetAnimInstance();
	// Detected if the interfaces is present on anim instance
	if(AnimInstance->GetClass()->ImplementsInterface(UCharacterAnimationInterface::StaticClass()))
	{
		bCharacterAnimationInterface = true;
	}
	
	CharacterTagContainer.AddTag(TeamTag);
	FallCapsule->SetRelativeLocation(MeshLocationOffset);
	FallCapsule->IgnoreActorWhenMoving(this, true); // Fall Capsule should ignore self to not register a fall when overlap with self

	// Create material instances for every material on mesh, mainly used for death dither effect
	TArray<UMaterialInterface*> Materials = GetMesh()->GetMaterials();
	for (int32 i = 0; i < Materials.Num(); i++)
	{
		Materials.Add(GetMesh()->CreateDynamicMaterialInstance(i, Materials[i]));
	}

	HealthComponent->Initialize();
	StaminaComponent->Initialize();
}

// Called every frame
void ABaseCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// Check if character stopped or moving
	if(GetVelocity().Size() == 0.0f)
	{
		if(bDoOnceStopped)
		{
			if(bCharacterAnimationInterface)
			{
				ICharacterAnimationInterface::Execute_SetMovementState(AnimInstance, PreviousMovementState);
			}
			
			GetWorld()->GetTimerManager().SetTimer(IdleTimer, this, &ABaseCharacter::PlayIdleAnimation, FMath::FRandRange(60.0f, 90.0f), true);
			bDoOnceMoving = true;
		}
	}
	else
	{
		if(bDoOnceMoving)
		{
			CharacterIsOnMove();
			bDoOnceStopped = true;
		}
	}

	// If ragdoll state is true then keep mesh and capsule together
	if(bRagdollState)
	{
		// If this is not the player then no need to set capsule location after character death
		if(bIsAlive || IsPlayerControlled())
		{
			CalculateCapsuleLocation();
			GetCapsuleComponent()->SetWorldLocation(MeshLocation);
		}
	}
}

// Called to bind functionality to input
void ABaseCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

void ABaseCharacter::PossessedBy(AController* NewController)
{
	bIsTryToUseVehicle = false;
	ToggleUsingVehicle(false);
	SetWeaponVisibility(true);
}

void ABaseCharacter::UnPossessed()
{
	SetWeaponVisibility(false);
}

void ABaseCharacter::CharacterIsOnMove()
{
	GetWorld()->GetTimerManager().ClearTimer(IdleTimer);
	StopAnimMontage(AnimMontageToPlay);
	UpdateMovementState();
}

void ABaseCharacter::SetMovementState_Implementation(EMovementState CurrentMovementState, bool bRelatedToCrouch, bool bRelatedToProne)
{
	if(bRelatedToCrouch || bRelatedToProne)
	{
		PreviousMovementState = CurrentMovementState;
	}
	
	if(StaminaComponent->CurrentStamina > 0.0f)
	{
		MovementState = CurrentMovementState;
	}
	else
	{
		MovementState = PreviousMovementState;
	}

	UpdateMovementState();
}

void ABaseCharacter::UpdateMovementState()
{
	switch (MovementState)
	{
	case 0:
		// Walk
		StaminaComponent->StopStaminaDrain();
		GetCharacterMovement()->MaxWalkSpeed = 150.0f;
		GetCharacterMovement()->MaxWalkSpeedCrouched = 150.0f;
		GetCharacterMovement()->JumpZVelocity = 300.0f;
		break;
	case 1:
		// Run
		UnCrouch();
		StaminaComponent->StartRunning();
		GetCharacterMovement()->MaxWalkSpeed = 300.0f;
		GetCharacterMovement()->MaxWalkSpeedCrouched = 300.0f;
		GetCharacterMovement()->JumpZVelocity = 360.0f;
		break;
	case 2:
		// Sprint
		UnCrouch();
		StaminaComponent->StartSprinting();
		GetCharacterMovement()->MaxWalkSpeed = 600.0f;
		GetCharacterMovement()->MaxWalkSpeedCrouched = 600.0f;
		GetCharacterMovement()->JumpZVelocity = 420.0f;
		break;
	case 3:
		// Crouch
		if(!GetCharacterMovement()->IsFalling())
		{
			Crouch();
		}
		StaminaComponent->StopStaminaDrain();
		GetCharacterMovement()->MaxWalkSpeed = 150.0f;
		GetCharacterMovement()->MaxWalkSpeedCrouched = 150.0f;
		GetCharacterMovement()->JumpZVelocity = 0.0f;
		break;
	case 4:
		// Prone
		StaminaComponent->StopStaminaDrain();
		GetCharacterMovement()->MaxWalkSpeed = 80.0f;
		GetCharacterMovement()->MaxWalkSpeedCrouched = 80.0f;
		GetCharacterMovement()->JumpZVelocity = 0.0f;
		break;
	}

	if(bCharacterAnimationInterface)
	{
		ICharacterAnimationInterface::Execute_SetMovementState(AnimInstance, MovementState);
	}
}

void ABaseCharacter::SetPickup_Implementation(const EItemType NewPickupType, APickup* NewPickup)
{
	PickupType = NewPickupType;
	Pickup = NewPickup;
}

void ABaseCharacter::SetInteractable_Implementation(AActor* NewInteractable)
{
	Interactable = NewInteractable;
	// If there is no actor to interact
	if(Interactable)
	{
		bIsTryToUseVehicle = false;
	}
}

void ABaseCharacter::Interact_Implementation()
{
	if(Pickup)
	{
		switch(PickupType)
		{
		case 0:
			// Weapon
			PickupWeapon(Pickup);
			break;
		case 1:
			// Ammo
			PickupAmmo(Pickup);
			break;
		case 2:
			// Health
			break;
		}
	}
	else if(Interactable)
	{
		if(Interactable->ActorHasTag(TEXT("Vehicle")))
		{
			bIsTryToUseVehicle = true;
			HolsterWeapon();
		}
	}
}

void ABaseCharacter::PickupWeapon(APickup* NewWeapon)
{
	if(NewWeapon && NewWeapon->GetClass()->ImplementsInterface(UPickupWeaponInterface::StaticClass()))
	{
		APickupWeapon* Weapon = IPickupWeaponInterface::Execute_GetWeaponReference(NewWeapon);
		if(Weapon)
		{
			switch(Weapon->WeaponInfo.WeaponType)
			{
			case 0: case 1:
				// Pistol and SMG
				if(SidearmWeapon)
				{
					DropWeapon(EWeaponToDo::SidearmWeapon);
				}
				AddWeapon(Weapon, EWeaponToDo::SidearmWeapon);
				break;
			case 2: case 3: case 4: case 5: case 6:
				// Rifle, LMG, Shotgun, Sniper, and Launcher
				if(PrimaryWeapon)
				{
					if(SecondaryWeapon)
					{
						switch (CurrentHoldingWeapon)
						{
						case 0: case 1:
							// No Weapon and Primary Weapon - if character currently holding the primary weapon or no weapon then add the new weapon to the primary slot
							DropWeapon(EWeaponToDo::PrimaryWeapon);
							AddWeapon(Weapon, EWeaponToDo::PrimaryWeapon);
							break;
						case 2:
							// Secondary Weapon - if character currently holding the secondary weapon then add the new weapon to the secondary slot
							DropWeapon(EWeaponToDo::SecondaryWeapon);
							AddWeapon(Weapon, EWeaponToDo::SecondaryWeapon);
							break;
						case 3:
							// Sidearm Weapon - pistol and SMG handles in cases 0 and 1
							break;
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
				break;
			}
		}
	}
}

void ABaseCharacter::AddWeapon(APickupWeapon* WeaponToEquip, EWeaponToDo EquipAsWeapon)
{
	
}

void ABaseCharacter::DropWeapon(EWeaponToDo WeaponToDo)
{
	
}

void ABaseCharacter::SetCurrentWeapon(APickupWeapon* NewCurrentWeapon, EWeaponToDo NewCurrentHoldingWeapon)
{
	
}

void ABaseCharacter::PickupAmmo(APickup* NewAmmo)
{
	if(NewAmmo && NewAmmo->GetClass()->ImplementsInterface(UPickupAmmoInterface::StaticClass()))
	{
		APickupAmmo* Ammo = IPickupAmmoInterface::Execute_GetPickupAmmoReference(NewAmmo);
		if(Ammo)
		{
			switch (CanPickupAmmo_Implementation(Ammo->AmmoType))
			{
			case 0:
				// No Weapon = no ammo
				break;
			case 1:
				// Primary Weapon
				PrimaryWeapon->AmmoComponent->AddAmmo(Ammo->Amount);
				break;
			case 2:
				// Secondary Weapon
				SecondaryWeapon->AmmoComponent->AddAmmo(Ammo->Amount);
				break;
			case 3:
				// Sidearm Weapon
				SidearmWeapon->AmmoComponent->AddAmmo(Ammo->Amount);
				break;
			}

			IPickupInterface::Execute_SetPickupStatus(Ammo, EPickupState::Remove);
		}
	}
}

EWeaponToDo ABaseCharacter::CanPickupAmmo_Implementation(int32 AmmoType)
{
	if(CurrentWeapon)
	{
		bool bIsSameAmmo = false;
		switch (CurrentHoldingWeapon)
		{
		case 0:
			// No weapon = no ammo
			break;
		case 1:
			// Primary Weapon
			bIsSameAmmo = AmmoType & PrimaryWeaponSupportedAmmo;
			break;
		case 2:
			// Secondary Weapon
			bIsSameAmmo = AmmoType & SecondaryWeaponSupportedAmmo;
			break;
		case 3:
			// Sidearm Weapon
			bIsSameAmmo = AmmoType & SidearmWeaponSupportedAmmo;
			break;
		}

		if(CurrentWeapon->CanPickupAmmo() && bIsSameAmmo)
		{
			return CurrentHoldingWeapon;
		}
	}
	if(PrimaryWeapon && PrimaryWeapon->CanPickupAmmo() && AmmoType & PrimaryWeaponSupportedAmmo)
	{
		return EWeaponToDo::PrimaryWeapon;
	}
	if(SecondaryWeapon && SecondaryWeapon->CanPickupAmmo() && AmmoType & SecondaryWeaponSupportedAmmo)
	{
		return EWeaponToDo::SecondaryWeapon;
	}
	if(SidearmWeapon && SidearmWeapon->CanPickupAmmo() && AmmoType & SidearmWeaponSupportedAmmo)
	{
		return EWeaponToDo::SidearmWeapon;
	}
	
	return EWeaponToDo::NoWeapon;
}

void ABaseCharacter::StartFireWeapon()
{
	if(CurrentWeapon && bIsAimed)
	{
		CurrentWeapon->StartFireWeapon();
	}
}

void ABaseCharacter::StopFireWeapon()
{
	if(CurrentWeapon)
	{
		CurrentWeapon->StopFireWeapon();
	}
}

// Reload Weapon based on movement state and weapon type
void ABaseCharacter::ReloadWeapon()
{
	if(bDoOnceReload && CurrentWeapon && CurrentWeapon->AmmoComponent->CanReload() && CurrentHoldingWeapon != EWeaponToDo::NoWeapon)
	{
		switch (MovementState)
		{
		case 0: case 1: case 2:
			// Walk, Run, and Sprint
			switch (WeaponType)
			{
		case 0:
			// Pistol
			AnimMontageToPlay = StandUpReloadAnimations[0];
				break;
		case 1:
			// SMG
			AnimMontageToPlay = StandUpReloadAnimations[1];
				break;
		case 2:
			// Rifle
			AnimMontageToPlay = StandUpReloadAnimations[2];
				break;
		case 3:
			// LMG
			AnimMontageToPlay = StandUpReloadAnimations[3];
				break;
		case 4:
			// Shotgun
			AnimMontageToPlay = StandUpReloadAnimations[4];
				break;
		case 5:
			// Sniper
			AnimMontageToPlay = StandUpReloadAnimations[5];
				break;
		case 6:
			// Launcher
			AnimMontageToPlay = StandUpReloadAnimations[6];
				break;
			}
			break;
		case 3:
			// Crouch
			switch (WeaponType)
			{
		case 0:
			// Pistol
			AnimMontageToPlay = CrouchReloadAnimations[0];
				break;
		case 1:
			// SMG
			AnimMontageToPlay = CrouchReloadAnimations[1];
				break;
		case 2:
			// Rifle
			AnimMontageToPlay = CrouchReloadAnimations[2];
				break;
		case 3:
			// LMG
			AnimMontageToPlay = CrouchReloadAnimations[3];
				break;
		case 4:
			// Shotgun
			AnimMontageToPlay = CrouchReloadAnimations[4];
				break;
		case 5:
			// Sniper
			AnimMontageToPlay = CrouchReloadAnimations[5];
				break;
		case 6:
			// Launcher
			AnimMontageToPlay = CrouchReloadAnimations[6];
				break;
			}
			break;
		case 4:
			// Prone
			switch (WeaponType)
			{
		case 0:
			// Pistol
			AnimMontageToPlay = ProneReloadAnimations[0];
				break;
		case 1:
			// SMG
			AnimMontageToPlay = ProneReloadAnimations[1];
				break;
		case 2:
			// Rifle
			AnimMontageToPlay = ProneReloadAnimations[2];
				break;
		case 3:
			// LMG
			AnimMontageToPlay = ProneReloadAnimations[3];
				break;
		case 4:
			// Shotgun
			AnimMontageToPlay = ProneReloadAnimations[4];
				break;
		case 5:
			// Sniper
			AnimMontageToPlay = ProneReloadAnimations[5];
				break;
		case 6:
			// Launcher
			AnimMontageToPlay = ProneReloadAnimations[6];
				break;
			}
			break;
		}
		AnimInstance->Montage_Play(AnimMontageToPlay, 2.0f);
		FOnMontageEnded CompleteDelegate;
		CompleteDelegate.BindUObject(this, &ABaseCharacter::ReloadHandler);
		CompleteDelegate.BindUFunction(this, TEXT("Reload"));
		AnimInstance->Montage_SetEndDelegate(CompleteDelegate, AnimMontageToPlay);
		/*void UPlayMontageCallbackProxy::PlayMontage(class USkeletalMeshComponent* InSkeletalMeshComponent, 
			class UAnimMontage* MontageToPlay, 
			float PlayRate, 
			float StartingPosition, 
			FName StartingSection)
		{
			bool bPlayedSuccessfully = false;
			if (InSkeletalMeshComponent)
			{
				if (UAnimInstance* AnimInstance = InSkeletalMeshComponent->GetAnimInstance())
				{
					const float MontageLength = AnimInstance->Montage_Play(MontageToPlay, PlayRate, EMontagePlayReturnType::MontageLength, StartingPosition);
					bPlayedSuccessfully = (MontageLength > 0.f);
 
					if (bPlayedSuccessfully)
					{
						AnimInstancePtr = AnimInstance;
						if (FAnimMontageInstance* MontageInstance = AnimInstance->GetActiveInstanceForMontage(MontageToPlay))
						{
							MontageInstanceID = MontageInstance->GetInstanceID();
						}
 
						if (StartingSection != NAME_None)
						{
							AnimInstance->Montage_JumpToSection(StartingSection, MontageToPlay);
						}
 
						BlendingOutDelegate.BindUObject(this, &UPlayMontageCallbackProxy::OnMontageBlendingOut);
						AnimInstance->Montage_SetBlendingOutDelegate(BlendingOutDelegate, MontageToPlay);
 
						MontageEndedDelegate.BindUObject(this, &UPlayMontageCallbackProxy::OnMontageEnded);
						AnimInstance->Montage_SetEndDelegate(MontageEndedDelegate, MontageToPlay);
 
						AnimInstance->OnPlayMontageNotifyBegin.AddDynamic(this, &UPlayMontageCallbackProxy::OnNotifyBeginReceived);
						AnimInstance->OnPlayMontageNotifyEnd.AddDynamic(this, &UPlayMontageCallbackProxy::OnNotifyEndReceived);
					}
				}
			}
 
			if (!bPlayedSuccessfully)
			{
				OnInterrupted.Broadcast(NAME_None);
			}
		}*/
	}
	else
	{
		ResetReload();
	}
}

void ABaseCharacter::ReloadHandler(UAnimMontage* AnimMontage, bool bInterrupted)
{
	
}

void ABaseCharacter::SetReloadState(EReloadState ReloadState)
{
	
}

// Override in childs
void ABaseCharacter::ResetReload()
{
	bDoOnceReload = true;
}

void ABaseCharacter::HolsterWeapon()
{
	
}

void ABaseCharacter::SwitchToPrimary()
{
	
}

void ABaseCharacter::SwitchToSecondary()
{
	
}

void ABaseCharacter::SwitchToSidearm()
{
	
}

// Override by AI character
void ABaseCharacter::SwitchIsEnded()
{
	
}

// Set weapons visibility on event Possessed and UnPossessed, to hide weapons when character is in vehicle
void ABaseCharacter::SetWeaponVisibility(bool bNewVisibility)
{
	
}

void ABaseCharacter::AddRecoil_Implementation(const FRotator RotationIntensity, const float ControlTime, const float CrosshairRecoil, const float ControllerPitch)
{
	if(bCharacterAnimationInterface)
	{
		ICharacterAnimationInterface::Execute_AddRecoil(AnimInstance, RotationIntensity, ControlTime);
	}
}

void ABaseCharacter::SetArmedState(bool bArmedState)
{
	
}

// Call from Set Current Weapon and use in player character to exit aim mode
void ABaseCharacter::ResetAim()
{
	
}

void ABaseCharacter::DropItem()
{
	
}

void ABaseCharacter::ToggleUsingVehicle(bool bIsVehicle)
{
	
}

// Health Recovery based on Stamina level
void ABaseCharacter::SetStaminaLevel_Implementation(float Stamina, const bool bIsFull)
{
	if(HealthComponent)
	{
		HealthComponent->bCanRecoverHealth = bIsFull;

		if(bIsFull)
		{
			HealthComponent->StartHealthRecovery();
		}
		else
		{
			HealthComponent->StopHealthRecovery();
		}
	}
}

void ABaseCharacter::PlayIdleAnimation()
{
	if(!bIsAimed && bIsArmed)
	{
		switch (WeaponType)
		{
		case 0:
			// Pistol
			AnimMontageToPlay = ArmedIdleAnimations[0];
			break;
		case 1:
			// SMG
			AnimMontageToPlay = ArmedIdleAnimations[1];
			break;
		case 2:
			// Rifle
			AnimMontageToPlay = ArmedIdleAnimations[2];
			break;
		case 3:
			// LMG
			AnimMontageToPlay = ArmedIdleAnimations[3];
			break;
		case 4:
			// Shotgun
			AnimMontageToPlay = ArmedIdleAnimations[4];
			break;
		case 5:
			// Sniper
			AnimMontageToPlay = ArmedIdleAnimations[5];
			break;
		case 6:
			// Launcher
			AnimMontageToPlay = ArmedIdleAnimations[6];
			break;
		}
	}
	else if(!bIsAimed && !bIsArmed)
	{
		const int32 Lenght = IdleAnimations.Max();
		AnimMontageToPlay = IdleAnimations[FMath::RandRange(0, Lenght)];
	}

	PlayAnimMontage(AnimMontageToPlay);
}

void ABaseCharacter::StartJump()
{
	
}

void ABaseCharacter::StopJump()
{
	
}

void ABaseCharacter::Landed(const FHitResult& Hit)
{
	
}

void ABaseCharacter::ToggleCrouch()
{
	
}

// Apply fall damage / Toggle ragdoll / Stand up the character
void ABaseCharacter::OnFallCapsuleBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
	UPrimitiveComponent* OtherComp,int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	
}

void ABaseCharacter::CheckForFalling()
{
	
}

void ABaseCharacter::CachePose()
{
	
}

void ABaseCharacter::CalculateCapsuleLocation()
{
	FVector CapsuleLocation;
	FHitResult HitResult;
	const FVector Start = GetMesh()->GetSocketLocation(TEXT("pelvis"));
	const FVector End = Start + FVector(0.0f, 0.0f, -1.0f) * 100.0f;
	FCollisionQueryParams CollisionQueryParams;
	CollisionQueryParams.AddIgnoredActor(this);
	
	const bool bHit = GetWorld()->LineTraceSingleByChannel(HitResult, Start, End, ECC_Visibility, CollisionQueryParams);

	if(bHit)
	{
		CapsuleLocation = HitResult.Location + MeshLocationOffset;
	}
	else
	{
		CapsuleLocation = GetMesh()->GetSocketLocation(TEXT("pelvis")) + MeshLocationOffset;
	}
	
	MeshLocation = FMath::VInterpTo(MeshLocation, CapsuleLocation,  GetWorld()->GetDeltaSeconds(), 2.5f);
}