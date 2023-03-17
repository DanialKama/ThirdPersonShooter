// Copyright 2022-2023 Danial Kamali. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Enums/PickupEnums.h"
#include "Pickup.generated.h"

UCLASS()
class APickup : public AActor
{
	GENERATED_BODY()

// Functions
public:	
	APickup();

	virtual void SetPickupStatus(EPickupState PickupState);

// Variables
protected:
	UPROPERTY(EditDefaultsOnly, Category = "Defaults")
	EItemType PickupType;
};