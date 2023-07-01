// Copyright 2022-2023 Danial Kamali. All Rights Reserved.

#pragma once

#include "Core/Enumerations/PickupEnums.h"
#include "WeaponInfo.generated.h"

// TODO: Declare this structure in the weapon class or don't use it at all if possible

/** Ammo component info that owner needs */
USTRUCT(BlueprintType)
struct FWeaponInfo
{
	GENERATED_BODY()

	FORCEINLINE	FWeaponInfo();

	explicit FORCEINLINE FWeaponInfo(EWeaponType InWeaponType, FString InName, float InRange, float InEffectiveRange, bool InbIsAutomatic, float InTimeBetweenShots, float InCoolDownTime, int32 InAmmoType, float InMinFireOffset, float InMaxFireOffset, UCurveFloat* InWeaponSpreadCurve);

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	EWeaponType WeaponType = EWeaponType::Pistol;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	FString Name = FString("Weapon");

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	float Range = 4000.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	float EffectiveRange = 2000.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	bool bIsAutomatic = false;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	float TimeBetweenShots = 0.5f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	float CoolDownTime = 0.5f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (Bitmask, BitmaskEnum = "/Script/ThirdPersonShooter.EAmmoType"))
	int32 AmmoType = 0;

	/** Min Fire Offset is only for AI */
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	float MinFireOffset = -10.0f;

	/** Max Fire Offset is only for AI */
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	float MaxFireOffset = 10.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	UCurveFloat* WeaponSpreadCurve = nullptr;
};

FORCEINLINE FWeaponInfo::FWeaponInfo()
{
}

FORCEINLINE FWeaponInfo::FWeaponInfo(const EWeaponType InWeaponType, const FString InName, const float InRange, const float InEffectiveRange, const bool InbIsAutomatic, const float InTimeBetweenShots, const float InCoolDownTime, const int32 InAmmoType, const float InMinFireOffset, const float InMaxFireOffset, UCurveFloat* InWeaponSpreadCurve)
: WeaponType(InWeaponType), Name(InName), Range(InRange), EffectiveRange(InEffectiveRange), bIsAutomatic(InbIsAutomatic), TimeBetweenShots(InTimeBetweenShots), CoolDownTime(InCoolDownTime), AmmoType(InAmmoType), MinFireOffset(InMinFireOffset), MaxFireOffset(InMaxFireOffset), WeaponSpreadCurve(InWeaponSpreadCurve)
{
}
