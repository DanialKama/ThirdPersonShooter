
#pragma once

#include "AmmoComponentInfoStruct.generated.h"

// Ammo component info that owner needs
USTRUCT(BlueprintType)
struct FAmmoComponentInfo
{
	GENERATED_BODY()

	FORCEINLINE FAmmoComponentInfo();

	explicit FORCEINLINE FAmmoComponentInfo(uint8 InNoAmmoLeftToReload, int32 InCurrentAmmo, int32 InMagazineSize, int32 InCurrentMagazineAmmo);

	UPROPERTY(BlueprintReadWrite, Category = "Structs")
	uint8 NoAmmoLeftToReload : 1;

	UPROPERTY(BlueprintReadWrite, Category = "Structs")
	int32 CurrentAmmo = 0;

	UPROPERTY(BlueprintReadWrite, Category = "Structs")
	int32 MagazineSize = 0;

	UPROPERTY(BlueprintReadWrite, Category = "Structs")
	int32 CurrentMagazineAmmo = 0;

	// Operators
	bool operator==(const FAmmoComponentInfo& V) const;
	bool operator!=(const FAmmoComponentInfo& V) const;
};

FORCEINLINE FAmmoComponentInfo::FAmmoComponentInfo()
{
}

FORCEINLINE FAmmoComponentInfo::FAmmoComponentInfo(const uint8 InNoAmmoLeftToReload, const int32 InCurrentAmmo, const int32 InMagazineSize, const int32 InCurrentMagazineAmmo) : NoAmmoLeftToReload(InNoAmmoLeftToReload), CurrentAmmo(InCurrentAmmo), MagazineSize(InMagazineSize), CurrentMagazineAmmo(InCurrentMagazineAmmo)
{
}

// Operators
FORCEINLINE bool FAmmoComponentInfo::operator==(const FAmmoComponentInfo& V) const
{
	return NoAmmoLeftToReload == V.NoAmmoLeftToReload && CurrentAmmo == V.CurrentAmmo && MagazineSize == V.MagazineSize && CurrentMagazineAmmo == V.CurrentMagazineAmmo;
}

FORCEINLINE bool FAmmoComponentInfo::operator!=(const FAmmoComponentInfo& V) const
{
	return NoAmmoLeftToReload != V.NoAmmoLeftToReload || CurrentAmmo != V.CurrentAmmo || MagazineSize != V.MagazineSize || CurrentMagazineAmmo != V.CurrentMagazineAmmo;
}

// To use struct as key for maps
FORCEINLINE uint32 GetTypeHash(const FAmmoComponentInfo& b)
{
	return FCrc::MemCrc_DEPRECATED(&b, sizeof(FAmmoComponentInfo));
}