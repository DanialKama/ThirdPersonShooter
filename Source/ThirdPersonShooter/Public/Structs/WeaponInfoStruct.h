// Copyright 2022 Danial Kamali. All Rights Reserved.

#pragma once

#include "UObject/NoExportTypes.h"
#include "Enums/PickupEnums.h"
#include "WeaponInfoStruct.generated.h"

/** Ammo component info that owner needs */
USTRUCT(BlueprintType)
struct THIRDPERSONSHOOTER_API FWeaponInfo
{
	GENERATED_BODY()

	FORCEINLINE FWeaponInfo();

	explicit FORCEINLINE FWeaponInfo(EWeaponType InWeaponType, FString InName, float InRange, float InEffectiveRange, bool InbIsAutomatic, float InTimeBetweenShots, float InCoolDownTime, int32 InAmmoType, float InMinFireOffset, float InMaxFireOffset, UCurveFloat* InWeaponSpreadCurve);

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Structs")
	EWeaponType WeaponType = EWeaponType::Pistol;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Structs")
	FString Name = TEXT("Weapon");

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Structs")
	float Range = 4000.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Structs")
	float EffectiveRange = 1500.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Structs")
	bool bIsAutomatic = false;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Structs")
	float TimeBetweenShots = 0.5f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Structs")
	float CoolDownTime = 0.5f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Structs", meta = (Bitmask, BitmaskEnum = "EAmmoType"))
	int32 AmmoType = static_cast<int32>(EAmmoType::None);

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Structs", meta = (ToolTip = "Min Fire Offset is only for AI"))
	float MinFireOffset = -10.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Structs", meta = (ToolTip = "Max Fire Offset is only for AI"))
	float MaxFireOffset = 10.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Structs")
	UCurveFloat* WeaponSpreadCurve = nullptr;
};

FORCEINLINE FWeaponInfo::FWeaponInfo()
{
}

FORCEINLINE FWeaponInfo::FWeaponInfo(const EWeaponType InWeaponType, const FString InName, const float InRange, const float InEffectiveRange, const bool InbIsAutomatic, const float InTimeBetweenShots, const float InCoolDownTime, const int32 InAmmoType, const float InMinFireOffset, const float InMaxFireOffset, UCurveFloat* InWeaponSpreadCurve)
: WeaponType(InWeaponType), Name(InName), Range(InRange), EffectiveRange(InEffectiveRange), bIsAutomatic(InbIsAutomatic), TimeBetweenShots(InTimeBetweenShots), CoolDownTime(InCoolDownTime), AmmoType(InAmmoType), MinFireOffset(InMinFireOffset), MaxFireOffset(InMaxFireOffset), WeaponSpreadCurve(InWeaponSpreadCurve)
{
}