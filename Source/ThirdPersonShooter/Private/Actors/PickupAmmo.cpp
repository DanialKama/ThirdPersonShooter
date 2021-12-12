// All Rights Reserved.

#include "Actors/PickupAmmo.h"
#include "Components/SphereComponent.h"
#include "Components/WidgetComponent.h"
#include "Sound/SoundCue.h"
#include "Kismet/GameplayStatics.h"
#include "Interfaces/CharacterInterface.h"
#include "Interfaces/WidgetInterface.h"

// Sets default values
APickupAmmo::APickupAmmo()
{
	// Create components
	StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	SphereCollision = CreateDefaultSubobject<USphereComponent>(TEXT("Collision"));
	Widget = CreateDefaultSubobject<UWidgetComponent>(TEXT("Widget"));

	// Setup components attachment
	SetRootComponent(StaticMesh);
	SphereCollision->SetupAttachment(StaticMesh);
	Widget->SetupAttachment(StaticMesh);

	// Set component defaults
	StaticMesh->SetSimulatePhysics(true);
	StaticMesh->bApplyImpulseOnDamage = false;
	StaticMesh->SetGenerateOverlapEvents(false);
	StaticMesh->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	StaticMesh->SetCollisionObjectType(ECC_PhysicsBody);

	SphereCollision->bApplyImpulseOnDamage = false;

	Widget->SetWidgetSpace(EWidgetSpace::Screen);
	Widget->SetGenerateOverlapEvents(false);
	Widget->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	Widget->SetVisibility(false);

	// Component Overlap
	SphereCollision->OnComponentBeginOverlap.AddDynamic(this, &APickupAmmo::OnBoxBeginOverlap);
	SphereCollision->OnComponentEndOverlap.AddDynamic(this, &APickupAmmo::OnBoxEndOverlap);

	// Initialize variables
	PickupType = EItemType::Ammo;
}

void APickupAmmo::BeginPlay()
{
	Super::BeginPlay();

	if(Widget->GetWidget()->GetClass()->ImplementsInterface(UWidgetInterface::StaticClass()))
	{
		IWidgetInterface::Execute_SetAmmoInfo(Widget->GetWidget(), AmmoType, Amount);
	}
}

// Interfaces
void APickupAmmo::SetPickupStatus(EPickupState PickupState)
{
	switch(PickupState)
	{
	case 0:
		// Drop
		SphereCollision->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
		break;
	case 1:
		// Pickup
		SphereCollision->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		break;
	case 2:
		// Remove
		UGameplayStatics::PlaySoundAtLocation(GetWorld(), PickupSound, GetActorLocation());
		Destroy();
		break;
	}
}

APickupAmmo* APickupAmmo::GetPickupAmmoReference_Implementation()
{
	UE_LOG(LogTemp, Warning, TEXT("REFERENCE..."))
	return this;
}
// End Of interfaces

// Overlap functions
void APickupAmmo::OnBoxBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	EWeaponToDo WeaponToDo = EWeaponToDo::NoWeapon;
	
	if(OtherActor->GetClass()->ImplementsInterface(UCharacterInterface::StaticClass()))
	{
		WeaponToDo = ICharacterInterface::Execute_CanPickupAmmo(OtherActor, AmmoType);
	}

	if(WeaponToDo != EWeaponToDo::NoWeapon)
	{
		if(OtherActor->GetClass()->ImplementsInterface(UCharacterInterface::StaticClass()))
		{
			ICharacterInterface::Execute_SetPickup(OtherActor, EItemType::Ammo, this);
		}
	
		// If OtherActor is the player then show the widget
		if(OtherActor == UGameplayStatics::GetPlayerPawn(GetWorld(), 0))
		{
			Widget->SetVisibility(true);
			StaticMesh->SetRenderCustomDepth(true);
		}
	}
}

void APickupAmmo::OnBoxEndOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if(OtherActor->GetClass()->ImplementsInterface(UCharacterInterface::StaticClass()))
	{
		ICharacterInterface::Execute_SetPickup(OtherActor, EItemType::Ammo, nullptr);
	}

	// If OtherActor is the player then hide the widget
	if(OtherActor == UGameplayStatics::GetPlayerPawn(GetWorld(), 0))
	{
		Widget->SetVisibility(false);
		StaticMesh->SetRenderCustomDepth(false);
	}
}