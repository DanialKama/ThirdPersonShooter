// Copyright 2022 Danial Kamali. All Rights Reserved.

#pragma once

#include "PickupEnums.generated.h"

UENUM(BlueprintType, Category = "PickupEnums")
enum class EItemType : uint8
{
	Weapon	UMETA(DisplayName = "Weapon"),
	Ammo	UMETA(DisplayName = "Ammo"),
	Health	UMETA(DisplayName = "Health")
};

UENUM(BlueprintType, Category = "PickupEnums")
enum class EPickupState : uint8
{
	Drop	UMETA(DisplayName = "Dropped"),
	Pickup	UMETA(DisplayName = "Picked Up"),
	Remove	UMETA(DisplayName = "Removed")
};

UENUM(BlueprintType, Category = "PickupWeaponEnums")
enum class EWeaponToDo : uint8
{
	NoWeapon		UMETA(DisplayName = "No Weapon"),
	PrimaryWeapon	UMETA(DisplayName = "Primary Weapon"),
	SecondaryWeapon	UMETA(DisplayName = "Secondary Weapon"),
	SidearmWeapon	UMETA(DisplayName = "Sidearm Weapon")
};

UENUM(BlueprintType, Category = "PickupWeaponEnums")
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

UENUM(BlueprintType, Category = "PickupWeaponEnums")
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

/** Supported ammo and ammo type */
UENUM(Meta = (Bitflags, UseEnumValuesAsMaskValuesInEditor = "true"))
enum class EAmmoType : uint32
{
	None						= 0x00000000	UMETA(Hidden),
	AssaultRifleNormal			= 0x00000001	UMETA(DisplayName = "Assault Rifle Normal"),
	AssaultRifleAntiArmor		= 0x00000002	UMETA(DisplayName = "Assault Rifle Anti-armor"),
	LightPistolNormal			= 0x00000004	UMETA(DisplayName = "Light Pistol Normal"),
	HeavyPistolNormal			= 0x00000008	UMETA(DisplayName = "Heavy Pistol Normal"),
	HeavyPistolAntiArmor		= 0x00000010	UMETA(DisplayName = "Heavy Pistol Anti-armor"),
	ShotgunNormal				= 0x00000020	UMETA(DisplayName = "Shotgun Normal"),
	ShotgunDragonsBreath		= 0x00000040	UMETA(DisplayName = "Shotgun Dragons Breath"),
	SniperRifleNormal			= 0x00000080	UMETA(DisplayName = "Sniper Rifle Normal"),
	SniperRifleAntiArmor		= 0x00000100	UMETA(DisplayName = "Sniper Rifle Anti-armor"),
	GrenadeLauncherExplosive	= 0x00000200	UMETA(DisplayName = "Grenade Launcher Explosive"),
	GrenadeLauncherSmoke		= 0x00000400	UMETA(DisplayName = "Grenade Launcher Smoke"),
	RocketLauncher				= 0x00000800	UMETA(DisplayName = "Rocket Launcher")
};
ENUM_CLASS_FLAGS(EAmmoType);