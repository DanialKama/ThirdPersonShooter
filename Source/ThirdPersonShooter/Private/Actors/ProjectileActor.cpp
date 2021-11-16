// Copyright Epic Games, Inc. All Rights Reserved.

#include "Actors/ProjectileActor.h"
#include "K2Node_GetDataTableRow.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "Particles/ParticleSystemComponent.h"

// Sets default values
AProjectileActor::AProjectileActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Create components
	StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Projectile Mesh"));
	TrailParticle = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("Trail Particle"));
	ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("Projectile Movement"));

	// Setup attachment
	StaticMesh->SetupAttachment(GetRootComponent());
	TrailParticle->SetupAttachment(StaticMesh);

	// Set component defaults
	StaticMesh->SetNotifyRigidBodyCollision(true);
	StaticMesh->SetGenerateOverlapEvents(true);
	StaticMesh->SetCollisionObjectType(ECollisionChannel::ECC_EngineTraceChannel1);
	StaticMesh->bReturnMaterialOnMove = true;
	StaticMesh->OnComponentHit.AddDynamic(this, &AProjectileActor::OnHit);

	ProjectileMovement->InitialSpeed = 2000.0f;
	ProjectileMovement->MaxSpeed = 2000.0f;
}

void AProjectileActor::OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	if(OtherActor != GetOwner()->GetOwner())	// Do not trigger by owner character
	{
		if(bIsExplosive)
		{
			// TODO Data Table
			// UK2Node_GetDataTableRow::
			// UGameplayStatics::ApplyRadialDamageWithFalloff(GetWorld(), 0, 0, Hit.ImpactPoint, 0, 0, 0, DamageTypeClass, nullptr, GetOwner(), GetInstigatorController(), ECollisionChannel::ECC_Visibility);
			if(Hit.PhysMaterial.IsValid())
			{
				SwitchOnSurfaceType(UGameplayStatics::GetSurfaceType(Hit));
				Destroy();
			}
			else
			{
				
			}
		}
		else
		{
			
		}
	}
}

void AProjectileActor::SwitchOnSurfaceType(EPhysicalSurface SurfaceType)
{
	
}

void AProjectileActor::HitEffect(UParticleSystem* Emitter, USoundCue* Sound, UMaterialInterface Decal, const FVector EmitterScale, FVector DecalSize, float DecalLifeSpan, bool bIsExplosive, const FHitResult HitResult)
{
	UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), Emitter, HitResult.ImpactPoint, CalculateDecalRotation(HitResult.ImpactNormal), EmitterScale);
	// TODO
}

FRotator AProjectileActor::CalculateEmitterRotation(FVector ImpactNormal)
{
	const FRotator RotFromX = UKismetMathLibrary::MakeRotFromX(ImpactNormal);
	return UKismetMathLibrary::MakeRotator(RotFromX.Roll, RotFromX.Pitch, RotFromX.Yaw);
}

FRotator AProjectileActor::CalculateDecalRotation(const FVector ImpactNormal)
{
	if(ImpactNormal.Y < 0)
	{
		return UKismetMathLibrary::MakeRotator(ImpactNormal.X, ImpactNormal.Y, ImpactNormal.Z);
	}
	return UKismetMathLibrary::MakeRotator(ImpactNormal.X, ImpactNormal.Y + 180.0f, ImpactNormal.Z);
}