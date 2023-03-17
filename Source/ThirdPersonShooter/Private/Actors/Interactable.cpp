// Copyright 2022-2023 Danial Kamali. All Rights Reserved.

#include "Actors/Interactable.h"

AInteractable::AInteractable()
{
	PrimaryActorTick.bCanEverTick = false;
	PrimaryActorTick.bStartWithTickEnabled = false;
}