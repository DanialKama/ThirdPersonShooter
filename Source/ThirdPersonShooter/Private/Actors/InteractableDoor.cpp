// Copyright 2022-2023 Danial Kamali. All Rights Reserved.

#include "Actors/InteractableDoor.h"

AInteractableDoor::AInteractableDoor()
{
	SetCanBeDamaged(false);
	
	// Initialize variables
	bIsEnable = false;
}
