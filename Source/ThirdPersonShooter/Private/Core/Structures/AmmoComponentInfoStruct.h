// Copyright 2022-2023 Danial Kamali. All Rights Reserved.

#pragma once

#include "AmmoComponentInfoStruct.generated.h"

// TODO: Declare it in the ammo component

/** Ammo component info that owner needs */
USTRUCT(BlueprintType)
struct FAmmoComponentInfo
{
	GENERATED_BODY()
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	bool bNoAmmoLeftToReload;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	int32 CurrentAmmo;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	int32 MagazineSize;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	int32 CurrentMagazineAmmo;

	/** Default constructor */
	FAmmoComponentInfo()
		: bNoAmmoLeftToReload(false), CurrentAmmo(0), MagazineSize(0), CurrentMagazineAmmo(0)
	{}

	FAmmoComponentInfo(const bool bInbNoAmmoLeftToReload, const int32 InCurrentAmmo, const int32 InMagazineSize, const int32 InCurrentMagazineAmmo)
		: bNoAmmoLeftToReload(bInbNoAmmoLeftToReload), CurrentAmmo(InCurrentAmmo), MagazineSize(InMagazineSize), CurrentMagazineAmmo(InCurrentMagazineAmmo)
	{}
};
