
#include "Pickups/Pickup_Weapon.h"
#include "AIController.h"
#include "Particles/ParticleSystemComponent.h"
#include "Components/AudioComponent.h"
#include "Components/BoxComponent.h"
#include "Components/WidgetComponent.h"
#include "Components/AmmoComponent.h"
#include "Interfaces/CharacterInterface.h"
#include "Interfaces/AIControllerInterface.h"
#include "Interfaces/PlayerControllerInterface.h"
#include "Kismet/GameplayStatics.h"
#include "Math/UnrealMathUtility.h"
#include "Sound/SoundCue.h"

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
	bDoOnce = true;
}


void APickup_Weapon::BeginPlay()
{
	Super::BeginPlay();
	
	SkeletalMesh->SetCollisionProfileName(TEXT("Ragdoll"), false);
}

void APickup_Weapon::RaiseWeapon() const
{
	UGameplayStatics::SpawnSoundAttached(RaiseSound, SkeletalMesh, TEXT("Root"), FVector::ZeroVector, FRotator::ZeroRotator, EAttachLocation::SnapToTarget, true);

	if(bOwnerIsAI)
	{
		IAIControllerInterface* Interface = Cast<IAIControllerInterface>(GetOwner()); // TODO Get AI Controller
		if(Interface)
		{
			if(AmmoComponent->BetterToReload())
			{
				Interface->SetWeaponState(0 ,EWeaponState::BetterToReload);
			}
			else
			{
				Interface->SetWeaponState(0, EWeaponState::Idle);
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
				Interface->SetWeaponState(0, EWeaponState::BetterToReload);
			}
			else
			{
				Interface->SetWeaponState(0, EWeaponState::Idle);
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
	// if(PickupOwner)
	// {
	// 	OwnerController = Cast<AController>(PickupOwner->GetController());
	// }
	// GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, FString::Printf(TEXT("Overlap with %s"), *OtherActor->GetName()));
	if(OtherActor == UGameplayStatics::GetPlayerPawn(GetWorld(), 0))
	{
		ICharacterInterface* Interface = Cast<ICharacterInterface>(OtherActor);
		if(Interface)
		{
			Interface->SetPickup(EItemType::Weapon, this);
			// Interface->Execute_SetPickup(OtherActor, EItemType::Weapon, this); // For calling blueprint implementation
			
			if (bDoOnce)
			{
				// Widget->GetWidget(); // Weapon Info Var data should send to widget by a interface
				bDoOnce = false;
			}
			// If OtherActor is player
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
		if(bOwnerIsAI)
		{
			// OwnerAIController = Cast<AAIController>(GetOwner());
			// if(OwnerAIController)
			// {
			// 	AIControllerInterface = Cast<IAIControllerInterface>(OwnerAIController);
			// }
		}
		else
		{
			// OwnerController = Cast<AController>(GetOwner());
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
	if(bOwnerIsAI && AIControllerInterface)
	{
		AIControllerInterface->SetWeaponState(0, WeaponState);
	}
	else
	{
		
	}
}
// End Of interfaces
