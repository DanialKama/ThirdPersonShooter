// Copyright Epic Games, Inc. All Rights Reserved.

#include "Actors/Pickup_Weapon.h"
#include "Kismet/GameplayStatics.h"
#include "Math/UnrealMathUtility.h"
#include "Kismet/KismetMathLibrary.h"
#include "DrawDebugHelpers.h"
#include "AIController.h"
#include "Perception/AISense_Hearing.h"
#include "Sound/SoundCue.h"
#include "Actors/EmptyShell.h"
#include "Actors/Projectile.h"
// Interfaces
#include "Interfaces/CharacterInterface.h"
#include "Interfaces/AIControllerInterface.h"
#include "Interfaces/PlayerControllerInterface.h"
#include "Interfaces/WidgetInterface.h"
// Components
#include "Components/BoxComponent.h"
#include "Particles/ParticleSystemComponent.h"
#include "Components/AudioComponent.h"
#include "Components/WidgetComponent.h"
#include "Components/AmmoComponent.h"
#include "Camera/CameraComponent.h"
// Structs
#include "Structs/AmmoComponentInfoStruct.h"

// Sets default values
APickup_Weapon::APickup_Weapon()
{
	// Create components
	SkeletalMesh	= CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Weapon"));
	BoxCollision	= CreateDefaultSubobject<UBoxComponent>(TEXT("Collision"));
	MuzzleFlash		= CreateDefaultSubobject<UParticleSystemComponent>(TEXT("Muzzle Flash"));
	FireSound		= CreateDefaultSubobject<UAudioComponent>(TEXT("Fire Sound"));
	Widget			= CreateDefaultSubobject<UWidgetComponent>(TEXT("Widget"));
	AmmoComponent	= CreateDefaultSubobject<UAmmoComponent>(TEXT("Ammo Component"));

	// Setup components attachment
	SetRootComponent(SkeletalMesh);
	BoxCollision->SetupAttachment(SkeletalMesh);
	MuzzleFlash->SetupAttachment(SkeletalMesh, TEXT("MuzzleFlashSocket"));
	FireSound->SetupAttachment(SkeletalMesh);
	Widget->SetupAttachment(SkeletalMesh);

	// Set component defaults
	SkeletalMesh->SetAnimationMode(EAnimationMode::AnimationSingleNode);
	SkeletalMesh->bApplyImpulseOnDamage = false;
	SkeletalMesh->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	SkeletalMesh->SetCollisionObjectType(ECC_PhysicsBody);	// In blueprint set all collision responses for skeletal mesh to ignore and World Static to block

	BoxCollision->SetBoxExtent(FVector (8.0f, 50.0f, 20.0f));
	BoxCollision->bApplyImpulseOnDamage = false;
	BoxCollision->SetGenerateOverlapEvents(true);
	BoxCollision->CanCharacterStepUp(nullptr);
	BoxCollision->SetCollisionEnabled(ECollisionEnabled::QueryOnly);

	MuzzleFlash->SetAutoActivate(false);

	FireSound->SetAutoActivate(false);

	Widget->SetWidgetSpace(EWidgetSpace::Screen);
	Widget->SetGenerateOverlapEvents(false);
	Widget->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	Widget->SetVisibility(false);

	// Component Overlap
	BoxCollision->OnComponentBeginOverlap.AddDynamic(this, &APickup_Weapon::OnBoxBeginOverlap);
	BoxCollision->OnComponentEndOverlap.AddDynamic(this, &APickup_Weapon::OnBoxEndOverlap);

	// Set value defaults
	bDoOnceFire = true;
	bCanFire = true;
}

void APickup_Weapon::BeginPlay()
{
	Super::BeginPlay();
	
	AmmoComponent->Initialize();
	MuzzleFlash->SetRelativeScale3D(MuzzleFlashScale);

	// Set weapon info on widget to show it when the player overlap with weapon
	// C++ only
	/*IWidgetInterface* Interface = Cast<IWidgetInterface>(Widget->GetWidget());
	if(Interface)
	{
		Interface->SetWeaponInfo(WeaponInfo);
	}*/
	// C++ and blueprint
	if(Widget->GetWidget()->GetClass()->ImplementsInterface(UWidgetInterface::StaticClass()))
	{
		IWidgetInterface::Execute_SetWeaponInfo(Widget->GetWidget(), WeaponInfo);
	}
	
	if(Projectile.Num() > 0)
	{
		CurrentProjectile = Projectile[0].GetDefaultObject();
	}
}

void APickup_Weapon::StartFireWeapon()
{
	if(bCanFire)
	{
		if(bDoOnceFire)
		{
			bDoOnceFire = false;
			FireWeapon();
			
			if(bOwnerIsAI || WeaponInfo.bIsAutomatic)
			{
				GetWorldTimerManager().SetTimer(FireWeaponTimer, this, &APickup_Weapon::FireWeapon, WeaponInfo.TimeBetweenShots, true);
			}
			FTimerHandle ResetDoOnceTimer;
			GetWorldTimerManager().SetTimer(ResetDoOnceTimer, this, &APickup_Weapon::CoolDownDelay, WeaponInfo.CoolDownTime);
		}
	}
	else
	{
		StopFireWeapon();
		if(AmmoComponent->CurrentMagazineAmmo <= 0 && !AmmoComponent->NoAmmoLeftToReload())
		{
			SetWeaponState_Implementation(EWeaponState::NeedToReload);
		}
		else
		{
			if(AmmoComponent->NoAmmoLeftToReload())
			{
				SetWeaponState_Implementation(EWeaponState::Empty);
			}
		}
	}
}

void APickup_Weapon::FireWeapon()
{
	AmmoComponent->ReduceAmmo();
	WeaponFireEffect();
	UAISense_Hearing::ReportNoiseEvent(GetWorld(), GetActorLocation(), 1.0f, GetOwner(), 0.0f, TEXT("Weapon"));
	SpawnProjectile();

	// Play camera shake if owner is player
	// C++
	if(!bOwnerIsAI && PlayerControllerInterface && CameraShake)
	{
		PlayerControllerInterface->Execute_PlayCameraShake(OwnerController, CameraShake);
	}
	// C++ and blueprint
	if (OwnerController->GetClass()->ImplementsInterface(UPlayerControllerInterface::StaticClass()) && CameraShake)
	{
		PlayerControllerInterface->Execute_PlayCameraShake(OwnerController, CameraShake);
	}
	
	FTimerHandle ResetAnimationTimer;
	GetWorldTimerManager().SetTimer(ResetAnimationTimer, this, &APickup_Weapon::ResetAnimationDelay, WeaponInfo.TimeBetweenShots / 2.0f);

	// Spawn Empty shell after each weapon fire
	// TODO improve empty shell spawn based on current ammo type
	if(EmptyShell.Num() > 0)
	{
		const FVector Location =  SkeletalMesh->GetSocketLocation(TEXT("EjectorSocket"));
		const FRotator Rotation = SkeletalMesh->GetSocketRotation(TEXT("EjectorSocket"));
		FActorSpawnParameters ActorSpawnParameters;
		ActorSpawnParameters.Owner = this;
		ActorSpawnParameters.Instigator = GetInstigator();
		ActorSpawnParameters.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
		GetWorld()->SpawnActor<AEmptyShell>(EmptyShell[0], Location, Rotation, ActorSpawnParameters);
	}
}

void APickup_Weapon::StopFireWeapon()
{
	GetWorldTimerManager().ClearTimer(FireWeaponTimer);
	// Play an empty animation to reset bones position
	SkeletalMesh->PlayAnimation(nullptr, false);
}

// Play weapon fire sound and muzzle emitter by activate them and play weapon fire animation
void APickup_Weapon::WeaponFireEffect() const
{
	FireSound->Activate(true);
	MuzzleFlash->Activate(true);
	SkeletalMesh->PlayAnimation(SkeletalMesh->AnimationData.AnimToPlay, false);
}

void APickup_Weapon::SpawnProjectile()
{
	if(CurrentProjectile)
	{
		for(int i = 0; i < CurrentProjectile->NumberOfPellets; i++)
		{
			FVector Location;
			FRotator Rotation;
			FActorSpawnParameters ActorSpawnParameters;

			ProjectileLineTrace(Location, Rotation);
			ActorSpawnParameters.Owner = this;
			ActorSpawnParameters.Instigator = GetInstigator();
			ActorSpawnParameters.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
			// Spawn projectile
			GetWorld()->SpawnActor<AProjectile>(Projectile[0], Location, Rotation, ActorSpawnParameters);
		}
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("No projectile to spawn!"))
	}
}

void APickup_Weapon::ProjectileLineTrace(FVector& OutLocation, FRotator& OutRotation)
{
	FHitResult HitResult;
	FVector Start;
	FVector End;
	FCollisionQueryParams CollisionQueryParams;
	CalculateLineTrace(Start, End);
	CollisionQueryParams.bTraceComplex = true;
	CollisionQueryParams.AddIgnoredActors(IgnoredActorsByTrace);
	const bool bHit = GetWorld()->LineTraceSingleByChannel(HitResult, Start, End, ECC_Visibility, CollisionQueryParams);

	// Draw debug line and box
	if(bDrawDebugLineTrace)
	{
		DrawDebugLine(GetWorld(), Start, End, FColor::Blue, false, 2.0f);
		if(bHit)
		{
			DrawDebugBox(GetWorld(), HitResult.ImpactPoint, FVector(10, 10, 10), FColor::Red, false, 2.0f);
		}
	}
	
	const FVector MuzzleLocation = SkeletalMesh->GetSocketLocation(TEXT("MuzzleFlashSocket"));
	OutLocation = MuzzleLocation;
	bHit ? OutRotation = UKismetMathLibrary::FindLookAtRotation(MuzzleLocation, HitResult.ImpactPoint) : OutRotation = UKismetMathLibrary::FindLookAtRotation(MuzzleLocation, HitResult.TraceEnd);
}

// Calculate line trace start and end points
void APickup_Weapon::CalculateLineTrace(FVector& OutStart, FVector& OutEnd)
{
	FVector TraceStart;
	FVector UpVector;
	FVector RightVector;
	FVector TraceEnd;
	
	if(bOwnerIsAI)
	{
		const FRotator SocketRotation = SkeletalMesh->GetSocketRotation(TEXT("MuzzleFlashSocket"));
		TraceStart = SkeletalMesh->GetSocketLocation(TEXT("MuzzleFlashSocket"));
		UpVector = UKismetMathLibrary::GetUpVector(SocketRotation);
		RightVector = UKismetMathLibrary::GetRightVector(SocketRotation);
		const FRotator Points = RandomPointInCircle(FMath::FRandRange(WeaponInfo.MinFireOffset, WeaponInfo.MaxFireOffset) * WeaponInfo.WeaponSpreadCurve->GetFloatValue(FMath::FRand()), true);
		TraceEnd = Points.RotateVector(UKismetMathLibrary::GetForwardVector(SocketRotation));
	}
	else
	{
		TraceStart = CameraComponent->GetComponentLocation();
		RightVector = CameraComponent->GetRightVector();
		UpVector = CameraComponent->GetUpVector();
		// TODO replace random point in circle for weapon spread with add controller pitch
		const FRotator Points = RandomPointInCircle(WeaponInfo.WeaponSpreadCurve->GetFloatValue(FMath::FRand()), false);
		TraceEnd = Points.RotateVector(CameraComponent->GetForwardVector());
	}

	if(CurrentProjectile)
	{
		if(CurrentProjectile->NumberOfPellets > 1)
		{
			const FRotator Points = RandomPointInCircle(FMath::FRandRange(CurrentProjectile->PelletSpread * -1.0f, CurrentProjectile->PelletSpread), true);
			OutStart = TraceStart;
			OutEnd = (TraceStart + (TraceEnd * WeaponInfo.Range)) + (RightVector * Points.Roll) + (UpVector * Points.Pitch);
		}
		else
		{
			OutStart = TraceStart;
			OutEnd = TraceStart + (TraceEnd * WeaponInfo.Range);
		}
	}
}

FRotator APickup_Weapon::RandomPointInCircle(const float Radius, const bool bIncludesNegative) const
{
	// Distance From Center can be a random value from 0 to Radius or just Radius
	float DistanceFromCenter;
	// DistanceFromCenter = FMath::FRandRange(0.0f, Radius); // Option 1
	DistanceFromCenter = Radius; // Option 2
	const float Angle = FMath::FRandRange(0.0f, 360.0f);

	FRotator Points;
	if(bIncludesNegative)
	{
		Points.Roll = DistanceFromCenter * UKismetMathLibrary::DegCos(Angle);
		Points.Pitch = DistanceFromCenter * UKismetMathLibrary::DegSin(Angle);
		Points.Yaw = 0.0f;
	}
	else
	{
		Points.Roll = abs(DistanceFromCenter * UKismetMathLibrary::DegCos(Angle));
		Points.Pitch = abs(DistanceFromCenter * UKismetMathLibrary::DegSin(Angle));
		Points.Yaw = 0.0f;
	}
	return Points;
}

void APickup_Weapon::RaiseWeapon() const
{
	UGameplayStatics::SpawnSoundAttached(RaiseSound, SkeletalMesh, TEXT("Root"), FVector::ZeroVector, FRotator::ZeroRotator, EAttachLocation::SnapToTarget, true);

	const FAmmoComponentInfo AmmoComponentInfo = AmmoComponent->GetAmmoComponentInfo();

	// Report weapon state to owner on pickup
	if(bOwnerIsAI)
	{
		// C++ only
		/*IAIControllerInterface* Interface = Cast<IAIControllerInterface>(GetOwner());
		if(Interface)
		{
			if(AmmoComponent->BetterToReload())
			{
				Interface->SetWeaponState(AmmoComponentInfo ,EWeaponState::BetterToReload);
			}
			else
			{
				Interface->SetWeaponState(AmmoComponentInfo, EWeaponState::Idle);
			}
		}*/
		// C++ and blueprint
		if(OwnerController->GetClass()->ImplementsInterface(UAIControllerInterface::StaticClass()))
		{
			if(AmmoComponent->BetterToReload())
			{
				IAIControllerInterface::Execute_SetWeaponState(OwnerController, AmmoComponentInfo, EWeaponState::BetterToReload);
			}
			else
			{
				IAIControllerInterface::Execute_SetWeaponState(OwnerController, AmmoComponentInfo, EWeaponState::Idle);
			}
		}
	}
	else
	{
		// C++ only
		/*IPlayerControllerInterface* Interface = Cast<IPlayerControllerInterface>(GetOwner());
		if(Interface)
		{
			if(AmmoComponent->BetterToReload())
			{
				Interface->SetWeaponState(AmmoComponentInfo, EWeaponState::BetterToReload);
			}
			else
			{
				Interface->SetWeaponState(AmmoComponentInfo, EWeaponState::Idle);
			}
		}*/
		// C++ and blueprint
		if(OwnerController->GetClass()->ImplementsInterface(UPlayerControllerInterface::StaticClass()))
		{
			if(AmmoComponent->BetterToReload())
			{
				IPlayerControllerInterface::Execute_SetWeaponState(OwnerController, AmmoComponentInfo, EWeaponState::BetterToReload);
			}
			else
			{
				IPlayerControllerInterface::Execute_SetWeaponState(OwnerController, AmmoComponentInfo, EWeaponState::Idle);
			}
		}
	}
}

void APickup_Weapon::LowerWeapon() const
{
	UGameplayStatics::SpawnSoundAttached(LowerSound, SkeletalMesh, TEXT("Root"), FVector::ZeroVector, FRotator::ZeroRotator, EAttachLocation::SnapToTarget, true);
}

void APickup_Weapon::SetMagazineVisibility(const bool bVisible) const
{
	if(bVisible)
	{
		SkeletalMesh->UnHideBoneByName(MagazineBoneName);
	}
	else
	{
		SkeletalMesh->HideBoneByName(MagazineBoneName, EPhysBodyOp::PBO_None);
	}
}

void APickup_Weapon::ReloadWeapon() const
{
	AmmoComponent->Reload();
	UGameplayStatics::PlaySoundAtLocation(GetWorld(), ReloadSound, GetActorLocation());
}

bool APickup_Weapon::CanPickupAmmo() const
{
	int32 CurrentAmmo, MagazineSize, CurrentMagazineAmmo;
	AmmoComponent->GetAmmoInfo(CurrentAmmo, MagazineSize, CurrentMagazineAmmo);

	if(CurrentAmmo < AmmoComponent->MaxAmmo)
	{
		return true;
	}
	return false;
}

FVector APickup_Weapon::GetLeftHandLocation() const
{
	// Location use to adjust character left hand with IK in animation blueprint
	return SkeletalMesh->GetSocketLocation(TEXT("LeftHandSocket"));
}

FVector APickup_Weapon::GetLeftHandAimLocation() const
{
	// Location use to adjust character left hand with IK in animation blueprint
	return SkeletalMesh->GetSocketLocation(TEXT("LeftHandAimSocket"));
}

// Overlap functions
void APickup_Weapon::OnBoxBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	// C++ only
	/*ICharacterInterface* Interface = Cast<ICharacterInterface>(OtherActor);
	if(Interface)
	{
		Interface->SetPickup(EItemType::Weapon, this);
	}*/
	// C++ and blueprint
	if(OtherActor->GetClass()->ImplementsInterface(UCharacterInterface::StaticClass()))
	{
		ICharacterInterface::Execute_SetPickup(OtherActor, EItemType::Weapon, this);
	}
	
	// If OtherActor is the player then show the widget
	if(OtherActor == UGameplayStatics::GetPlayerPawn(GetWorld(), 0))
	{
		Widget->SetVisibility(true);
		SkeletalMesh->SetRenderCustomDepth(true);
	}
}

void APickup_Weapon::OnBoxEndOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	// C++ only
	/*ICharacterInterface* Interface = Cast<ICharacterInterface>(OtherActor);
	if(Interface)
	{
		Interface->SetPickup(EItemType::Weapon, nullptr);
	}*/
	// C++ and blueprint
	if(OtherActor->GetClass()->ImplementsInterface(UCharacterInterface::StaticClass()))
	{
		ICharacterInterface::Execute_SetPickup(OtherActor, EItemType::Weapon, nullptr);
	}

	// If OtherActor is the player then hide the widget
	if(OtherActor == UGameplayStatics::GetPlayerPawn(GetWorld(), 0))
	{
		Widget->SetVisibility(false);
		SkeletalMesh->SetRenderCustomDepth(false);
	}
}
// End of overlap functions

// Interfaces
void APickup_Weapon::SetPickupStatus_Implementation(EPickupState PickupState)
{
	switch(PickupState)
	{
	case 0:
		// Drop
		SetOwner(nullptr);
		BoxCollision->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
		SkeletalMesh->SetCollisionProfileName(TEXT("Ragdoll"), false);
		SetLifeSpan(FMath::FRandRange(30.0f, 60.0f));
		break;
	case 1:
		// Pickup
		BoxCollision->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		SkeletalMesh->SetCollisionProfileName(TEXT("NoCollision"), false);
		SkeletalMesh->SetRelativeLocationAndRotation(FVector::ZeroVector, FRotator::ZeroRotator, false, nullptr, ETeleportType::TeleportPhysics);
		SetLifeSpan(0.0f);
		bOwnerIsAI = GetOwner()->ActorHasTag(TEXT("AI"));
		IgnoredActorsByTrace.Empty();
		IgnoredActorsByTrace.Add(this);
		IgnoredActorsByTrace.Add(GetOwner());
		
		if(PickupOwner)
		{
			OwnerController = Cast<AController>(PickupOwner->GetController());
			if(bOwnerIsAI)
			{
				AIControllerInterface = Cast<IAIControllerInterface>(OwnerController);
			}
			else
			{
				PlayerControllerInterface = Cast<IPlayerControllerInterface>(OwnerController);
				if(PlayerControllerInterface)
				{
					GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, FString::Printf(TEXT("PlayerControllerInterface")));
				}
			}
		}
		break;
	case 2:
		// Remove
		Destroy();
		break;
	}
}

void APickup_Weapon::SetCanFire_Implementation(const bool bInCanFire)
{
	bCanFire = bInCanFire;
}

void APickup_Weapon::SetWeaponState_Implementation(EWeaponState WeaponState)
{
	const FAmmoComponentInfo AmmoComponentInfo = AmmoComponent->GetAmmoComponentInfo();
	// For C++
	/*if(bOwnerIsAI && AIControllerInterface)
	{
		AIControllerInterface->SetWeaponState(AmmoComponentInfo, WeaponState);
	}
	else if(!bOwnerIsAI && PlayerControllerInterface)
	{
		PlayerControllerInterface->SetWeaponState(AmmoComponentInfo, WeaponState);
	}*/
	// For C++ and blueprint
	if(bOwnerIsAI && OwnerController->GetClass()->ImplementsInterface(UAIControllerInterface::StaticClass()))
	{
		AIControllerInterface->Execute_SetWeaponState(OwnerController, AmmoComponentInfo, WeaponState);
	}
	else if(!bOwnerIsAI && OwnerController->GetClass()->ImplementsInterface(UPlayerControllerInterface::StaticClass()))
	{
		PlayerControllerInterface->Execute_SetWeaponState(OwnerController, AmmoComponentInfo, WeaponState);
	}
	
	switch(WeaponState)
	{
	case 0:
		// Idle
		break;
	case 1:
		// Firing
		break;
	case 2:
		// Better To Reload
		break;
	case 3:
		// Need To Reload
		bCanFire = false;
		StopFireWeapon();
		break;
	case 4:
		// Reloading
		bCanFire = false;
		StopFireWeapon();
		break;
	case 5:
		// Cancel Reload
		AmmoComponent->CurrentMagazineAmmo > 0 ? bCanFire = true : bCanFire = false;
		break;
	case 6:
		// Reloaded
		bCanFire = true;
		break;
	case 7:
		// Ammo Added
		break;
	case 8:
		// Empty
		bCanFire = false;
		StopFireWeapon();
		break;
	case 9:
		// Overheat
		bCanFire = false;
		StopFireWeapon();
		break;
	}
}

APickup_Weapon* APickup_Weapon::GetWeaponReference_Implementation()
{
	return this;
}
// End Of interfaces

//Delays
void APickup_Weapon::CoolDownDelay()
{
	bDoOnceFire = true;
}

void APickup_Weapon::ResetAnimationDelay() const
{
	SkeletalMesh->PlayAnimation(nullptr, false);
}

// GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, FString::Printf(TEXT("Overlap with %s"), *OtherActor->GetName()));