
#include "Pickups/Pickup_Weapon.h"
#include "Kismet/GameplayStatics.h"
#include "Math/UnrealMathUtility.h"
#include "Kismet/KismetMathLibrary.h"
#include "DrawDebugHelpers.h"
#include "AIController.h"
#include "Perception/AISense_Hearing.h"
#include "Sound/SoundCue.h"
#include "Actors/EmptyShellActor.h"
#include "Actors/ProjectileActor.h"
// Interfaces
#include "Interfaces/CharacterInterface.h"
#include "Interfaces/AIControllerInterface.h"
#include "Interfaces/PlayerControllerInterface.h"
// Components
#include "Components/BoxComponent.h"
#include "Particles/ParticleSystemComponent.h"
#include "Components/AudioComponent.h"
#include "Components/WidgetComponent.h"
#include "Components/AmmoComponent.h"
// Structs
#include "Camera/CameraComponent.h"
#include "Structs/AmmoComponentInfoStruct.h"

// Sets default values
APickup_Weapon::APickup_Weapon()
{
	SkeletalMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Skeletal Mesh"));
	BoxCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("Box Collision"));
	MuzzleFlash = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("Muzzle Flash"));
	FireSound = CreateDefaultSubobject<UAudioComponent>(TEXT("Fire Sound"));
	Widget = CreateDefaultSubobject<UWidgetComponent>(TEXT("Widget"));
	AmmoComponent = CreateDefaultSubobject<UAmmoComponent>(TEXT("Ammo Component"));

	// Setup Attachment
	SkeletalMesh->SetupAttachment(Scene);
	BoxCollision->SetupAttachment(SkeletalMesh);
	MuzzleFlash->SetupAttachment(SkeletalMesh, TEXT("MuzzleFlashSocket"));
	FireSound->SetupAttachment(SkeletalMesh);
	Widget->SetupAttachment(SkeletalMesh);

	// Set component defaults
	SkeletalMesh->SetAnimationMode(EAnimationMode::AnimationSingleNode);
	SkeletalMesh->bApplyImpulseOnDamage = false;
	SkeletalMesh->CanCharacterStepUp(nullptr);
	SkeletalMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);

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
	bDoOnceWidget = true;
	IgnoredActorsByTrace.Add(this);
	IgnoredActorsByTrace.Add(GetOwner());
}


void APickup_Weapon::BeginPlay()
{
	Super::BeginPlay();
	
	SkeletalMesh->SetCollisionProfileName(TEXT("Ragdoll"), false);

	if(Projectile.Num() > 0)
	{
		CurrentProjectile = Cast<AProjectileActor>(Projectile[0]);
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
				GetWorldTimerManager().SetTimer(FireWeaponTimer,this, &APickup_Weapon::FireWeapon, WeaponInfo.TimeBetweenShots, true);
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

	if(!bOwnerIsAI && PlayerControllerInterface && CameraShake)
	{
		PlayerControllerInterface->PlayCameraShake(CameraShake);
	}
	FTimerHandle ResetAnimationTimer;
	GetWorldTimerManager().SetTimer(ResetAnimationTimer, this, &APickup_Weapon::ResetAnimationDelay, WeaponInfo.TimeBetweenShots / 2.0f);

	// Spawn Empty shell after each weapon fire
	// TODO improve empty shell spawn based on ammo current ammo type
	if(EmptyShell.Num() > 0)
	{
		const FVector Location =  SkeletalMesh->GetSocketLocation(TEXT("EjectorSocket"));
		const FRotator Rotation = SkeletalMesh->GetSocketRotation(TEXT("EjectorSocket"));
		FActorSpawnParameters ActorSpawnParameters;
		ActorSpawnParameters.Owner = this;
		ActorSpawnParameters.Instigator = GetInstigator();
		ActorSpawnParameters.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
		GetWorld()->SpawnActor<AEmptyShellActor>(EmptyShell[0], Location, Rotation, ActorSpawnParameters);
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
		for(int i = 1; i >= CurrentProjectile->NumberOfPellets; i++)
		{
			FVector Location;
			FRotator Rotation;
			FActorSpawnParameters ActorSpawnParameters;
		
			ProjectileLineTrace(Location, Rotation);
			ActorSpawnParameters.Owner = this;
			ActorSpawnParameters.Instigator = GetInstigator();
			ActorSpawnParameters.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
			// Spawn projectile
			GetWorld()->SpawnActor<AProjectileActor>(Projectile[0], Location, Rotation, ActorSpawnParameters);
		}
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
	if(bDrawLineTraceDebug)
	{
		DrawDebugLine(GetWorld(), Start, End, FColor::Blue, false, 2.0f);
		if(bHit)
		{
			DrawDebugBox(GetWorld(), HitResult.ImpactPoint, FVector(10, 10, 10), FColor::Green, false, 2.0f);
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
	
	if(bOwnerIsAI)
	{
		IAIControllerInterface* Interface = Cast<IAIControllerInterface>(GetOwner()); // TODO Get AI Controller
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
		}
	}
	else
	{
		IPlayerControllerInterface* Interface = Cast<IPlayerControllerInterface>(GetOwner());
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
		}
	}
}

void APickup_Weapon::LowerWeapon() const
{
	UGameplayStatics::SpawnSoundAttached(LowerSound, SkeletalMesh, TEXT("Root"), FVector::ZeroVector, FRotator::ZeroRotator, EAttachLocation::SnapToTarget, true);
}

// Components overlap functions
void APickup_Weapon::OnBoxBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
                                       UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	// If OtherActor is player
	if(OtherActor == UGameplayStatics::GetPlayerPawn(GetWorld(), 0))
	{
		ICharacterInterface* Interface = Cast<ICharacterInterface>(OtherActor);
		if(Interface)
		{
			Interface->SetPickup(EItemType::Weapon, this);
			// Interface->Execute_SetPickup(OtherActor, EItemType::Weapon, this); // For calling blueprint implementation
			
			if (bDoOnceWidget)
			{
				// TODO Widget->GetWidget(); // Weapon Info Var data should send to widget by a interface
				bDoOnceWidget = false;
			}
			Widget->SetVisibility(true);
			SkeletalMesh->SetRenderCustomDepth(true);
		}
	}
}

void APickup_Weapon::OnBoxEndOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
                                     UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if(OtherActor == UGameplayStatics::GetPlayerPawn(GetWorld(), 0))
	{
		ICharacterInterface* Interface = Cast<ICharacterInterface>(OtherActor);
		if(Interface)
		{
			Interface->SetPickup(EItemType::Weapon, nullptr);

			Widget->SetVisibility(false);
			SkeletalMesh->SetRenderCustomDepth(false);
		}
	}
}
// End of component overlap functions

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

	if(bOwnerIsAI && AIControllerInterface)
	{
		AIControllerInterface->SetWeaponState(AmmoComponentInfo, WeaponState);
	}
	else if(!bOwnerIsAI && PlayerControllerInterface)
	{
		PlayerControllerInterface->SetWeaponState(AmmoComponentInfo, WeaponState);
	}
	// TODO
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