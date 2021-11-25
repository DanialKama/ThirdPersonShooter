
#pragma once

#include "Engine/DataTable.h"
// #include "Sound/SoundCue.h"
#include "ProjectileHitEffectStruct.generated.h"

// Projectile hit effect info that projectile class needs
USTRUCT(BlueprintType)
struct FProjectileHitEffect : public FTableRowBase
{
	GENERATED_BODY()

	FORCEINLINE FProjectileHitEffect();

	// explicit FORCEINLINE FProjectileHitEffect(UParticleSystem* InEmitter, FVector InEmitterScale,
	// 	UParticleSystem* InExplosiveEmitter, FVector InExplosiveEmitterScale, USoundCue* InSound,
	// 	UMaterialInterface* InDecal, FVector InDecalSize, float InDecalLifeSpan);
	//
	// // Particle systems and particle spawn scale
	// UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Struct")
	// UParticleSystem* Emitter;
	//
	// UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Struct")
	// FVector EmitterScale = FVector(1.0f, 1.0f, 1.0f);
	//
	// UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Struct")
	// UParticleSystem* ExplosiveEmitter;
	//
	// UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Struct")
	// FVector ExplosionEmitterScale = FVector(1.0f, 1.0f, 1.0f);
	//
	// // Sound Cues
	// UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Struct")
	// USoundCue* Sound;
	//
	// // Decals, decal spawn size, and decal life span
	// UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Struct")
	// UMaterialInterface* Decal;
	//
	// UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Struct")
	// FVector DecalSize = FVector(1.0f, 5.0f, 5.0f);
	//
	// UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Struct")
	// float DecalLifeSpan = 30.0f;

	// Operators
	// bool operator==(const FProjectileInfo& V) const;
	// bool operator!=(const FProjectileInfo& V) const;
};

FORCEINLINE FProjectileHitEffect::FProjectileHitEffect()
{
}

// FORCEINLINE FProjectileHitEffect::FProjectileHitEffect(UParticleSystem* InEmitter, const FVector InEmitterScale,
// 	UParticleSystem* InExplosiveEmitter, const FVector InExplosiveEmitterScale, USoundCue* InSound,
// 	UMaterialInterface* InDecal, const FVector InDecalSize, const float InDecalLifeSpan) : Emitter(InEmitter),
// 	EmitterScale(InEmitterScale), ExplosiveEmitter(InExplosiveEmitter), ExplosionEmitterScale(InExplosiveEmitterScale),
// 	Sound(InSound), Decal(InDecal), DecalSize(InDecalSize), DecalLifeSpan(InDecalLifeSpan)
// {
// }

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