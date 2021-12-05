// Copyright Epic Games, Inc. All Rights Reserved.

#include "Actors/Projectile.h"
#include "Engine/DataTable.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "Particles/ParticleSystemComponent.h"
#include "Sound/SoundCue.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Structs/ExplosiveProjectileInfoStruct.h"
#include "Structs/ProjectileInfoStruct.h"

// Sets default values
AProjectile::AProjectile()
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
	StaticMesh->SetGenerateOverlapEvents(false);
	StaticMesh->bReturnMaterialOnMove = true;
	StaticMesh->OnComponentHit.AddDynamic(this, &AProjectile::OnHit);

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

void AProjectile::OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	float SurfaceTypeIndex;
	if(Hit.PhysMaterial.IsValid())
	{
		SurfaceTypeIndex =	StaticEnum<EPhysicalSurface>()->GetIndexByValue(UGameplayStatics::GetSurfaceType(Hit));
	}
	else
	{
		SurfaceTypeIndex = 0.0f;
	}

	const FName AmmoName = StaticEnum<EAmmoType>()->GetValueAsName(AmmoType);
	
	if(bIsExplosive && ExplosiveProjectileDataTable)
	{
		const FExplosiveProjectileInfo* ExplosiveProjectileInfo = ExplosiveProjectileDataTable->FindRow<FExplosiveProjectileInfo>(AmmoName, TEXT("Projectile Info Context"), true);
		if(ExplosiveProjectileInfo)
		{
			// Apply radial damage with fall off for explosive projectiles
			const TArray<AActor*> IgnoreActors;
			UGameplayStatics::ApplyRadialDamageWithFalloff(GetWorld(), ExplosiveProjectileInfo->BaseDamage, ExplosiveProjectileInfo->MinimumDamage, Hit.ImpactPoint, ExplosiveProjectileInfo->DamageInnerRadius, ExplosiveProjectileInfo->DamageOuterRadius, 2.0f, DamageType, IgnoreActors, GetOwner(), GetInstigatorController(), ECollisionChannel::ECC_Visibility);
		}
	}
	else if(!bIsExplosive && ProjectileDataTable)
	{
		const FProjectileInfo* ProjectileInfo = ProjectileDataTable->FindRow<FProjectileInfo>(AmmoName, TEXT("Projectile Info Context"), true);
		if(ProjectileInfo)
		{
			// Apply point damage for nonexplosive projectiles based on surface type
			UGameplayStatics::ApplyPointDamage(Hit.GetActor(), CalculatePointDamage(ProjectileInfo, SurfaceTypeIndex), Hit.TraceStart, Hit, GetInstigatorController(), GetOwner(), DamageType);
		}
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("No damage applied."));
	}
	HitEffect(SurfaceTypeIndex, Hit);
	Destroy();
}

void AProjectile::HitEffect(const float SurfaceTypeIndex, const FHitResult HitResult) const
{
	UParticleSystem* Emitter;
	USoundCue* Sound;
	UMaterialInterface* Decal;
	FVector DecalSize;
	float DecalLifeSpan;
	
	CalculateProjectileHitInfo(SurfaceTypeIndex, Emitter, Sound, Decal, DecalSize, DecalLifeSpan);

	const FRotator SpawnRotation = UKismetMathLibrary::MakeRotFromX(HitResult.ImpactNormal);
	
	// Spawn impact emitter
	UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), Emitter, HitResult.ImpactPoint, SpawnRotation);
	
	// Spawn decal attached
	UGameplayStatics::SpawnDecalAttached(Decal, DecalSize, HitResult.GetComponent(),
		HitResult.BoneName, HitResult.ImpactPoint, SpawnRotation,
		EAttachLocation::KeepWorldPosition, DecalLifeSpan);

	// Play sound at location
	UGameplayStatics::PlaySoundAtLocation(GetWorld(), Sound, HitResult.ImpactPoint);

	// If projectile is explosive in addition to surface impact emitter another emitter spawn for explosion
	if(bIsExplosive)
	{
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ExplosiveEmitter, HitResult.ImpactPoint, SpawnRotation);
	}
}

float AProjectile::CalculatePointDamage(const FProjectileInfo* ProjectileInfo, const float SurfaceTypeIndex)
{
	const int32 SwitchExpression = FMath::FloorToInt(SurfaceTypeIndex);
	// Switch on surface type to calculate the appropriate damage
	switch (SwitchExpression)
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
	case 8:
		// Dirt
		return ProjectileInfo->DamageToDirt;
	default: return ProjectileInfo->DefaultDamage;
	}
}

void AProjectile::CalculateProjectileHitInfo(const float SurfaceTypeIndex, UParticleSystem*& Emitter,
	USoundCue*& Sound, UMaterialInterface*& Decal, FVector& DecalSize, float& DecalLifeSpan) const
{
	const int32 SwitchExpression = FMath::FloorToInt(SurfaceTypeIndex);
	// Switch on surface type to calculate the appropriate effect
	switch (SwitchExpression)
	{
	case 0:
		// Default
		Emitter = StoneHitEmitter;
		Sound = ObjectHitSound;
		Decal = StoneDecal;
		DecalSize = ObjectDecalSize;
		DecalLifeSpan = ObjectDecalLifeSpan;
		break;
	case 1:
		// Head Flesh
		Emitter = FleshHitEmitter;
		Sound = FleshHitSound;
		Decal = FleshDecal;
		DecalSize = FleshDecalSize;
		DecalLifeSpan = FleshDecalLifeSpan;
		break;
	case 2:
		// Body Flesh
		Emitter = FleshHitEmitter;
		Sound = FleshHitSound;
		Decal = FleshDecal;
		DecalSize = FleshDecalSize;
		DecalLifeSpan = FleshDecalLifeSpan;
		break;
	case 3:
		// Hand Flesh
		Emitter = FleshHitEmitter;
		Sound = FleshHitSound;
		Decal = FleshDecal;
		DecalSize = FleshDecalSize;
		DecalLifeSpan = FleshDecalLifeSpan;
		break;
	case 4:
		// Leg Flesh
		Emitter = FleshHitEmitter;
		Sound = FleshHitSound;
		Decal = FleshDecal;
		DecalSize = FleshDecalSize;
		DecalLifeSpan = FleshDecalLifeSpan;
		break;
	case 5:
		// Wood
		Emitter = WoodHitEmitter;
		Sound = ObjectHitSound;
		Decal = WoodDecal;
		DecalSize = ObjectDecalSize;
		DecalLifeSpan = ObjectDecalLifeSpan;
		break;
	case 6:
		// Metal
		Emitter = MetalHitEmitter;
		Sound = ObjectHitSound;
		Decal = MetalDecal;
		DecalSize = ObjectDecalSize;
		DecalLifeSpan = ObjectDecalLifeSpan;
		break;
	case 7:
		// Stone
		Emitter = StoneHitEmitter;
		Sound = ObjectHitSound;
		Decal = StoneDecal;
		DecalSize = ObjectDecalSize;
		DecalLifeSpan = ObjectDecalLifeSpan;
		break;
	case 8:
		// Dirt
		Emitter = DirtHitEmitter;
		Sound = ObjectHitSound;
		Decal = DirtDecal;
		DecalSize = ObjectDecalSize;
		DecalLifeSpan = ObjectDecalLifeSpan;
		break;
	default:
		Emitter = StoneHitEmitter;
		Sound = ObjectHitSound;
		Decal = StoneDecal;
		DecalSize = ObjectDecalSize;
		DecalLifeSpan = ObjectDecalLifeSpan;
	}
}