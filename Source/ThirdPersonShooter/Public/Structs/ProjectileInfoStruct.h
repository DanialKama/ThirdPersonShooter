
#pragma once

#include "Engine/DataTable.h"
#include "ProjectileInfoStruct.generated.h"

// Projectile info that projectile class needs
USTRUCT(BlueprintType)
struct FProjectileInfo : public FTableRowBase
{
	GENERATED_BODY()

	FORCEINLINE FProjectileInfo();

	explicit FORCEINLINE FProjectileInfo(float InDefaultDamage, float InDamageToHead, float InDamageToBody, float InDamageToHand,
		float InDamageToLeg, float InDamageToWood, float InDamageToMetal, float InDamageToStone, TSubclassOf<UDamageType> InDamageType,
		float InStrainMagnitude, float InForceMagnitude, float InTorqueMagnitude);

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Structs")
	float DefaultDamage = 75.0f;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Structs")
	float DamageToHead = 100.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Structs")
	float DamageToBody = 50.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Structs")
	float DamageToHand = 25.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Structs")
	float DamageToLeg = 25.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Structs")
	float DamageToWood = 70.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Structs")
	float DamageToMetal = 50.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Structs")
	float DamageToStone = 60.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Structs")
	TSubclassOf<UDamageType> DamageType;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Structs", Meta = (ToolTip = "Only for destructible objects"))
	float StrainMagnitude = 1000.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Structs", Meta = (ToolTip = "Only for destructible objects"))
	float ForceMagnitude = 250.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Structs", Meta = (ToolTip = "Only for destructible objects"))
	float TorqueMagnitude = 100.0f;
	
	// Operators
	// bool operator==(const FProjectileInfo& V) const;
	// bool operator!=(const FProjectileInfo& V) const;
};

FORCEINLINE FProjectileInfo::FProjectileInfo()
{
}

FORCEINLINE FProjectileInfo::FProjectileInfo(const float InDefaultDamage, const float InDamageToHead, const float InDamageToBody,
	const float InDamageToHand, const float InDamageToLeg, const float InDamageToWood, const float InDamageToMetal,
	const float InDamageToStone, TSubclassOf<UDamageType> InDamageType, const float InStrainMagnitude, const float InForceMagnitude,
	const float InTorqueMagnitude) : DefaultDamage(InDefaultDamage) ,DamageToHead(InDamageToHead), DamageToBody(InDamageToBody),
	DamageToHand(InDamageToHand), DamageToLeg(InDamageToLeg), DamageToWood(InDamageToWood), DamageToMetal(InDamageToMetal),
	DamageToStone(InDamageToStone), DamageType(InDamageType), StrainMagnitude(InStrainMagnitude), ForceMagnitude(InForceMagnitude),
	TorqueMagnitude(InTorqueMagnitude)
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