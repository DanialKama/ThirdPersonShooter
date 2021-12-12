// All Rights Reserved.

#include "Characters/BaseCharacter.h"
#include "Actors/Magazine.h"
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
#include "Kismet/GameplayStatics.h"
#include "AIModule/Classes/AIController.h"

class AAIController;

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

	GetCharacterMovement()->BrakingFriction = 0.1f;
	GetCharacterMovement()->CrouchedHalfHeight = 65.0f;
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
	
	bUseControllerRotationYaw = false;
	
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

void ABaseCharacter::CharacterIsOnMove()
{
	GetWorld()->GetTimerManager().ClearTimer(IdleTimer);
	StopAnimMontage();
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
		if(GetCharacterMovement()->IsFalling())
		{
			UnCrouch();
		}
		else
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
	APickupWeapon* NewWeapon;
	AAIController* AIController = Cast<AAIController>(GetController());
	// If the controller is AI do not spawn weapon (AI by default spawn weapon and does not pick up)
	if(AIController)
	{
		NewWeapon = WeaponToEquip;
	}
	else
	{
		NewWeapon = SpawnAndReplaceWeapon(WeaponToEquip);
	}

	if(NewWeapon)
	{
		// If weapon owner is the player then weapon use camera reference for line trace
		NewWeapon->CameraComponent = ChildCameraComponent;
		NewWeapon->SetInstigator(this);
		NewWeapon->SetOwner(this);
		NewWeapon->SetPickupStatus(EPickupState::Pickup);
	
		if(CurrentWeapon)
		{
			NewWeapon->LowerWeapon();
			switch (EquipAsWeapon)
			{
			case 0:
				// Not a valid slot
				break;
			case 1:
				// Add to primary weapon slot
				AttachToPhysicsConstraint(NewWeapon, EWeaponToDo::PrimaryWeapon);
				PrimaryWeapon = NewWeapon;
				PrimaryWeaponSupportedAmmo = PrimaryWeapon->WeaponInfo.AmmoType;
				break;
			case 2:
				// Add to secondary weapon slot
				AttachToPhysicsConstraint(NewWeapon, EWeaponToDo::SecondaryWeapon);
				SecondaryWeapon = NewWeapon;
				SecondaryWeaponSupportedAmmo = SecondaryWeapon->WeaponInfo.AmmoType;
				break;
			case 3:
				// Add to sidearm weapon slot
				AttachToPhysicsConstraint(NewWeapon, EWeaponToDo::SidearmWeapon);
				SidearmWeapon = NewWeapon;
				SidearmWeaponSupportedAmmo = SidearmWeapon->WeaponInfo.AmmoType;
				break;
			}
		}
		else
		{
			NewWeapon->RaiseWeapon();
			const FAttachmentTransformRules AttachmentTransformRules = FAttachmentTransformRules(EAttachmentRule::SnapToTarget, EAttachmentRule::SnapToTarget, EAttachmentRule::KeepWorld, true);
			NewWeapon->AttachToComponent(GetMesh(), AttachmentTransformRules, FName("RightHandHoldSocket"));

			// Check if the new weapon is a sidearm then replace it with the old sidearm
			const EWeaponType NewWeaponType = NewWeapon->WeaponInfo.WeaponType;
			if(NewWeaponType == EWeaponType::Pistol || NewWeaponType == EWeaponType::SMG)
			{
				SidearmWeapon = NewWeapon;
				SidearmWeaponSupportedAmmo = SidearmWeapon->WeaponInfo.AmmoType;
				SetCurrentWeapon(SidearmWeapon, EWeaponToDo::SidearmWeapon);
			}
			// If the primary weapon is valid then the secondary weapon replace with the new weapon
			else if(PrimaryWeapon)
			{
				SecondaryWeapon = NewWeapon;
				SecondaryWeaponSupportedAmmo = SecondaryWeapon->WeaponInfo.AmmoType;
				SetCurrentWeapon(SecondaryWeapon, EWeaponToDo::SecondaryWeapon);
			}
			// The primary weapon is only replaced when it is invalid
			else
			{
				PrimaryWeapon = NewWeapon;
				PrimaryWeaponSupportedAmmo = PrimaryWeapon->WeaponInfo.AmmoType;
				SetCurrentWeapon(PrimaryWeapon, EWeaponToDo::PrimaryWeapon);
			}

			SetArmedState(true);
		}
	}
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
	case 0:
		// No Weapon = nothing to drop
		break;
	case 1:
		// Drop the primary weapon
		if(PrimaryWeapon)
		{
			PrimaryWeapon->LowerWeapon();
			PrimaryWeapon->DetachFromActor(DetachmentTransformRules);
			PrimaryWeapon->SetPickupStatus(EPickupState::Drop);
			PrimaryWeapon = nullptr;
		}
		break;
	case 2:
		// Drop the secondary weapon
		if(SecondaryWeapon)
		{
			SecondaryWeapon->LowerWeapon();
			SecondaryWeapon->DetachFromActor(DetachmentTransformRules);
			SecondaryWeapon->SetPickupStatus(EPickupState::Drop);
			SecondaryWeapon = nullptr;
		}
		break;
	case 3:
		// Drop the sidearm weapon
		if(SidearmWeapon)
		{
			SidearmWeapon->LowerWeapon();
			SidearmWeapon->DetachFromActor(DetachmentTransformRules);
			SidearmWeapon->SetPickupStatus(EPickupState::Drop);
			SidearmWeapon = nullptr;
		}
		break;
	}

	// If something dropped and dropped weapon was not the sidearm weapon
	if(WeaponToDrop != EWeaponToDo::NoWeapon && CurrentHoldingWeapon != EWeaponToDo::SidearmWeapon)
	{
		SetCurrentWeapon(nullptr, EWeaponToDo::NoWeapon);
	}
}

void ABaseCharacter::SetCurrentWeapon(APickupWeapon* NewCurrentWeapon, EWeaponToDo WeaponSlot)
{
	CurrentHoldingWeapon = WeaponSlot;
	if(!NewCurrentWeapon)
	{
		StopAiming();
		ResetAim();
	}

	CurrentWeapon = NewCurrentWeapon;
	if(CurrentWeapon)
	{
		WeaponType = CurrentWeapon->WeaponInfo.WeaponType;
	}
}

void ABaseCharacter::PickupAmmo(APickup* NewAmmo)
{
	if(NewAmmo)
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

			Ammo->SetPickupStatus(EPickupState::Remove);
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

void ABaseCharacter::StartFireWeapon() const
{
	if(CurrentWeapon && bIsAimed)
	{
		CurrentWeapon->StartFireWeapon();
	}
}

void ABaseCharacter::StopFireWeapon() const
{
	if(CurrentWeapon)
	{
		CurrentWeapon->StopFireWeapon();
	}
}

void ABaseCharacter::ReloadWeapon()
{
	if(bDoOnceReload && CurrentWeapon && CurrentWeapon->AmmoComponent->CanReload() && CurrentHoldingWeapon != EWeaponToDo::NoWeapon)
	{
		UAnimMontage* MontageToPlay = nullptr;
		const int32 Index = static_cast<int32>(WeaponType);
		switch (MovementState)
		{
		case 0: case 1: case 2:
			// Walk, Run, and Sprint
			MontageToPlay = StandUpReloadAnimations[Index];
			break;
		case 3:
			// Crouch
			MontageToPlay = CrouchReloadAnimations[Index];
			break;
		case 4:
			// Prone
			DisableInput(UGameplayStatics::GetPlayerController(GetWorld(), 0));
			MontageToPlay = ProneReloadAnimations[Index];
			break;
		}
		
		const float MontageLenght = AnimInstance->Montage_Play(MontageToPlay);
		if(MontageLenght > 0.0f)
		{
			AnimInstance->Montage_JumpToSection(FName("Start"), MontageToPlay);
			
			FOnMontageEnded EndedDelegate;
			EndedDelegate.BindUObject(this, &ABaseCharacter::ReloadWeaponMontageHandler);
			AnimInstance->Montage_SetEndDelegate(EndedDelegate, MontageToPlay);

			if(CurrentWeapon->GetClass()->ImplementsInterface(UCommonInterface::StaticClass()))
			{
				Execute_SetWeaponState(CurrentWeapon, EWeaponState::Idle);
			}
		}
	}
	else
	{
		ResetReload();
	}
}

void ABaseCharacter::ReloadWeaponMontageHandler(UAnimMontage* AnimMontage, bool bInterrupted)
{
	if(bInterrupted)
	{
		if(CurrentWeapon)
		{
			CurrentWeapon->SetMagazineVisibility(true);
			if(CurrentWeapon->GetClass()->ImplementsInterface(UCommonInterface::StaticClass()))
			{
				Execute_SetWeaponState(CurrentWeapon, EWeaponState::Idle);
			}
		}
		Magazine->Destroy();
		ResetReload();
	}
	else
	{
		EnableInput(UGameplayStatics::GetPlayerController(GetWorld(), 0));
		StopAnimMontage();
		if(CurrentWeapon)
		{
			CurrentWeapon->ReloadWeapon();
			ResetReload();
		}
	}
}

void ABaseCharacter::SetReloadState(const EReloadState ReloadState)
{
	if(CurrentWeapon)
	{
		const FAttachmentTransformRules AttachmentTransformRules = FAttachmentTransformRules(EAttachmentRule::SnapToTarget, EAttachmentRule::SnapToTarget, EAttachmentRule::KeepWorld, true);
		switch (ReloadState)
		{
		case 0:
			// Start Reload
			break;
		case 1:
			// Remove Mag
			SpawnMagazine(CurrentWeapon);
			CurrentWeapon->SetMagazineVisibility(false);
			Magazine->StaticMesh->AttachToComponent(GetMesh(), AttachmentTransformRules, FName("LeftHandHoldSocket"));
			break;
		case 2:
			// Drop Mag
			Magazine->StaticMesh->SetSimulatePhysics(true);
			break;
		case 3:
			// Pick Mag
			SpawnMagazine(CurrentWeapon);
			CurrentWeapon->SetMagazineVisibility(false);
			Magazine->StaticMesh->AttachToComponent(GetMesh(), AttachmentTransformRules, FName("LeftHandHoldSocket"));
			break;
		case 4:
			// Insert Mag
			Magazine->Destroy();
			CurrentWeapon->SetMagazineVisibility(true);
			break;
		case 5:
			// End Reload
			break;
		}
	}
}

void ABaseCharacter::SpawnMagazine(const APickupWeapon* Weapon)
{
	if(IsValid(Weapon->WeaponDefaults.Magazine))
	{
		FActorSpawnParameters SpawnParameters;
		SpawnParameters.Owner = this;
		SpawnParameters.Instigator = GetInstigator();
		SpawnParameters.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
		Magazine = GetWorld()->SpawnActor<AMagazine>(Weapon->WeaponDefaults.Magazine, GetMesh()->GetSocketLocation(FName("LeftHandHoldSocket")), FRotator::ZeroRotator, SpawnParameters);
		CurrentWeapon->AmmoComponent->CurrentMagazineAmmo > 0 ? Magazine->bMagazineIsEmpty = false : Magazine->bMagazineIsEmpty = true;
	}
}

void ABaseCharacter::ResetReload()
{
	bDoOnceReload = true;
}

void ABaseCharacter::HolsterWeapon()
{
	if(CurrentWeapon)
	{
		const int32 CurrentWeaponIndex = static_cast<int32>(WeaponType);
		UAnimMontage* MontageToPlay = HolsterWeaponAnimations[CurrentWeaponIndex];
		
		SetArmedState(false);
		if(CurrentHoldingWeapon != EWeaponToDo::NoWeapon)
		{
			CurrentWeapon->LowerWeapon();
			const float MontageLenght = AnimInstance->Montage_Play(MontageToPlay);
			if(MontageLenght > 0.0f)
			{
				AnimInstance->Montage_JumpToSection(FName("Start"), MontageToPlay);
			
				FOnMontageEnded EndedDelegate;
				EndedDelegate.BindUObject(this, &ABaseCharacter::HolsterWeaponMontageHandler);
				AnimInstance->Montage_SetEndDelegate(EndedDelegate, MontageToPlay);
			}
		}
		else
		{
			SwitchIsEnded();
		}
	}
}

void ABaseCharacter::HolsterWeaponMontageHandler(UAnimMontage* AnimMontage, bool bInterrupted)
{
	if(bInterrupted)
	{
		SetArmedState(true);
		SwitchIsEnded();
	}
}

void ABaseCharacter::UpdateHolsterWeaponNotifyState(ENotifyState NotifyState)
{
	if(NotifyState == ENotifyState::End)
	{
		if(HolsterWeaponType != EWeaponType::Pistol && HolsterWeaponType != EWeaponType::SMG)
		{
			switch (WeaponToGrab)
			{
			case 0:
				// No Weapon = no switch
				break;
			case 1:
				// Primary Weapon
				SwitchWeaponHandler(PrimaryWeapon, EWeaponToDo::PrimaryWeapon, true);
				break;
			case 2:
				// Secondary Weapon
				SwitchWeaponHandler(SecondaryWeapon, EWeaponToDo::SecondaryWeapon, true);
				break;
			case 3:
				// Sidearm Weapon
				SwitchWeaponHandler(SidearmWeapon, EWeaponToDo::SidearmWeapon, true);
				break;
			}
		}
		else
		{
			AttachToPhysicsConstraint(CurrentWeapon, CurrentHoldingWeapon);
			SetCurrentWeapon(nullptr, EWeaponToDo::NoWeapon);
			SetArmedState(false);
			switch (WeaponToGrab)
			{
			case 0:
				// No Weapon = no switch
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
	}
}

void ABaseCharacter::SwitchToPrimary()
{
	if(PrimaryWeapon)
	{
		SetArmedState(false);

		const int32 CurrentWeaponType = static_cast<int32>(WeaponType);
		const int32 PrimaryWeaponType = static_cast<int32>(PrimaryWeapon->WeaponInfo.WeaponType);
		UAnimMontage* GrabMontage = GrabWeaponAnimations[PrimaryWeaponType];
		UAnimMontage* HolsterMontage = HolsterWeaponAnimations[CurrentWeaponType];
		
		switch (CurrentHoldingWeapon)
		{
			float MontageLenght;
		case 0:
			// If currently holding no weapon then grab the primary weapon
			MontageLenght = AnimInstance->Montage_Play(GrabMontage);
			if(MontageLenght > 0.0f)
			{
				AnimInstance->Montage_JumpToSection(FName("Start"), GrabMontage);
				GrabbedWeapon = PrimaryWeapon;
				WeaponToGrab = EWeaponToDo::PrimaryWeapon;
			}
			break;
		case 1:
			// If currently holding the primary weapon then holster it
			HolsterWeapon();
			break;
		case 2: case 3:
			// If currently holding the secondary or sidearm weapon then holster it and grab the primary weapon
			CurrentWeapon->LowerWeapon();
			MontageLenght = AnimInstance->Montage_Play(HolsterMontage);
			if(MontageLenght > 0.0f)
			{
				AnimInstance->Montage_JumpToSection(FName("Start"), HolsterMontage);

				HolsterWeaponType = CurrentWeapon->WeaponInfo.WeaponType;
				FOnMontageEnded EndedDelegate;
				EndedDelegate.BindUObject(this, &ABaseCharacter::HolsterWeaponMontageHandler);
				AnimInstance->Montage_SetEndDelegate(EndedDelegate, HolsterMontage);
			}
			break;
		}
	}
}

void ABaseCharacter::SwitchToSecondary()
{
	if(SecondaryWeapon)
	{
		SetArmedState(false);

		const int32 CurrentWeaponType = static_cast<int32>(WeaponType);
		const int32 SecondaryWeaponType = static_cast<int32>(SecondaryWeapon->WeaponInfo.WeaponType);
		UAnimMontage* GrabMontage = GrabWeaponAnimations[SecondaryWeaponType];
		UAnimMontage* HolsterMontage = HolsterWeaponAnimations[CurrentWeaponType];
		
		switch (CurrentHoldingWeapon)
		{
			float MontageLenght;
		case 0:
			// If currently holding no weapon then grab the secondary weapon
			MontageLenght = AnimInstance->Montage_Play(GrabMontage);
			if(MontageLenght > 0.0f)
			{
				AnimInstance->Montage_JumpToSection(FName("Start"), GrabMontage);
				GrabbedWeapon = SecondaryWeapon;
				WeaponToGrab = EWeaponToDo::SecondaryWeapon;
			}
			break;
		case 2:
			// If currently holding the secondary weapon then holster it
			HolsterWeapon();
			break;
		case 1: case 3:
			// If currently holding the primary or sidearm weapon then holster it and grab the secondary weapon
			CurrentWeapon->LowerWeapon();
			MontageLenght = AnimInstance->Montage_Play(HolsterMontage);
			if(MontageLenght > 0.0f)
			{
				AnimInstance->Montage_JumpToSection(FName("Start"), HolsterMontage);
				
				HolsterWeaponType = CurrentWeapon->WeaponInfo.WeaponType;
				FOnMontageEnded EndedDelegate;
				EndedDelegate.BindUObject(this, &ABaseCharacter::HolsterWeaponMontageHandler);
				AnimInstance->Montage_SetEndDelegate(EndedDelegate, HolsterMontage);
			}
			break;
		}
	}
}

void ABaseCharacter::SwitchToSidearm()
{
	if(SidearmWeapon)
	{
		SetArmedState(false);

		const int32 CurrentWeaponType = static_cast<int32>(WeaponType);
		const int32 SidearmWeaponType = static_cast<int32>(SidearmWeapon->WeaponInfo.WeaponType);
		UAnimMontage* GrabMontage = GrabWeaponAnimations[SidearmWeaponType];
		UAnimMontage* HolsterMontage = HolsterWeaponAnimations[CurrentWeaponType];
		
		switch (CurrentHoldingWeapon)
		{
			float MontageLenght;
		case 0:
			// If currently holding no weapon then grab the sidearm weapon
			MontageLenght = AnimInstance->Montage_Play(GrabMontage);
			if(MontageLenght > 0.0f)
			{
				AnimInstance->Montage_JumpToSection(FName("Start"), GrabMontage);
				GrabbedWeapon = SidearmWeapon;
				WeaponToGrab = EWeaponToDo::SidearmWeapon;
			}
			break;
		case 3:
			// If currently holding the sidearm weapon then holster it
			HolsterWeapon();
			break;
		case 1: case 2:
			// If currently holding the primary or secondary weapon then holster it and grab the sidearm weapon
			CurrentWeapon->LowerWeapon();
			MontageLenght = AnimInstance->Montage_Play(HolsterMontage);
			if(MontageLenght > 0.0f)
			{
				AnimInstance->Montage_JumpToSection(FName("Start"), HolsterMontage);
				
				HolsterWeaponType = CurrentWeapon->WeaponInfo.WeaponType;
				FOnMontageEnded EndedDelegate;
				EndedDelegate.BindUObject(this, &ABaseCharacter::HolsterWeaponMontageHandler);
				AnimInstance->Montage_SetEndDelegate(EndedDelegate, HolsterMontage);
			}
			break;
		}
	}
}

void ABaseCharacter::SwitchIsEnded()
{
	
}

void ABaseCharacter::SwitchWeaponHandler(APickupWeapon* WeaponToSwitch, EWeaponToDo TargetWeapon, bool bSwitchWeapon)
{
	if(WeaponToSwitch)
	{
		WeaponToSwitch->SkeletalMesh->SetSimulatePhysics(false);
		WeaponToSwitch->SkeletalMesh->SetCollisionProfileName(FName("NoCollision"), false);

		// If character try to switch attach the current weapon to physics container and set target weapon as current weapon
		if(bSwitchWeapon)
		{
			AttachToPhysicsConstraint(CurrentWeapon, CurrentHoldingWeapon);
		}
		
		switch (TargetWeapon)
		{
		case 0:
			// No weapon to switch to
			break;
		case 1:
			SetCurrentWeapon(WeaponToSwitch, EWeaponToDo::PrimaryWeapon);
			break;
		case 2:
			SetCurrentWeapon(WeaponToSwitch, EWeaponToDo::SecondaryWeapon);
			break;
		case 3:
			SetCurrentWeapon(WeaponToSwitch, EWeaponToDo::SidearmWeapon);
			break;
		}

		const FAttachmentTransformRules AttachmentTransformRules = FAttachmentTransformRules(EAttachmentRule::SnapToTarget, EAttachmentRule::SnapToTarget, EAttachmentRule::KeepWorld, true);
		WeaponToSwitch->AttachToComponent(GetMesh(), AttachmentTransformRules, FName("RightHandHoldSocket"));
		SetArmedState(true);
	}
}

void ABaseCharacter::UpdateGrabWeaponNotifyState(ENotifyState NotifyState)
{
	switch (NotifyState)
	{
	case 0:
		// Start
		if(GrabbedWeapon)
		{
			SwitchWeaponHandler(GrabbedWeapon, WeaponToGrab, false);
			GrabbedWeapon->RaiseWeapon();
			
			const int32 GrabbedWeaponIndex = static_cast<int32>(WeaponType);
			UAnimMontage* MontageToPlay = GrabWeaponAnimations[GrabbedWeaponIndex];
			
			const float MontageLenght = AnimInstance->Montage_Play(MontageToPlay, 1.0f, EMontagePlayReturnType::MontageLength);
			if(MontageLenght > 0.0f)
			{
				AnimInstance->Montage_JumpToSection(FName("Grab"), MontageToPlay);
			}
		}
		break;
	case 1:
		// End
		StopAnimMontage();
		SwitchIsEnded();
		break;
	}
}

void ABaseCharacter::AttachToPhysicsConstraint(APickupWeapon* WeaponToAttach, EWeaponToDo TargetWeapon) const
{
	const FAttachmentTransformRules AttachmentTransformRules = FAttachmentTransformRules(EAttachmentRule::SnapToTarget, EAttachmentRule::SnapToTarget, EAttachmentRule::KeepWorld, true);
	WeaponToAttach->SkeletalMesh->SetCollisionProfileName(FName("HolsterWeapon"), false);
	switch (TargetWeapon)
	{
	case 0:
		// No weapon to attach
		break;
	case 1:
		// Attach the primary weapon to hinge 1
		WeaponToAttach->AttachToComponent(Hinge1, AttachmentTransformRules);
		break;
	case 2:
		// Attach the secondary weapon to hinge 2
		WeaponToAttach->AttachToComponent(Hinge2, AttachmentTransformRules);
		break;
	case 3:
		// Attach the sidearm weapon to mesh socket
		WeaponToAttach->AttachToComponent(GetMesh(), AttachmentTransformRules, FName("Weapon3Socket"));
		break;
	}
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
	bIsArmed = bArmedState;
	if(bCharacterAnimationInterface)
	{
		ICharacterAnimationInterface::Execute_SetArmedState(AnimInstance, bArmedState, CurrentWeapon);
	}
	StopAnimMontage();
	if(!bArmedState)
	{
		StopAiming();
	}
}

bool ABaseCharacter::StartAiming()
{
	if(bIsArmed && CurrentWeapon)
	{
		bIsAimed = true;
		GetCharacterMovement()->bUseControllerDesiredRotation = true;
		GetCharacterMovement()->bOrientRotationToMovement = false;
		
		if(bCharacterAnimationInterface)
		{
			ICharacterAnimationInterface::Execute_SetAimedState(AnimInstance, true, CurrentWeapon);
		}
		
		switch (MovementState)
		{
		case 0: case 1: case 2:
			// Set character movement state to walk to stop the character from running or sprinting while aiming
			SetMovementState_Implementation(EMovementState::Walk, false, false);
			break;
		case 3:
			SetMovementState_Implementation(EMovementState::Crouch, false, false);
			break;
		case 4:
			SetMovementState_Implementation(EMovementState::Prone, false, false);
			break;
		}
		
		StopAnimMontage();
		return true;
	}
	
	StopAiming();
	return false;
}

void ABaseCharacter::StopAiming()
{
	bIsAimed = false;
	GetCharacterMovement()->bUseControllerDesiredRotation = false;
	GetCharacterMovement()->bOrientRotationToMovement = true;
	StopFireWeapon();

	if(bCharacterAnimationInterface)
	{
		ICharacterAnimationInterface::Execute_SetAimedState(AnimInstance, false, CurrentWeapon);
	}
}

void ABaseCharacter::DropItem()
{
	DropWeapon(CurrentHoldingWeapon);
	SetArmedState(false);
}

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
	UAnimMontage* MontageToPlay = nullptr;
	const int32 Index = static_cast<int32>(WeaponType);
	// If the character is holding a weapon but not aiming play idle montage based on weapon type
	if(!bIsAimed && bIsArmed)
	{
		MontageToPlay = ArmedIdleAnimations[Index];
	}
	// If the character is not holding a weapon play a random idle animation
	else if(!bIsAimed && !bIsArmed)
	{
		const int32 Lenght = IdleAnimations.Max();
		MontageToPlay = IdleAnimations[FMath::RandRange(0, Lenght)];
	}
	
	PlayAnimMontage(MontageToPlay);
}

void ABaseCharacter::StartJump()
{
	if(MovementState != EMovementState::Crouch && MovementState != EMovementState::Prone)
	{
		Jump();
	}
}

void ABaseCharacter::Landed(const FHitResult& Hit)
{
	if(MovementState == EMovementState::Crouch)
	{
		Crouch();
	}
}

void ABaseCharacter::ToggleCrouch()
{
	switch (MovementState)
	{
	case 0: case 1: case 2: case 4:
		SetMovementState_Implementation(EMovementState::Crouch, true, false);
		break;
	case 3:
		SetMovementState_Implementation(EMovementState::Walk, true, false);
		break;
	}
}

/** Apply fall damage / Toggle ragdoll / Stand up the character */
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

void ABaseCharacter::ResetAim()
{
}