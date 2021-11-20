// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "Enums/WeaponEnums.h"
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
	bool bIsExplosive;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Defaults")
	UDataTable* ProjectileDataTable;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Defaults")
	UDataTable* ExplosiveProjectileDataTable;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Defaults")
	UDataTable* ProjectileHitEffectDataTable;
	
private:
	// Functions
	UFUNCTION()
	void OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);
	
	static float CalculatePointDamage(const FProjectileInfo* ProjectileInfo, const FHitResult HitResult);
	
	void HitEffect(const FHitResult HitResult) const;
	
	static FRotator CalculateEmitterRotation(FVector ImpactNormal);
	
	static FRotator CalculateDecalRotation(FVector ImpactNormal);

	void IHateMyLife(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);
};
