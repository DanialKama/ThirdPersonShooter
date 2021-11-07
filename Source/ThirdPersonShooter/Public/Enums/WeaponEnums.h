
#pragma once

#include "WeaponEnums.generated.h"

UENUM(BlueprintType, Category = "Weapon")
enum class EWeaponState : uint8
{
	Idle			UMETA(DisplayName = "Idle"),
	Firing			UMETA(DisplayName = "Firing"),
	BetterToReload	UMETA(DisplayName = "BetterToReload"),
	NeedToReload	UMETA(DisplayName = "NeedToReload"),
	Reloading		UMETA(DisplayName = "Reloading"),
	CancelReload	UMETA(DisplayName = "CancelReload"),
	Reloaded		UMETA(DisplayName = "Reloaded"),
	AmmoAdded		UMETA(DisplayName = "AmmoAdded"),
	Empty			UMETA(DisplayName = "Empty"),
	Overheat		UMETA(DisplayName = "Overheat")
};

UENUM(BlueprintType, Category = "WEapon")
enum class EWeaponType : uint8
{
	Pistol		UMETA(DisplayName = "Pistol"),
	SMG			UMETA(DisplayName = "SMG"),
	Rifle		UMETA(DisplayName = "Rifle"),
	LMG			UMETA(DisplayName = "LMG"),
	Shotgun		UMETA(DisplayName = "Shotgun"),
	Sniper		UMETA(DisplayName = "Sniper"),
	Launcher	UMETA(DisplayName = "Launcher")
};
