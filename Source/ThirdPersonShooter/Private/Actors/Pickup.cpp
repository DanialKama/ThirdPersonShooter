// Copyright 2022-2023 Danial Kamali. All Rights Reserved.

#include "Pickup.h"

APickup::APickup()
{
	PrimaryActorTick.bCanEverTick = false;

	// Initialize variables
	PickupType = EItemType::Weapon;
}

void APickup::SetPickupStatus(EPickupState PickupState)
{
}