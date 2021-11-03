// Danial Kama

#pragma once

#include "PickupEnums.generated.h"

UENUM(BlueprintType, Category = "Pickup")
enum class EItemType : uint8
{
	Weapon	UMETA(DisplayName = "Weapon"),
	Ammo	UMETA(DisplayName = "Ammo"),
	Health	UMETA(DisplayName = "Health")
};