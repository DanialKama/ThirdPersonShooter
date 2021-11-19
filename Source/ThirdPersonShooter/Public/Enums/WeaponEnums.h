
#pragma once

#include "WeaponEnums.generated.h"

UENUM(BlueprintType, Category = "Enums")
enum class EWeaponState : uint8
{
	Idle			UMETA(DisplayName = "Idle"),
	Firing			UMETA(DisplayName = "Firing"),
	BetterToReload	UMETA(DisplayName = "Better To Reload"),
	NeedToReload	UMETA(DisplayName = "Need To Reload"),
	Reloading		UMETA(DisplayName = "Reloading"),
	CancelReload	UMETA(DisplayName = "Cancel Reload"),
	Reloaded		UMETA(DisplayName = "Reloaded"),
	AmmoAdded		UMETA(DisplayName = "Ammo Added"),
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
	AssaultRifleNormal			= 0		UMETA(DisplayName = "Assault Rifle Normal"),
	AssaultRifleAntiArmor		= 1		UMETA(DisplayName = "Assault Rifle Anti-armor"),
	LightPistolNormal			= 2		UMETA(DisplayName = "Light Pistol Normal"),
	HeavyPistolNormal			= 3		UMETA(DisplayName = "Heavy Pistol Normal"),
	HeavyPistolAntiArmor		= 4		UMETA(DisplayName = "Heavy Pistol Anti-armor"),
	ShotgunNormal				= 5		UMETA(DisplayName = "Shotgun Normal"),
	ShotgunDragonsBreath		= 6		UMETA(DisplayName = "Shotgun Dragons Breath"),
	SniperRifleNormal			= 7		UMETA(DisplayName = "Sniper Rifle Normal"),
	SniperRifleAntiArmor		= 8		UMETA(DisplayName = "Sniper Rifle Anti-armor"),
	GrenadeLauncherExplosive	= 9		UMETA(DisplayName = "Grenade Launcher Explosive"),
	GrenadeLauncherSmoke		= 10	UMETA(DisplayName = "Grenade Launcher Smoke"),
	RocketLauncher				= 11	UMETA(DisplayName = "Rocket Launcher")
};