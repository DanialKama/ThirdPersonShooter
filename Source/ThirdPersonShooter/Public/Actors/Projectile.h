// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "Enums/PickupEnums.h"
#include "GameFramework/Actor.h"
#include "Structs/ProjectileInfoStruct.h"
#include "Projectile.generated.h"

class USoundCue;
class UProjectileMovementComponent;

USTRUCT(BlueprintType)
struct FProjectileEffect
{
	GENERATED_BODY()
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Defaults", meta = (AllowPrivateAccess = "true"))
	uint8 bIsExplosive : 1;

	// Particle systems and particle spawn scale
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Defaults|Effects", meta = (AllowPrivateAccess = "true"))
	UParticleSystem* FleshHitEmitter;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Defaults|Effects", meta = (AllowPrivateAccess = "true"))
	UParticleSystem* WoodHitEmitter;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Defaults|Effects", meta = (AllowPrivateAccess = "true"))
	UParticleSystem* MetalHitEmitter;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Defaults|Effects", meta = (AllowPrivateAccess = "true"))
	UParticleSystem* StoneHitEmitter;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Defaults|Effects", meta = (AllowPrivateAccess = "true"))
	UParticleSystem* DirtHitEmitter;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Defaults|Effects", meta = (AllowPrivateAccess = "true"))
	UParticleSystem* ExplosiveEmitter;
	
	// Sound Cues
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Defaults|Effects", meta = (AllowPrivateAccess = "true"))
	USoundCue* FleshHitSound;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Defaults|Effects", meta = (AllowPrivateAccess = "true"))
	USoundCue* ObjectHitSound;
	
	// Decals, decal spawn size, and decal life span
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Defaults|Effects", meta = (AllowPrivateAccess = "true"))
	UMaterialInterface* FleshDecal;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Defaults|Effects", meta = (AllowPrivateAccess = "true"))
	FVector FleshDecalSize = FVector(1.0f, 5.0f, 5.0f);

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Defaults|Effects", meta = (AllowPrivateAccess = "true"))
	float FleshDecalLifeSpan = 20.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Defaults|Effects", meta = (AllowPrivateAccess = "true"))
	UMaterialInterface* WoodDecal;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Defaults|Effects", meta = (AllowPrivateAccess = "true"))
	UMaterialInterface* MetalDecal;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Defaults|Effects", meta = (AllowPrivateAccess = "true"))
	UMaterialInterface* StoneDecal;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Defaults|Effects", meta = (AllowPrivateAccess = "true"))
	UMaterialInterface* DirtDecal;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Defaults|Effects", meta = (AllowPrivateAccess = "true"))
	FVector ObjectDecalSize = FVector(5.0f, 10.0f, 10.0f);

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Defaults|Effects", meta = (AllowPrivateAccess = "true"))
	float ObjectDecalLifeSpan = 10.0f;
};

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

private:
	// Components
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UStaticMeshComponent> StaticMesh;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UParticleSystemComponent> TrailParticle;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UProjectileMovementComponent> ProjectileMovement;

	// Functions
	UFUNCTION()
	void OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

	void HitEffect(const FHitResult HitResult) const;

	float CalculatePointDamage(const FProjectileInfo* ProjectileInfo) const;
	
	void CalculateProjectileHitInfo(UParticleSystem*& Emitter,
		USoundCue*& Sound, UMaterialInterface*& Decal, FVector& DecalSize, float& DecalLifeSpan) const;

	// Variables
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Defaults", meta = (AllowPrivateAccess = "true"))
	EAmmoType AmmoType = EAmmoType::AssaultRifleNormal;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Defaults", meta = (AllowPrivateAccess = "true"))
	UDataTable* ProjectileDataTable;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Defaults", meta = (AllowPrivateAccess = "true"))
	UDataTable* ExplosiveProjectileDataTable;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Defaults", meta = (AllowPrivateAccess = "true"))
	TSubclassOf<UDamageType> DamageType;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Defaults", meta = (AllowPrivateAccess = "true"))
	FProjectileEffect ProjectileEffect;

	int32 SwitchExpression = 0;
};