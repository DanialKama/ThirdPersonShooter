// Copyright 2022-2023 Danial Kamali. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interactable.generated.h"

UCLASS(Abstract, meta = (DisplayName = "Interactable"))
class AInteractable : public AActor
{
	GENERATED_BODY()

// Functions
public:
	AInteractable();
};
