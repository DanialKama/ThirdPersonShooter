// Copyright Epic Games, Inc. All Rights Reserved.

#include "Actors/ProjectileActor.h"
#include "Engine/DataTable.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "Particles/ParticleSystemComponent.h"
#include "UObject/ReflectedTypeAccessors.h"
// Structs
#include "Structs/ExplosiveProjectileInfoStruct.h"
#include "Structs/ProjectileInfoStruct.h"
#include "Structs/ProjectileHitEffectStruct.h"

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
	SetRootComponent(StaticMesh);
	TrailParticle->SetupAttachment(StaticMesh, TEXT("TrailSocket"));

	// Set component defaults
	StaticMesh->SetNotifyRigidBodyCollision(true);
	StaticMesh->SetGenerateOverlapEvents(true);
	StaticMesh->SetCollisionObjectType(ECollisionChannel::ECC_GameTraceChannel1);
	StaticMesh->bReturnMaterialOnMove = true;
	StaticMesh->OnComponentHit.AddDynamic(this, &AProjectileActor::OnHit);

	ProjectileMovement->InitialSpeed = 2000.0f;
	ProjectileMovement->MaxSpeed = 2000.0f;

	// Load data tables
	static ConstructorHelpers::FObjectFinder<UDataTable> ProjectileDataObject(TEXT("DataTable'/Game/Blueprints/Projectiles/DT_ProjectileInfo.DT_ProjectileInfo'"));
	if(ProjectileDataObject.Succeeded())
	{
		ProjectileDataTable = ProjectileDataObject.Object;
	}

	static ConstructorHelpers::FObjectFinder<UDataTable> ExplosiveProjectileDataObject(TEXT("DataTable'/Game/Blueprints/Projectiles/DT_ExplosiveProjectileInfo.DT_ExplosiveProjectileInfo'"));
	if(ProjectileDataObject.Succeeded())
	{
		ExplosiveProjectileDataTable = ExplosiveProjectileDataObject.Object;
	}
}

void AProjectileActor::OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	if(OtherActor != GetOwner()->GetOwner() && ProjectileHitEffectDataTable)	// Do not trigger by owner character TODO try not using this condition
	{
		const FName AmmoName = StaticEnum<EAmmoType>()->GetValueAsName(AmmoType);
		
		if(bIsExplosive && ExplosiveProjectileDataTable)
		{
			const FExplosiveProjectileInfo* ExplosiveProjectileInfo = ExplosiveProjectileDataTable->FindRow<FExplosiveProjectileInfo>(AmmoName, TEXT("Projectile Info Context"), true);
			if(ExplosiveProjectileInfo)
			{
				// Apply radial damage with fall off for explosive projectiles
				const TArray<AActor*> IgnoreActors;
				UGameplayStatics::ApplyRadialDamageWithFalloff(GetWorld(), ExplosiveProjectileInfo->BaseDamage, ExplosiveProjectileInfo->MinimumDamage, Hit.ImpactPoint, ExplosiveProjectileInfo->DamageInnerRadius, ExplosiveProjectileInfo->DamageOuterRadius, 2.0f, ExplosiveProjectileInfo->DamageType->StaticClass(), IgnoreActors, GetOwner(), GetInstigatorController(), ECollisionChannel::ECC_Visibility);
			}
		}
		else if(!bIsExplosive && ProjectileDataTable)
		{
			const FProjectileInfo* ProjectileInfo = ProjectileDataTable->FindRow<FProjectileInfo>(AmmoName, TEXT("Projectile Info Context"), true);
			if(ProjectileInfo)
			{
				// Apply point damage for nonexplosive projectiles based on surface type
				UGameplayStatics::ApplyPointDamage(Hit.GetActor(), CalculatePointDamage(ProjectileInfo, Hit), Hit.TraceStart, Hit, GetInstigatorController(), GetOwner(), ProjectileInfo->DamageType->StaticClass());
			}
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("No damage applied."));
		}
		HitEffect(Hit);
		Destroy();
	}
}

float AProjectileActor::CalculatePointDamage(const FProjectileInfo* ProjectileInfo, const FHitResult HitResult)
{
	if(HitResult.PhysMaterial.IsValid())
	{
		// Switch on surface type to calculate the appropriate damage
		switch (StaticEnum<EPhysicalSurface>()->GetIndexByValue(UGameplayStatics::GetSurfaceType(HitResult)))
		{
		case 0:
			// Default
			return ProjectileInfo->DefaultDamage;
		case 1:
			// Head Flesh
			return ProjectileInfo->DamageToHead;
		case 2:
			// Body Flesh
			return ProjectileInfo->DamageToBody;
		case 3:
			// Hand Flesh
			return ProjectileInfo->DamageToHand;
		case 4:
			// Leg Flesh
			return  ProjectileInfo->DamageToLeg;
		case 5:
			// Wood
			return ProjectileInfo->DamageToWood;
		case 6:
			// Metal
			return ProjectileInfo->DamageToMetal;
		case 7:
			// Stone
			return ProjectileInfo->DamageToStone;
		default: return ProjectileInfo->DefaultDamage;
		}
	}
	return ProjectileInfo->DefaultDamage;
}

void AProjectileActor::HitEffect(const FHitResult HitResult) const
{
	FProjectileHitEffect* ProjectileHitEffect;
	
	if(HitResult.PhysMaterial.IsValid())
	{
		const EPhysicalSurface SurfaceType = UGameplayStatics::GetSurfaceType(HitResult);
		const FName SurfaceName = FName(StaticEnum<EPhysicalSurface>()->GetDisplayNameTextByValue(SurfaceType).ToString());
		ProjectileHitEffect = ProjectileHitEffectDataTable->FindRow<FProjectileHitEffect>(SurfaceName, TEXT("Projectile Hit Effect Context"), true);
	}
	else
	{
		ProjectileHitEffect = ProjectileHitEffectDataTable->FindRow<FProjectileHitEffect>(TEXT("Default"), TEXT("Projectile Hit Effect Context"), true);
	}
	
	if(ProjectileHitEffect)
	{
		// Spawn impact emitter
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ProjectileHitEffect->Emitter, HitResult.ImpactPoint, CalculateDecalRotation(HitResult.ImpactNormal), ProjectileHitEffect->EmitterScale);
	
		// Spawn decal attached
		UGameplayStatics::SpawnDecalAttached(ProjectileHitEffect->Decal, ProjectileHitEffect->DecalSize, HitResult.GetComponent(),
			HitResult.BoneName, HitResult.ImpactPoint, CalculateDecalRotation(HitResult.ImpactNormal),
			EAttachLocation::KeepWorldPosition, ProjectileHitEffect->DecalLifeSpan);

		// Play sound at location
		UGameplayStatics::PlaySoundAtLocation(GetWorld(), ProjectileHitEffect->Sound, HitResult.ImpactPoint);

		// If projectile is explosive in addition to surface impact emitter another emitter spawn for explosion
		if(bIsExplosive)
		{
			UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ProjectileHitEffect->ExplosiveEmitter, HitResult.ImpactPoint, CalculateDecalRotation(HitResult.ImpactNormal), ProjectileHitEffect->ExplosionEmitterScale);
		}
	}
}

FRotator AProjectileActor::CalculateEmitterRotation(FVector ImpactNormal)
{
	const FRotator RotFromX = UKismetMathLibrary::MakeRotFromX(ImpactNormal);
	return UKismetMathLibrary::MakeRotator(RotFromX.Roll, RotFromX.Pitch, RotFromX.Yaw);
}

FRotator AProjectileActor::CalculateDecalRotation(const FVector ImpactNormal)
{
	const FRotator Rotator = UKismetMathLibrary::MakeRotFromX(ImpactNormal);
	if(Rotator.Pitch < 0)
	{
		return Rotator;
	}
	return UKismetMathLibrary::MakeRotator(Rotator.Roll, Rotator.Pitch + 180.0f, Rotator.Yaw);
}