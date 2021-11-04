

#include "Pickups/Pickup_Weapon.h"
#include "Particles/ParticleSystemComponent.h"
#include "Components/AudioComponent.h"
#include "Components/BoxComponent.h"
#include "Components/WidgetComponent.h"
#include "Interfaces/CharacterInterface.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
APickup_Weapon::APickup_Weapon()
{
	SkeletalMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Skeletal Mesh"));
	MuzzleFlash = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("Muzzle Flash"));
	FireSound = CreateDefaultSubobject<UAudioComponent>(TEXT("Fire Sound"));
	BoxCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("Box Collision"));
	Widget = CreateDefaultSubobject<UWidgetComponent>(TEXT("Widget"));

	// Setup Attachment
	SkeletalMesh->SetupAttachment(Scene);
	MuzzleFlash->SetupAttachment(SkeletalMesh);
	FireSound->SetupAttachment(SkeletalMesh);
	BoxCollision->SetupAttachment(SkeletalMesh);
	Widget->SetupAttachment(SkeletalMesh);

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

// Components overlap functions
void APickup_Weapon::OnBoxBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
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
		break;
 
	case 1:
		// Pickup
		break;

	case 2:
		// Remove
		break;
	}
}
// End Of interfaces