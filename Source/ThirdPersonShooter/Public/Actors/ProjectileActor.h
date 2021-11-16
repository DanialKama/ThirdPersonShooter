// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Enums/WeaponEnums.h"
#include "GameFramework/Actor.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "ProjectileActor.generated.h"

class USoundCue;

UCLASS()
class THIRDPERSONSHOOTER_API AProjectileActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AProjectileActor();
	
	// Variables
	UPROPERTY(EditDefaultsOnly, Category = "Projectile")
	int32 NumberOfPellets = 1;

	UPROPERTY(EditDefaultsOnly, Category = "Projectile")
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
	UPROPERTY(EditDefaultsOnly, Category = "Projectile")
	EAmmoType AmmoType;
	UPROPERTY(EditDefaultsOnly, Category = "Projectile")
	bool bIsExplosive;
	// UPROPERTY(EditDefaultsOnly, Category = "Projectile")
	

private:
	// Functions
	UFUNCTION()    
	void OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);
	void SwitchOnSurfaceType(EPhysicalSurface SurfaceType);	// Switch on surface type to execute the appropriate effect
	void HitEffect(UParticleSystem* Emitter, USoundCue* Sound, UMaterialInterface Decal, const FVector EmitterScale, FVector DecalSize, float DecalLifeSpan, bool bIsExplosive, const FHitResult HitResult);
	static FRotator CalculateEmitterRotation(FVector ImpactNormal);
	static FRotator CalculateDecalRotation(FVector ImpactNormal);
};
