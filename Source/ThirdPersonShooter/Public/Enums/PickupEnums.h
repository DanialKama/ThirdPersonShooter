
#pragma once

#include "PickupEnums.generated.h"

UENUM(BlueprintType, Category = "Enums")
enum class EItemType : uint8
{
	Weapon	UMETA(DisplayName = "Weapon"),
	Ammo	UMETA(DisplayName = "Ammo"),
	Health	UMETA(DisplayName = "Health")
};

UENUM(BlueprintType, Category = "Enums")
enum class EPickupState : uint8
{
	Drop	UMETA(DisplayName = "Drop"),
	Pickup	UMETA(DisplayName = "Pickup"),
	Remove	UMETA(DisplayName = "Remove")
};