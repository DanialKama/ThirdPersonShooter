
#pragma once

#include "WeaponEnums.generated.h"

UENUM(BlueprintType, Category = "Enums")
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

UENUM(BlueprintType, Category = "Enums")
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

UENUM(BlueprintType, Category = "Enums")
enum class EAmmoType : uint8
{
	AssaultRifleNormal			UMETA(DisplayName = "Assault Rifle Normal"),
	AssaultRifleAntiArmor		UMETA(DisplayName = "Assault Rifle Anti-armor"),
	LightPistolNormal			UMETA(DisplayName = "Light Pistol Normal"),
	HeavyPistolNormal			UMETA(DisplayName = "Heavy Pistol Normal"),
	HeavyPistolAntiArmor		UMETA(DisplayName = "Heavy Pistol Anti-armor"),
	ShotgunNormal				UMETA(DisplayName = "Shotgun Normal"),
	ShotgunDragonsBreath		UMETA(DisplayName = "Shotgun Dragons Breath"),
	SniperRifleNormal			UMETA(DisplayName = "Sniper Rifle Normal"),
	SniperRifleAntiArmor		UMETA(DisplayName = "Sniper Rifle Anti-armor"),
	GrenadeLauncherExplosive	UMETA(DisplayName = "Grenade Launcher Explosive"),
	GrenadeLauncherSmoke		UMETA(DisplayName = "Grenade Launcher Smoke"),
	RocketLauncher				UMETA(DisplayName = "Rocket Launcher")
};