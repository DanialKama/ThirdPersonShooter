// All Rights Reserved.

#include "Actors/PickupAmmo.h"
#include "Components/SphereComponent.h"
#include "Components/WidgetComponent.h"
#include "Sound/SoundCue.h"
#include "Kismet/GameplayStatics.h"
#include "Interfaces/CharacterInterface.h"
#include "Interfaces/WidgetInterface.h"

APickupAmmo::APickupAmmo()
{
	PrimaryActorTick.bCanEverTick = false;
	
	StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	SetRootComponent(StaticMesh);
	StaticMesh->SetComponentTickEnabled(false);
	StaticMesh->SetSimulatePhysics(true);
	StaticMesh->bApplyImpulseOnDamage = false;
	StaticMesh->SetGenerateOverlapEvents(false);
	StaticMesh->CanCharacterStepUpOn = ECB_No;
	StaticMesh->SetCollisionProfileName("Ammo");
	
	SphereCollision = CreateDefaultSubobject<USphereComponent>(TEXT("Collision"));
	SphereCollision->SetupAttachment(StaticMesh);
	SphereCollision->SetComponentTickEnabled(false);
	SphereCollision->bApplyImpulseOnDamage = false;
	SphereCollision->CanCharacterStepUpOn = ECB_No;
	SphereCollision->SetCollisionProfileName("CollisionBound");
	SphereCollision->OnComponentBeginOverlap.AddDynamic(this, &APickupAmmo::OnBoxBeginOverlap);
	SphereCollision->OnComponentEndOverlap.AddDynamic(this, &APickupAmmo::OnBoxEndOverlap);
	
	Widget = CreateDefaultSubobject<UWidgetComponent>(TEXT("Widget"));
	Widget->SetupAttachment(StaticMesh);
	Widget->SetComponentTickEnabled(false);
	Widget->SetWidgetSpace(EWidgetSpace::Screen);
	Widget->SetGenerateOverlapEvents(false);
	Widget->CanCharacterStepUpOn = ECB_No;
	Widget->SetVisibility(false);
	
	// Initialize variables
	PickupType = EItemType::Ammo;
	AmmoType = static_cast<int32>(EAmmoType::None);
	Amount = 1;
}

void APickupAmmo::BeginPlay()
{
	Super::BeginPlay();

	if (Widget->GetWidget()->GetClass()->ImplementsInterface(UWidgetInterface::StaticClass()))
	{
		IWidgetInterface::Execute_SetAmmoInfo(Widget->GetWidget(), AmmoType, Amount);
	}
}

void APickupAmmo::SetPickupStatus(EPickupState PickupState)
{
	switch (PickupState)
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

void APickupAmmo::OnBoxBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	EWeaponToDo WeaponToDo = EWeaponToDo::NoWeapon;
	
	if (OtherActor->GetClass()->ImplementsInterface(UCharacterInterface::StaticClass()))
	{
		WeaponToDo = ICharacterInterface::Execute_CanPickupAmmo(OtherActor, AmmoType);
	}

	if (WeaponToDo != EWeaponToDo::NoWeapon)
	{
		if (OtherActor->GetClass()->ImplementsInterface(UCharacterInterface::StaticClass()))
		{
			ICharacterInterface::Execute_SetPickup(OtherActor, EItemType::Ammo, this);
		}
		
		// Show the widget if OtherActor is the player
		if (OtherActor == UGameplayStatics::GetPlayerPawn(GetWorld(), 0))
		{
			Widget->SetVisibility(true);
			StaticMesh->SetRenderCustomDepth(true);
		}
	}
}

void APickupAmmo::OnBoxEndOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (OtherActor->GetClass()->ImplementsInterface(UCharacterInterface::StaticClass()))
	{
		ICharacterInterface::Execute_SetPickup(OtherActor, EItemType::Ammo, nullptr);
	}

	// Hide the widget if OtherActor is the player
	if (OtherActor == UGameplayStatics::GetPlayerPawn(GetWorld(), 0))
	{
		Widget->SetVisibility(false);
		StaticMesh->SetRenderCustomDepth(false);
	}
}