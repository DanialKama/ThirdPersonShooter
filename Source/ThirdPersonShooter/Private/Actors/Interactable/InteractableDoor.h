// Copyright 2022-2023 Danial Kamali. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Interactable.h"
#include "InteractableDoor.generated.h"

UCLASS(meta = (DisplayName = "Interactable Door Actor"))
class AInteractableDoor : public AInteractable
{
	GENERATED_BODY()

// Functions
public:
	AInteractableDoor();

// Variables
public:
	uint8 bIsEnable : 1;
};
