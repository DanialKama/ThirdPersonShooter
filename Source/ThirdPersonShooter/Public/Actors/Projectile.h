// All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "Enums/PickupEnums.h"
#include "GameFramework/Actor.h"
#include "Structs/ProjectileInfoStruct.h"
#include "Projectile.generated.h"

class USoundCue;
class UProjectileMovementComponent;
class AProjectileFieldSystemActor;

USTRUCT(BlueprintType)
struct FProjectileEffect
{
	GENERATED_BODY()
	
	UPROPERTY(EditDefaultsOnly)
	uint8 bIsExplosive : 1;
	// Particle systems and particle spawn scale
	UPROPERTY(EditDefaultsOnly)
	UParticleSystem* FleshHitEmitter;
	UPROPERTY(EditDefaultsOnly)
	UParticleSystem* WoodHitEmitter;
	UPROPERTY(EditDefaultsOnly)
	UParticleSystem* MetalHitEmitter;
	UPROPERTY(EditDefaultsOnly)
	UParticleSystem* StoneHitEmitter;
	UPROPERTY(EditDefaultsOnly)
	UParticleSystem* DirtHitEmitter;
	UPROPERTY(EditDefaultsOnly)
	UParticleSystem* ExplosiveEmitter;
	// Sound Cues
	UPROPERTY(EditDefaultsOnly)
	USoundCue* FleshHitSound;
	UPROPERTY(EditDefaultsOnly)
	USoundCue* ObjectHitSound;
	// Decals, decal spawn size, and decal life span
	UPROPERTY(EditDefaultsOnly)
	UMaterialInterface* FleshDecal;
	UPROPERTY(EditDefaultsOnly)
	FVector FleshDecalSize = FVector(1.0f, 5.0f, 5.0f);
	UPROPERTY(EditDefaultsOnly)
	float FleshDecalLifeSpan = 20.0f;
	UPROPERTY(EditDefaultsOnly)
	UMaterialInterface* WoodDecal;
	UPROPERTY(EditDefaultsOnly)
	UMaterialInterface* MetalDecal;
	UPROPERTY(EditDefaultsOnly)
	UMaterialInterface* StoneDecal;
	UPROPERTY(EditDefaultsOnly)
	UMaterialInterface* DirtDecal;
	UPROPERTY(EditDefaultsOnly)
	FVector ObjectDecalSize = FVector(5.0f, 10.0f, 10.0f);
	UPROPERTY(EditDefaultsOnly)
	float ObjectDecalLifeSpan = 10.0f;
};

UCLASS()
class THIRDPERSONSHOOTER_API AProjectile : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AProjectile();
	
	// Components
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UStaticMeshComponent* StaticMesh;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UParticleSystemComponent* TrailParticle;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UProjectileMovementComponent* ProjectileMovement;
	
	// Variables
	UPROPERTY(EditDefaultsOnly, Category = "Defaults")
	int32 NumberOfPellets = 1;

	UPROPERTY(EditDefaultsOnly, Category = "Defaults")
	float PelletSpread = 0.0f;

private:
	// Functions
	UFUNCTION()
	void OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);
	void HitEffect(const FHitResult HitResult) const;
	float CalculatePointDamage(const FProjectileInfo* ProjectileInfo) const;
	void CalculateProjectileHitInfo(UParticleSystem*& Emitter, USoundCue*& Sound, UMaterialInterface*& Decal, FVector& DecalSize, float& DecalLifeSpan) const;
	void SpawnFieldSystem(float StrainMagnitude, float ForceMagnitude, float TorqueMagnitude) const;

	// Variables
	UPROPERTY(EditDefaultsOnly, Category = "Defaults", meta = (AllowPrivateAccess = "true"))
	EAmmoType AmmoType = EAmmoType::AssaultRifleNormal;
	
	UPROPERTY(EditDefaultsOnly, Category = "Defaults", meta = (AllowPrivateAccess = "true"))
	UDataTable* ProjectileDataTable;

	UPROPERTY(EditDefaultsOnly, Category = "Defaults", meta = (AllowPrivateAccess = "true"))
	UDataTable* ExplosiveProjectileDataTable;

	UPROPERTY(EditDefaultsOnly, Category = "Defaults", meta = (AllowPrivateAccess = "true"))
	TSubclassOf<UDamageType> DamageType;
	
	UPROPERTY(EditDefaultsOnly, Category = "Defaults", meta = (AllowPrivateAccess = "true"))
	TSubclassOf<AProjectileFieldSystemActor> ProjectileImpactFieldSystem;
	
	UPROPERTY(EditDefaultsOnly, Category = "Defaults", meta = (AllowPrivateAccess = "true"))
	FProjectileEffect ProjectileEffect;

	int32 SwitchExpression = 0;
};