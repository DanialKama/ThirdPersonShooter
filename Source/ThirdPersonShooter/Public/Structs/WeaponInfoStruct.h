
#pragma once

#include "Enums/WeaponEnums.h"
#include "WeaponInfoStruct.generated.h"

// Ammo component info that owner needs
USTRUCT(BlueprintType)
struct FWeaponInfo
{
	GENERATED_BODY()

	FORCEINLINE FWeaponInfo();

	explicit FORCEINLINE FWeaponInfo(EWeaponType InWeaponType, FString InName, float InRange, uint8 InbIsAutomatic, float InTimeBetweenShots, float InCoolDownTime, TArray<EAmmoType> InAmmoType, float InMinFireOffset, float InMaxFireOffset, UCurveFloat* WeaponSpreadCurve);

	UPROPERTY(BlueprintReadWrite, Category = "Structs")
	EWeaponType WeaponType = EWeaponType::Pistol;

	UPROPERTY(BlueprintReadWrite, Category = "Structs")
	FString Name = TEXT("Weapon");

	UPROPERTY(BlueprintReadWrite, Category = "Structs")
	float Range = 4000.0f;

	UPROPERTY(BlueprintReadWrite, Category = "Structs")
	uint8 bIsAutomatic : 1;

	UPROPERTY(BlueprintReadWrite, Category = "Structs")
	float TimeBetweenShots = 0.5f;

	UPROPERTY(BlueprintReadWrite, Category = "Structs")
	float CoolDownTime = 0.5f;

	UPROPERTY(BlueprintReadWrite, Category = "Structs")
	TArray<EAmmoType> AmmoType;

	UPROPERTY(BlueprintReadWrite, Category = "Structs", meta = (ToolTip = "Min Fire Offset is only for AI"))
	float MinFireOffset = -10.0f;

	UPROPERTY(BlueprintReadWrite, Category = "Structs", meta = (ToolTip = "Max Fire Offset is only for AI"))
	float MaxFireOffset = 10.0f;

	UPROPERTY(BlueprintReadWrite, Category = "Structs")
	UCurveFloat* WeaponSpreadCurve;

	// Operators
	// bool operator==(const FWeaponInfo& V) const;
	// bool operator!=(const FWeaponInfo& V) const;
};

FORCEINLINE FWeaponInfo::FWeaponInfo()
{
}

FORCEINLINE FWeaponInfo::FWeaponInfo(const EWeaponType InWeaponType, const FString InName, const float InRange, const uint8 InbIsAutomatic, const float InTimeBetweenShots, const float InCoolDownTime, const TArray<EAmmoType> InAmmoType, const float InMinFireOffset, const float InMaxFireOffset, UCurveFloat* InWeaponSpreadCurve) : WeaponType(InWeaponType), Name(InName), Range(InRange), bIsAutomatic(InbIsAutomatic), TimeBetweenShots(InTimeBetweenShots), CoolDownTime(InCoolDownTime), AmmoType(InAmmoType), MinFireOffset(InMinFireOffset), MaxFireOffset(InMaxFireOffset), WeaponSpreadCurve(InWeaponSpreadCurve)
{
}

// Operators
// FORCEINLINE bool FWeaponInfo::operator==(const FWeaponInfo& V) const
// {
// 	// TODO
// }
//
// FORCEINLINE bool FWeaponInfo::operator!=(const FWeaponInfo& V) const
// {
// 	// TODO
// }

// To use struct as key for maps
FORCEINLINE uint32 GetTypeHash(const FWeaponInfo& b)
{
	return FCrc::MemCrc_DEPRECATED(&b, sizeof(FWeaponInfo));
}