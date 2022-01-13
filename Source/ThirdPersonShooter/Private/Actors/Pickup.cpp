// All Rights Reserved.

#include "Actors/Pickup.h"

APickup::APickup()
{
	PrimaryActorTick.bCanEverTick = false;

	// Initialize variables
	PickupType = EItemType::Weapon;
}

void APickup::SetPickupStatus(EPickupState PickupState)
{
}