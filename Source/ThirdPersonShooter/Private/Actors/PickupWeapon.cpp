// All Rights Reserved.

#include "Actors/PickupWeapon.h"
#include "Kismet/GameplayStatics.h"
#include "Math/UnrealMathUtility.h"
#include "Kismet/KismetMathLibrary.h"
#include "DrawDebugHelpers.h"
#include "AIController.h"
#include "Perception/AISense_Hearing.h"
#include "Sound/SoundCue.h"
#include "Actors/EmptyShell.h"
#include "Actors/Projectile.h"
#include "Components/BoxComponent.h"
#include "Particles/ParticleSystemComponent.h"
#include "Components/AudioComponent.h"
#include "Components/WidgetComponent.h"
#include "Components/AmmoComponent.h"
#include "Camera/CameraComponent.h"
#include "Interfaces/CharacterInterface.h"
#include "Interfaces/AIControllerInterface.h"
#include "Interfaces/PlayerControllerInterface.h"
#include "Interfaces/WidgetInterface.h"
#include "Structs/AmmoComponentInfoStruct.h"

// Sets default values
APickupWeapon::APickupWeapon()
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
	BoxCollision->OnComponentBeginOverlap.AddDynamic(this, &APickupWeapon::OnBoxBeginOverlap);
	BoxCollision->OnComponentEndOverlap.AddDynamic(this, &APickupWeapon::OnBoxEndOverlap);

	// Initialize variables
	PickupType = EItemType::Weapon;
	bDoOnceFire = true;
	bCanFire = true;
}

void APickupWeapon::BeginPlay()
{
	Super::BeginPlay();
	
	AmmoComponent->Initialize();
	MuzzleFlash->SetRelativeScale3D(WeaponDefaults.MuzzleFlashScale);

	// Set weapon info on widget to show it when the player overlap with weapon
	if(Widget->GetWidget()->GetClass()->ImplementsInterface(UWidgetInterface::StaticClass()))
	{
		IWidgetInterface::Execute_SetWeaponInfo(Widget->GetWidget(), WeaponInfo);
	}
	
	if(WeaponDefaults.Projectile.Num() > 0)
	{
		CurrentProjectile = WeaponDefaults.Projectile[0].GetDefaultObject();
	}
}

void APickupWeapon::StartFireWeapon()
{
	if(bCanFire)
	{
		if(bDoOnceFire)
		{
			bDoOnceFire = false;
			FireWeapon();
			
			if(bOwnerIsAI || WeaponInfo.bIsAutomatic)
			{
				GetWorld()->GetTimerManager().SetTimer(FireWeaponTimer, this, &APickupWeapon::FireWeapon, WeaponInfo.TimeBetweenShots, true);
			}
			FTimerHandle ResetDoOnceTimer;
			GetWorld()->GetTimerManager().SetTimer(ResetDoOnceTimer, this, &APickupWeapon::CoolDownDelay, WeaponInfo.CoolDownTime);
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

void APickupWeapon::FireWeapon()
{
	AmmoComponent->ReduceAmmo();
	WeaponFireEffect();
	UAISense_Hearing::ReportNoiseEvent(GetWorld(), GetActorLocation(), 1.0f, GetOwner(), 0.0f, TEXT("Weapon"));
	SpawnProjectile();

	// Add recoil to character animation and player UI crosshair
	if(bCharacterInterface)
	{
		ICharacterInterface::Execute_AddRecoil(Owner, WeaponDefaults.RotationIntensity, WeaponDefaults.ControlTime, WeaponDefaults.CrosshairRecoil, WeaponDefaults.ControllerPitch);
	}

	if (bPlayerControllerInterface && WeaponDefaults.CameraShake)
	{
		IPlayerControllerInterface::Execute_PlayCameraShake(OwnerController, WeaponDefaults.CameraShake);
	}
	
	FTimerHandle ResetAnimationTimer;
	GetWorldTimerManager().SetTimer(ResetAnimationTimer, this, &APickupWeapon::ResetAnimationDelay, WeaponInfo.TimeBetweenShots / 2.0f);

	// Spawn Empty shell
	if(WeaponDefaults.EmptyShell.Num() > 0)
	{
		const FVector Location =  SkeletalMesh->GetSocketLocation(TEXT("EjectorSocket"));
		const FRotator Rotation = SkeletalMesh->GetSocketRotation(TEXT("EjectorSocket"));
		FActorSpawnParameters ActorSpawnParameters;
		ActorSpawnParameters.Owner = this;
		ActorSpawnParameters.Instigator = GetInstigator();
		ActorSpawnParameters.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
		GetWorld()->SpawnActor<AEmptyShell>(WeaponDefaults.EmptyShell[0], Location, Rotation, ActorSpawnParameters);
	}
}

void APickupWeapon::StopFireWeapon()
{
	GetWorld()->GetTimerManager().ClearTimer(FireWeaponTimer);
	// Play an empty animation to reset bones position
	SkeletalMesh->PlayAnimation(nullptr, false);
}

// Play weapon fire sound and muzzle emitter by activate them and play weapon fire animation
void APickupWeapon::WeaponFireEffect() const
{
	FireSound->Activate(true);
	MuzzleFlash->Activate(true);
	SkeletalMesh->PlayAnimation(SkeletalMesh->AnimationData.AnimToPlay, false);
}

void APickupWeapon::SpawnProjectile()
{
	if(CurrentProjectile)
	{
		for(int i = 0; i < CurrentProjectile->NumberOfPellets; i++)
		{
			FVector Location;
			FRotator Rotation;

			ProjectileLineTrace(Location, Rotation);
			
			FActorSpawnParameters ActorSpawnParameters;
			ActorSpawnParameters.Owner = this;
			ActorSpawnParameters.Instigator = GetInstigator();
			ActorSpawnParameters.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
			// Spawn projectile
			GetWorld()->SpawnActor<AProjectile>(WeaponDefaults.Projectile[0], Location, Rotation, ActorSpawnParameters);
		}
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("No projectile to spawn!"))
	}
}

void APickupWeapon::ProjectileLineTrace(FVector& OutLocation, FRotator& OutRotation)
{
	FHitResult HitResult;
	FVector Start;
	FVector End;
	
	CalculateLineTrace(Start, End);
	
	FCollisionQueryParams CollisionQueryParams;
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
void APickupWeapon::CalculateLineTrace(FVector& OutStart, FVector& OutEnd)
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

FRotator APickupWeapon::RandomPointInCircle(const float Radius, const bool bIncludesNegative) const
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

void APickupWeapon::RaiseWeapon() const
{
	UGameplayStatics::SpawnSoundAttached(WeaponDefaults.RaiseSound, SkeletalMesh, TEXT("root"), FVector::ZeroVector, FRotator::ZeroRotator, EAttachLocation::SnapToTarget, true);

	const FAmmoComponentInfo AmmoComponentInfo = AmmoComponent->GetAmmoComponentInfo();

	// Report weapon state to owner on pickup
	if(bAIControllerInterface)
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
	else if(bPlayerControllerInterface)
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

void APickupWeapon::LowerWeapon() const
{
	UGameplayStatics::SpawnSoundAttached(WeaponDefaults.LowerSound, SkeletalMesh, TEXT("Root"), FVector::ZeroVector, FRotator::ZeroRotator, EAttachLocation::SnapToTarget, true);
}

void APickupWeapon::SetMagazineVisibility(const bool bVisible) const
{
	if(bVisible)
	{
		SkeletalMesh->UnHideBoneByName(WeaponDefaults.MagazineBoneName);
	}
	else
	{
		SkeletalMesh->HideBoneByName(WeaponDefaults.MagazineBoneName, EPhysBodyOp::PBO_None);
	}
}

void APickupWeapon::ReloadWeapon() const
{
	AmmoComponent->Reload();
	UGameplayStatics::PlaySoundAtLocation(GetWorld(), WeaponDefaults.ReloadSound, GetActorLocation());
}

bool APickupWeapon::CanPickupAmmo() const
{
	const int32 CurrentAmmo = AmmoComponent->CurrentAmmo;
	if(CurrentAmmo < AmmoComponent->MaxAmmo)
	{
		return true;
	}
	return false;
}

FVector APickupWeapon::GetLeftHandLocation() const
{
	// Location use to adjust character left hand with IK in animation blueprint
	return SkeletalMesh->GetSocketLocation(TEXT("LeftHandSocket"));
}

FVector APickupWeapon::GetLeftHandAimLocation() const
{
	// Location use to adjust character left hand with IK in animation blueprint
	return SkeletalMesh->GetSocketLocation(TEXT("LeftHandAimSocket"));
}

// Interfaces
void APickupWeapon::SetPickupStatus(const EPickupState PickupState)
{
	switch(PickupState)
	{
	case 0:
		// Drop
		SetOwner(nullptr);
		BoxCollision->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
		SkeletalMesh->SetCollisionProfileName(TEXT("Ragdoll"), false);
		SkeletalMesh->SetCollisionObjectType(ECC_PhysicsBody);
		SetLifeSpan(FMath::FRandRange(30.0f, 60.0f));
		break;
	case 1:
		// Pickup
		BoxCollision->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		SkeletalMesh->SetCollisionProfileName(TEXT("NoCollision"), false);
		SkeletalMesh->SetRelativeLocationAndRotation(FVector::ZeroVector, FRotator::ZeroRotator, false, nullptr, ETeleportType::TeleportPhysics);
		SetLifeSpan(0.0f);

		// Ignore self and owner for line trace
		IgnoredActorsByTrace.Empty();
		IgnoredActorsByTrace.Add(this);
		IgnoredActorsByTrace.Add(GetOwner());
		
		// Determine if owner is player or AI controlled
		if (Cast<APlayerController>(Owner->GetInstigatorController()))
		{
			bOwnerIsAI = false;
		}
		else
		{
			bOwnerIsAI = true;
		}
		
		// Detected if the interfaces is present on owner
		if(Owner)
		{
			bCharacterInterface = false;
			bPlayerControllerInterface = false;
			bAIControllerInterface = false;

			if(Owner->GetClass()->ImplementsInterface(UCharacterInterface::StaticClass()))
			{
				bCharacterInterface = true;
			}

			OwnerController = Cast<AController>(Owner->GetInstigatorController());
			if(OwnerController->GetClass()->ImplementsInterface(UAIControllerInterface::StaticClass()))
			{
				bAIControllerInterface = true;
			}
			else if(OwnerController->GetClass()->ImplementsInterface(UPlayerControllerInterface::StaticClass()))
			{
				bPlayerControllerInterface = true;
			}
		}
		break;
	case 2:
		// Remove
		Destroy();
		break;
	}
}

void APickupWeapon::SetCanFire_Implementation(const bool bInCanFire)
{
	bCanFire = bInCanFire;
}

void APickupWeapon::SetWeaponState_Implementation(const EWeaponState WeaponState)
{
	const FAmmoComponentInfo AmmoComponentInfo = AmmoComponent->GetAmmoComponentInfo();
	
	if(bAIControllerInterface)
	{
		IAIControllerInterface::Execute_SetWeaponState(OwnerController, AmmoComponentInfo, WeaponState);
	}
	else if(bPlayerControllerInterface)
	{
		IPlayerControllerInterface::Execute_SetWeaponState(OwnerController, AmmoComponentInfo, WeaponState);
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
		AmmoComponent->CurrentMagazineAmmo > 0 ? bCanFire = true : bCanFire = false;
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

APickupWeapon* APickupWeapon::GetWeaponReference_Implementation()
{
	return this;
}
// End Of interfaces

// Overlap functions
void APickupWeapon::OnBoxBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if(OtherActor->GetClass()->ImplementsInterface(UCharacterInterface::StaticClass()))
	{
		ICharacterInterface::Execute_SetPickup(OtherActor, EItemType::Weapon, this);
	}
	
	// If Other Actor is the player then show the widget
	if(OtherActor == UGameplayStatics::GetPlayerPawn(GetWorld(), 0))
	{
		Widget->SetVisibility(true);
		SkeletalMesh->SetRenderCustomDepth(true);
	}
}

void APickupWeapon::OnBoxEndOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if(OtherActor->GetClass()->ImplementsInterface(UCharacterInterface::StaticClass()))
	{
		ICharacterInterface::Execute_SetPickup(OtherActor, EItemType::Weapon, nullptr);
	}

	// If Other Actor is the player then hide the widget
	if(OtherActor == UGameplayStatics::GetPlayerPawn(GetWorld(), 0))
	{
		Widget->SetVisibility(false);
		SkeletalMesh->SetRenderCustomDepth(false);
	}
}
// End of overlap functions

//Delays
void APickupWeapon::CoolDownDelay()
{
	bDoOnceFire = true;
}

void APickupWeapon::ResetAnimationDelay() const
{
	SkeletalMesh->PlayAnimation(nullptr, false);
}