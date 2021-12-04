// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "Enums/PickupEnums.h"
#include "GameFramework/Actor.h"
#include "Structs/ProjectileInfoStruct.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Projectile.generated.h"

class USoundCue;

UCLASS()
class THIRDPERSONSHOOTER_API AProjectile : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AProjectile();
	
	// Variables
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Defaults")
	int32 NumberOfPellets = 1;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Defaults")
	float PelletSpread = 0.0f;

protected:
	// Components
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
	UStaticMeshComponent* StaticMesh;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
	UParticleSystemComponent* TrailParticle;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
	UProjectileMovementComponent* ProjectileMovement;

	// Variables
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Defaults")
	EAmmoType AmmoType = EAmmoType::AssaultRifleNormal;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Defaults")
	uint8 bIsExplosive : 1;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Defaults")
	UDataTable* ProjectileDataTable;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Defaults")
	UDataTable* ExplosiveProjectileDataTable;

	// UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Defaults")
	// UDataTable* ProjectileHitEffectDataTable;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Defaults")
	TSubclassOf<UDamageType> DamageType;

	// Particle systems and particle spawn scale
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Defaults|Effects")
	UParticleSystem* FleshHitEmitter;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Defaults|Effects")
	UParticleSystem* WoodHitEmitter;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Defaults|Effects")
	UParticleSystem* MetalHitEmitter;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Defaults|Effects")
	UParticleSystem* StoneHitEmitter;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Defaults|Effects")
	UParticleSystem* DirtHitEmitter;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Defaults|Effects")
	UParticleSystem* ExplosiveEmitter;
	
	// Sound Cues
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Defaults|Effects")
	USoundCue* FleshHitSound;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Defaults|Effects")
	USoundCue* ObjectHitSound;
	
	// Decals, decal spawn size, and decal life span
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Defaults|Effects")
	UMaterialInterface* FleshDecal;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Defaults|Effects")
	FVector FleshDecalSize = FVector(1.0f, 5.0f, 5.0f);

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Defaults|Effects")
	float FleshDecalLifeSpan = 20.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Defaults|Effects")
	UMaterialInterface* WoodDecal;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Defaults|Effects")
	UMaterialInterface* MetalDecal;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Defaults|Effects")
	UMaterialInterface* StoneDecal;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Defaults|Effects")
	UMaterialInterface* DirtDecal;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Defaults|Effects")
	FVector ObjectDecalSize = FVector(5.0f, 10.0f, 10.0f);

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Defaults|Effects")
	float ObjectDecalLifeSpan = 10.0f;
	
private:
	// Functions
	UFUNCTION()
	void OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

	void HitEffect(const float SurfaceTypeIndex, const FHitResult HitResult) const;

	static float CalculatePointDamage(const FProjectileInfo* ProjectileInfo, const float SurfaceTypeIndex);
	
	void CalculateProjectileHitInfo(const float SurfaceTypeIndex, UParticleSystem*& Emitter,
		USoundCue*& Sound, UMaterialInterface*& Decal, FVector& DecalSize, float& DecalLifeSpan) const;
};