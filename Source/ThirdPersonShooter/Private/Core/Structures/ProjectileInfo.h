// Copyright 2022-2023 Danial Kamali. All Rights Reserved.

#pragma once

#include "Engine/DataTable.h"
#include "ProjectileInfo.generated.h"

// TODO: Delete properties related to destructible object if we are no longer using them

/** Used by the projectile class and the projectile data table */
USTRUCT(BlueprintType, meta = (DisplayName = "Projectile Info"))
struct FProjectileInfo : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	float DefaultDamage;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	float DamageToHead;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	float DamageToBody;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	float DamageToHand;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	float DamageToLeg;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	float DamageToWood;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	float DamageToMetal;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	float DamageToStone;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	float DamageToDirt;

	/** Only for destructible objects */
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	float StrainMagnitude;

	/** Only for destructible objects */
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	float ForceMagnitude;

	/** Only for destructible objects */
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	float TorqueMagnitude;
	
	/** Default constructor */
	FProjectileInfo()
		: DefaultDamage(75.0f), DamageToHead(100.0f), DamageToBody(50.0f), DamageToHand(25.0f), DamageToLeg(25.0),
		DamageToWood(70.0f), DamageToMetal(50.0f), DamageToStone(60.0f), DamageToDirt(65.0f),
		StrainMagnitude(1000.0f), ForceMagnitude(250.0f), TorqueMagnitude(100.0f)
	{}
	
	FProjectileInfo(const float InDefaultDamage, const float InDamageToHead, const float InDamageToBody, const float InDamageToHand, const float InDamageToLeg,
		const float InDamageToWood, const float InDamageToMetal, const float InDamageToStone, const float InDamageToDirt,
		const float InStrainMagnitude, const float InForceMagnitude, const float InTorqueMagnitude)
		: DefaultDamage(InDefaultDamage), DamageToHead(InDamageToHead), DamageToBody(InDamageToBody), DamageToHand(InDamageToHand), DamageToLeg(InDamageToLeg),
		DamageToWood(InDamageToWood), DamageToMetal(InDamageToMetal), DamageToStone(InDamageToStone), DamageToDirt(InDamageToDirt),
		StrainMagnitude(InStrainMagnitude), ForceMagnitude(InForceMagnitude), TorqueMagnitude(InTorqueMagnitude)
	{}
};

/** Used by the projectile class and the projectile data table */
USTRUCT(BlueprintType, meta = (DisplayName = "Explosive Projectile Info"))
struct FExplosiveProjectileInfo : public FTableRowBase
{
	GENERATED_BODY()
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	float BaseDamage;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	float MinimumDamage;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	float DamageInnerRadius;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	float DamageOuterRadius;

	/** Only for destructible objects */
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	float StrainMagnitude;

	/** Only for destructible objects */
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	float ForceMagnitude;

	/** Only for destructible objects */
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	float TorqueMagnitude;

	/** Default constructor */
	FExplosiveProjectileInfo()
		: BaseDamage(200.0f), MinimumDamage(50.0f), DamageInnerRadius(100.0f), DamageOuterRadius(400.0f),
		StrainMagnitude(2000.0f), ForceMagnitude(2000.0f), TorqueMagnitude(500.0f)
	{}

	FExplosiveProjectileInfo(const float InBaseDamage, const float InMinimumDamage, const float InDamageInnerRadius, const float InDamageOuterRadius,
		const float InStrainMagnitude, const float InForceMagnitude, const float InTorqueMagnitude)
		: BaseDamage(InBaseDamage), MinimumDamage(InMinimumDamage), DamageInnerRadius(InDamageInnerRadius), DamageOuterRadius(InDamageOuterRadius),
		StrainMagnitude(InStrainMagnitude), ForceMagnitude(InForceMagnitude), TorqueMagnitude(InTorqueMagnitude)
	{}
};
