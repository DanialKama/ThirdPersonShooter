// Copyright 2022-2023 Danial Kamali. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "GameFramework/Actor.h"
#include "Core/Enumerations/PickupEnums.h"
#include "Core/Structures/ProjectileInfo.h"
#include "Projectile.generated.h"

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
	USoundBase* FleshHitSound;
	
	UPROPERTY(EditDefaultsOnly)
	USoundBase* ObjectHitSound;
	
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

struct FHitInfo
{
	UMaterialInterface* Decal;
	UParticleSystem* Particle;
	USoundBase* Sound;
};

UCLASS(meta = (DisplayName = "Projectile"))
class AProjectile : public AActor
{
	GENERATED_BODY()

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = true))
	TObjectPtr<UStaticMeshComponent> StaticMesh;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = true))
	TObjectPtr<UParticleSystemComponent> TrailParticle;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = true))
	TObjectPtr<class UProjectileMovementComponent> ProjectileMovement;

// Functions
public:
	AProjectile();

protected:
	virtual void BeginPlay() override;

private:
	UFUNCTION()
	void OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);
	
	void HitEffect(const FHitResult& HitResult) const;
	float CalculatePointDamage(const FProjectileInfo* ProjectileInfo) const;

	/** Get projectile hit info */
	FHitInfo GetHitInfo() const;
	
	void SpawnFieldSystem(float StrainMagnitude, float ForceMagnitude, float TorqueMagnitude) const;

// Variables
public:
	UPROPERTY(EditDefaultsOnly, Category = "Default")
	int32 NumberOfPellets = 1;

	UPROPERTY(EditDefaultsOnly, Category = "Default")
	float PelletSpread = 0.0f;

private:
	UPROPERTY(EditDefaultsOnly, Category = "Default", meta = (AllowPrivateAccess = true))
	EAmmoType AmmoType = EAmmoType::None;
	
	UPROPERTY(EditDefaultsOnly, Category = "Default", meta = (AllowPrivateAccess = true))
	UDataTable* ProjectileDataTable;

	UPROPERTY(EditDefaultsOnly, Category = "Default", meta = (AllowPrivateAccess = true))
	uint8 bIsExplosive : 1;
	
	UPROPERTY(EditDefaultsOnly, Category = "Default", meta = (AllowPrivateAccess = true))
	UDataTable* ExplosiveProjectileDataTable;
	
	UPROPERTY(EditDefaultsOnly, Category = "Default", meta = (AllowPrivateAccess = true))
	TObjectPtr<UParticleSystem> FleshHitParticle;
	
	UPROPERTY(EditDefaultsOnly, Category = "Default", meta = (AllowPrivateAccess = true))
	TObjectPtr<UParticleSystem> WoodHitParticle;
	
	UPROPERTY(EditDefaultsOnly, Category = "Default", meta = (AllowPrivateAccess = true))
	TObjectPtr<UParticleSystem> MetalHitParticle;
	
	UPROPERTY(EditDefaultsOnly, Category = "Default", meta = (AllowPrivateAccess = true))
	TObjectPtr<UParticleSystem> StoneHitParticle;
	
	UPROPERTY(EditDefaultsOnly, Category = "Default", meta = (AllowPrivateAccess = true))
	TObjectPtr<UParticleSystem> DirtHitParticle;
	
	UPROPERTY(EditDefaultsOnly, Category = "Default", meta = (AllowPrivateAccess = true))
	TObjectPtr<UParticleSystem> ExplosiveParticle;
	
	UPROPERTY(EditDefaultsOnly, Category = "Default", meta = (AllowPrivateAccess = true))
	TObjectPtr<USoundBase> FleshHitSound;
	
	UPROPERTY(EditDefaultsOnly, Category = "Default", meta = (AllowPrivateAccess = true))
	TObjectPtr<USoundBase> ObjectHitSound;

	UPROPERTY(EditDefaultsOnly, Category = "Default", meta = (AllowPrivateAccess = true))
	FVector DecalSize = FVector::OneVector;
	
	UPROPERTY(EditDefaultsOnly, Category = "Default", meta = (AllowPrivateAccess = true))
	float DecalLifeSpan = 10.0f;
	
	UPROPERTY(EditDefaultsOnly, Category = "Default", meta = (AllowPrivateAccess = true))
	TObjectPtr<UMaterialInterface> FleshDecal;
	
	UPROPERTY(EditDefaultsOnly, Category = "Default", meta = (AllowPrivateAccess = true))
	TObjectPtr<UMaterialInterface> WoodDecal;
	
	UPROPERTY(EditDefaultsOnly, Category = "Default", meta = (AllowPrivateAccess = true))
	TObjectPtr<UMaterialInterface> MetalDecal;
	
	UPROPERTY(EditDefaultsOnly, Category = "Default", meta = (AllowPrivateAccess = true))
	TObjectPtr<UMaterialInterface> StoneDecal;
	
	UPROPERTY(EditDefaultsOnly, Category = "Default", meta = (AllowPrivateAccess = true))
	TObjectPtr<UMaterialInterface> DirtDecal;

	UPROPERTY(EditDefaultsOnly, Category = "Default", meta = (AllowPrivateAccess = true))
	TSubclassOf<UDamageType> DamageType;
	
	UPROPERTY(EditDefaultsOnly, Category = "Default", meta = (AllowPrivateAccess = true))
	TSubclassOf<AProjectileFieldSystemActor> ProjectileImpactFieldSystem;
	
	UPROPERTY(EditDefaultsOnly, Category = "Default", meta = (AllowPrivateAccess = true))
	FProjectileEffect ProjectileEffect;

	/** For switch on surface types */
	int32 SwitchExpression = 0;
};
