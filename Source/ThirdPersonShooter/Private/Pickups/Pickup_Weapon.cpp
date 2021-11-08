
#include "Pickups/Pickup_Weapon.h"
#include "Kismet/GameplayStatics.h"
#include "Math/UnrealMathUtility.h"
#include "AIController.h"
#include "Perception/AISense_Hearing.h"
#include "Sound/SoundCue.h"
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
	SkeletalMesh->SetComponentTickEnabled(false);
	SkeletalMesh->bApplyImpulseOnDamage = false;
	SkeletalMesh->CanCharacterStepUp(nullptr);
	SkeletalMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	BoxCollision->SetBoxExtent(FVector (8.0f, 50.0f, 20.0f));
	BoxCollision->SetComponentTickEnabled(false);
	BoxCollision->bApplyImpulseOnDamage = false;
	BoxCollision->SetGenerateOverlapEvents(true);
	BoxCollision->CanCharacterStepUp(nullptr);
	BoxCollision->SetCollisionEnabled(ECollisionEnabled::QueryOnly);

	MuzzleFlash->SetComponentTickEnabled(false);
	MuzzleFlash->SetAutoActivate(false);

	FireSound->SetComponentTickEnabled(false);
	FireSound->SetAutoActivate(false);

	Widget->SetWidgetSpace(EWidgetSpace::Screen);
	Widget->SetComponentTickEnabled(false);
	Widget->SetGenerateOverlapEvents(false);
	Widget->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	Widget->SetVisibility(false);

	// Component Overlap
	BoxCollision->OnComponentBeginOverlap.AddDynamic(this, &APickup_Weapon::OnBoxBeginOverlap);
	BoxCollision->OnComponentEndOverlap.AddDynamic(this, &APickup_Weapon::OnBoxEndOverlap);

	// Set value defaults
	bDoOnceFire = true;
	bDoOnceWidget = true;
}


void APickup_Weapon::BeginPlay()
{
	Super::BeginPlay();
	
	SkeletalMesh->SetCollisionProfileName(TEXT("Ragdoll"), false);
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

	if(EmptyShell.Num() > 0)
	{
	// TODO fix and improve spawn actor
		FVector Location =  SkeletalMesh->GetSocketLocation(TEXT("EjectorSocket"));
		FRotator Rotation = SkeletalMesh->GetSocketRotation(TEXT("EjectorSocket"));
		// FActorSpawnParameters ActorSpawnParameters;
		// GetWorld()->SpawnActor<EmptyShell[0]>(Location, Rotation, ActorSpawnParameters);
	}
}

void APickup_Weapon::StopFireWeapon()
{
	GetWorldTimerManager().ClearTimer(FireWeaponTimer);
	// Play an empty animation to reset bones position
	SkeletalMesh->PlayAnimation(nullptr, false);
}

// Play weapon fire sound and muzzle emitter by activate them and play weapon fire animation
void APickup_Weapon::WeaponFireEffect()
{
	
}

void APickup_Weapon::SpawnProjectile()
{
	
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