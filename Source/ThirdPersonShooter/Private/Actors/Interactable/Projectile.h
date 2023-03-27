// Copyright 2022-2023 Danial Kamali. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "GameFramework/Actor.h"
#include "Core/Enumerations/PickupEnums.h"
#include "Core/Structures/ProjectileInfoStruct.h"
#include "Projectile.generated.h"

class USoundCue;
class AProjectileFieldSystemActor;

USTRUCT(BlueprintType)
struct FProjectileEffect
{
	GENERATED_BODY()
	
	UPROPERTY(EditDefaultsOnly)
	uint8 bIsExplosive : 1;
	
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
	
	UPROPERTY(EditDefaultsOnly)
	USoundCue* FleshHitSound;
	
	UPROPERTY(EditDefaultsOnly)
	USoundCue* ObjectHitSound;
	
	UPROPERTY(EditDefaultsOnly)
	UMaterialInterface* FleshDecal;
	
	UPROPERTY(EditDefaultsOnly)
	FVector FleshDecalSize;
	
	UPROPERTY(EditDefaultsOnly)
	float FleshDecalLifeSpan;
	
	UPROPERTY(EditDefaultsOnly)
	UMaterialInterface* WoodDecal;
	
	UPROPERTY(EditDefaultsOnly)
	UMaterialInterface* MetalDecal;
	
	UPROPERTY(EditDefaultsOnly)
	UMaterialInterface* StoneDecal;
	
	UPROPERTY(EditDefaultsOnly)
	UMaterialInterface* DirtDecal;
	
	UPROPERTY(EditDefaultsOnly)
	FVector ObjectDecalSize;
	
	UPROPERTY(EditDefaultsOnly)
	float ObjectDecalLifeSpan;

	// Default constructor
	FProjectileEffect()
	{
		bIsExplosive = false;
		FleshHitEmitter = nullptr;
		WoodHitEmitter = nullptr;
		MetalHitEmitter = nullptr;
		StoneHitEmitter = nullptr;
		DirtHitEmitter = nullptr;
		ExplosiveEmitter = nullptr;
		FleshHitSound = nullptr;
		ObjectHitSound = nullptr;
		FleshDecal = nullptr;
		FleshDecalSize = FVector(1.0f, 5.0f, 5.0f);
		FleshDecalLifeSpan = 20.0f;
		WoodDecal = nullptr;
		MetalDecal = nullptr;
		StoneDecal = nullptr;
		DirtDecal = nullptr;
		ObjectDecalSize = FVector(5.0f, 10.0f, 10.0f);
		ObjectDecalLifeSpan = 10.0f;
	}
};

UCLASS()
class AProjectile : public AActor
{
	GENERATED_BODY()

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = true))
	UStaticMeshComponent* StaticMesh;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = true))
	UParticleSystemComponent* TrailParticle;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = true))
	class UProjectileMovementComponent* ProjectileMovement;

// Functions
public:
	AProjectile();

private:
	UFUNCTION()
	void OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);
	
	void HitEffect(const FHitResult HitResult) const;
	float CalculatePointDamage(const FProjectileInfo* ProjectileInfo) const;
	void CalculateProjectileHitInfo(UParticleSystem*& Emitter, USoundCue*& Sound, UMaterialInterface*& Decal, FVector& DecalSize, float& DecalLifeSpan) const;
	void SpawnFieldSystem(float StrainMagnitude, float ForceMagnitude, float TorqueMagnitude) const;

// Variables
public:
	UPROPERTY(EditDefaultsOnly, Category = "Default")
	int32 NumberOfPellets;

	UPROPERTY(EditDefaultsOnly, Category = "Default")
	float PelletSpread;

private:
	UPROPERTY(EditDefaultsOnly, Category = "Default", meta = (AllowPrivateAccess = true))
	EAmmoType AmmoType;
	
	UPROPERTY(EditDefaultsOnly, Category = "Default", meta = (AllowPrivateAccess = true))
	UDataTable* ProjectileDataTable;

	UPROPERTY(EditDefaultsOnly, Category = "Default", meta = (AllowPrivateAccess = true))
	UDataTable* ExplosiveProjectileDataTable;

	UPROPERTY(EditDefaultsOnly, Category = "Default", meta = (AllowPrivateAccess = true))
	TSubclassOf<UDamageType> DamageType;
	
	UPROPERTY(EditDefaultsOnly, Category = "Default", meta = (AllowPrivateAccess = true))
	TSubclassOf<AProjectileFieldSystemActor> ProjectileImpactFieldSystem;
	
	UPROPERTY(EditDefaultsOnly, Category = "Default", meta = (AllowPrivateAccess = true))
	FProjectileEffect ProjectileEffect;

	/** For switch on surface types */
	int32 SwitchExpression;
};