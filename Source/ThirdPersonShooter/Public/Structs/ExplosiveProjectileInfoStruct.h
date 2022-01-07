// All Rights Reserved.

#pragma once

#include "Engine/DataTable.h"
#include "ExplosiveProjectileInfoStruct.generated.h"

/** Projectile info that projectile class needs */
USTRUCT(BlueprintType)
struct FExplosiveProjectileInfo : public FTableRowBase
{
	GENERATED_BODY()

	FORCEINLINE FExplosiveProjectileInfo();

	explicit FORCEINLINE FExplosiveProjectileInfo(float InBaseDamage, float InMinimumDamage, float InDamageInnerRadius, float InDamageOuterRadius, float InStrainMagnitude, float InForceMagnitude, float InTorqueMagnitude);

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Structs")
	float BaseDamage = 200.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Structs")
	float MinimumDamage = 50.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Structs")
	float DamageInnerRadius = 100.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Structs")
	float DamageOuterRadius = 400.0f;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Structs", Meta = (ToolTip = "Only for destructible objects"))
	float StrainMagnitude = 2000.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Structs", Meta = (ToolTip = "Only for destructible objects"))
	float ForceMagnitude = 2000.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Structs", Meta = (ToolTip = "Only for destructible objects"))
	float TorqueMagnitude = 500.0f;
};

FORCEINLINE FExplosiveProjectileInfo::FExplosiveProjectileInfo()
{
}

FORCEINLINE FExplosiveProjectileInfo::FExplosiveProjectileInfo(const float InBaseDamage, const float InMinimumDamage, const float InDamageInnerRadius, const float InDamageOuterRadius, const float InStrainMagnitude, const float InForceMagnitude, const float InTorqueMagnitude)
: BaseDamage(InBaseDamage), MinimumDamage(InMinimumDamage), DamageInnerRadius(InDamageInnerRadius), DamageOuterRadius(InDamageOuterRadius), StrainMagnitude(InStrainMagnitude), ForceMagnitude(InForceMagnitude), TorqueMagnitude(InTorqueMagnitude)
{
}