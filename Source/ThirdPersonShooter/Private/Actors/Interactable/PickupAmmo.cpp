// Copyright 2022-2023 Danial Kamali. All Rights Reserved.

#include "PickupAmmo.h"

#include "Components/SphereComponent.h"
#include "Components/WidgetComponent.h"
#include "Core/Interfaces/CharacterInterface.h"
#include "Core/Interfaces/WidgetInterface.h"
#include "Kismet/GameplayStatics.h"

APickupAmmo::APickupAmmo()
{
	StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	SetRootComponent(StaticMesh);
	StaticMesh->SetComponentTickEnabled(false);
	StaticMesh->SetSimulatePhysics(true); // TODO: set to false
	StaticMesh->bApplyImpulseOnDamage = false;
	StaticMesh->SetGenerateOverlapEvents(false);
	StaticMesh->CanCharacterStepUpOn = ECB_No;
	StaticMesh->SetCollisionProfileName("Ammo");
	
	SphereCollision = CreateDefaultSubobject<USphereComponent>(TEXT("Collision"));
	SphereCollision->SetupAttachment(StaticMesh);
	SphereCollision->SetComponentTickEnabled(false);
	SphereCollision->bApplyImpulseOnDamage = false;
	SphereCollision->CanCharacterStepUpOn = ECB_No;
	SphereCollision->SetCollisionProfileName("CollisionBound");	// TODO: Use Trigger profile
	
	// TODO: Show message on the player UI
	Widget = CreateDefaultSubobject<UWidgetComponent>(TEXT("Widget"));
	Widget->SetupAttachment(StaticMesh);
	Widget->SetComponentTickEnabled(false);
	Widget->SetWidgetSpace(EWidgetSpace::Screen);
	Widget->SetGenerateOverlapEvents(false);
	Widget->CanCharacterStepUpOn = ECB_No;
	Widget->SetVisibility(false);
	
	// Initialize variables
	PickupType = EItemType::Ammo;
}

void APickupAmmo::BeginPlay()
{
	Super::BeginPlay();

	if (Widget->GetWidget()->GetClass()->ImplementsInterface(UWidgetInterface::StaticClass()))
	{
		IWidgetInterface::Execute_SetAmmoInfo(Widget->GetWidget(), AmmoType, Amount);
	}
	
	SphereCollision->OnComponentBeginOverlap.AddDynamic(this, &APickupAmmo::OnBoxBeginOverlap);
	SphereCollision->OnComponentEndOverlap.AddDynamic(this, &APickupAmmo::OnBoxEndOverlap);
}

void APickupAmmo::SetPickUpState(const EPickupState PickupState)
{
	switch (PickupState)
	{
	case EPickupState::Drop:
		SphereCollision->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
		break;
	case EPickupState::PickUp:
		SphereCollision->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		break;
	case EPickupState::Remove:
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
