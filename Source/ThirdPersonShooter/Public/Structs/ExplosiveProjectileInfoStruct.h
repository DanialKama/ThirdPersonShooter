
#pragma once

#include "UObject/NoExportTypes.h"
#include "ExplosiveProjectileInfoStruct.generated.h"

// Projectile info that projectile class needs
USTRUCT(BlueprintType)
struct THIRDPERSONSHOOTER_API FExplosiveProjectileInfo
{
	GENERATED_BODY()

	FORCEINLINE FExplosiveProjectileInfo();

	explicit FORCEINLINE FExplosiveProjectileInfo(float InBaseDamage, float InMinimumDamage, float InDamageInnerRadius, float InDamageOuterRadius, UDamageType* InDamageType, float InStrainMagnitude, float InForceMagnitude, float InTorqueMagnitude);

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Structs")
	float BaseDamage = 200.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Structs")
	float MinimumDamage = 50.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Structs")
	float DamageInnerRadius = 100.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Structs")
	float DamageOuterRadius = 400.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Structs")
	UDamageType* DamageType = nullptr;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Structs", Meta = (ToolTip = "Only for destructible objects"))
	float StrainMagnitude = 2000.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Structs", Meta = (ToolTip = "Only for destructible objects"))
	float ForceMagnitude = 2000.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Structs", Meta = (ToolTip = "Only for destructible objects"))
	float TorqueMagnitude = 500.0f;
	
	// Operators
	// bool operator==(const FProjectileInfo& V) const;
	// bool operator!=(const FProjectileInfo& V) const;
};

FORCEINLINE FExplosiveProjectileInfo::FExplosiveProjectileInfo()
{
}

FORCEINLINE FExplosiveProjectileInfo::FExplosiveProjectileInfo(float InBaseDamage, float InMinimumDamage, float InDamageInnerRadius, float InDamageOuterRadius, UDamageType* InDamageType, float InStrainMagnitude, float InForceMagnitude, float InTorqueMagnitude)
: BaseDamage(InBaseDamage), MinimumDamage(InMinimumDamage), DamageInnerRadius(InDamageInnerRadius), DamageOuterRadius(InDamageOuterRadius), DamageType(InDamageType), StrainMagnitude(InStrainMagnitude), ForceMagnitude(InForceMagnitude), TorqueMagnitude(InTorqueMagnitude)
{
}

// Operators
// FORCEINLINE bool FProjectileInfo::operator==(const FAmmoComponentInfo& V) const
// {
// }
//
// FORCEINLINE bool FProjectileInfo::operator!=(const FAmmoComponentInfo& V) const
// {
// }

// To use struct as key for maps
// FORCEINLINE uint32 GetTypeHash(const FProjectileInfo& b)
// {
// 	return FCrc::MemCrc_DEPRECATED(&b, sizeof(FProjectileInfo));
// }