
#pragma once

#include "UObject/NoExportTypes.h"
#include "ProjectileInfoStruct.generated.h"

// Projectile info that projectile class needs
USTRUCT(BlueprintType)
struct THIRDPERSONSHOOTER_API FProjectileInfo
{
	GENERATED_BODY()

	FORCEINLINE FProjectileInfo();

	explicit FORCEINLINE FProjectileInfo(float InDamageToHead, float InDamageToBody, float InDamageToHand, float InDamageToLeg, float InDamageToWood, float InDamageToMetal, float InDamageToStone, UDamageType* InDamageType, float InStrainMagnitude, float InForceMagnitude, float InTorqueMagnitude);

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
	UDamageType* DamageType = nullptr;
	
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

FORCEINLINE FProjectileInfo::FProjectileInfo(float InDamageToHead, float InDamageToBody, float InDamageToHand, float InDamageToLeg, float InDamageToWood, float InDamageToMetal, float InDamageToStone, UDamageType* InDamageType, float InStrainMagnitude, float InForceMagnitude, float InTorqueMagnitude)
: DamageToHead(InDamageToHead), DamageToBody(InDamageToBody), DamageToHand(InDamageToHand), DamageToLeg(InDamageToLeg), DamageToWood(InDamageToWood), DamageToMetal(InDamageToMetal), DamageToStone(InDamageToStone), DamageType(InDamageType), StrainMagnitude(InStrainMagnitude), ForceMagnitude(InForceMagnitude), TorqueMagnitude(InTorqueMagnitude)
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