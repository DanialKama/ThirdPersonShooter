// Copyright 2022-2023 Danial Kamali. All Rights Reserved.

#include "Projectile.h"

#include "Actors/NonInteractive/ProjectileFieldSystemActor.h"
#include "Core/Structures/ProjectileInfo.h"
#include "Engine/DataTable.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "Particles/ParticleSystemComponent.h"
#include "Perception/AISense_Damage.h"

AProjectile::AProjectile()
{
	PrimaryActorTick.bCanEverTick = false;
	PrimaryActorTick.bStartWithTickEnabled = false;

	StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Projectile Mesh"));
	SetRootComponent(StaticMesh);
	StaticMesh->SetNotifyRigidBodyCollision(true);
	StaticMesh->CanCharacterStepUpOn = ECB_No;
	StaticMesh->SetCollisionProfileName("Projectile");
	StaticMesh->SetGenerateOverlapEvents(false);
	StaticMesh->bReturnMaterialOnMove = true;
	
	TrailParticle = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("Trail Particle"));
	TrailParticle->SetupAttachment(StaticMesh, TEXT("TrailSocket"));
	
	ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("Projectile Movement"));
	ProjectileMovement->InitialSpeed = 2000.0f;
	ProjectileMovement->MaxSpeed = 2000.0f;

	// Initialize variables
	bIsExplosive = false;

	// TODO: Use the blueprint instead of loading them like this
	// Load data tables
	static ConstructorHelpers::FObjectFinder<UDataTable> ProjectileDataObject(TEXT("DataTable'/Game/Blueprints/Projectiles/DT_ProjectileInfo.DT_ProjectileInfo'"));
	if (ProjectileDataObject.Succeeded())
	{
		ProjectileDataTable = ProjectileDataObject.Object;
	}

	static ConstructorHelpers::FObjectFinder<UDataTable> ExplosiveProjectileDataObject(TEXT("DataTable'/Game/Blueprints/Projectiles/DT_ExplosiveProjectileInfo.DT_ExplosiveProjectileInfo'"));
	if (ProjectileDataObject.Succeeded())
	{
		ExplosiveProjectileDataTable = ExplosiveProjectileDataObject.Object;
	}
}

void AProjectile::BeginPlay()
{
	Super::BeginPlay();

	StaticMesh->OnComponentHit.AddDynamic(this, &AProjectile::OnHit);
}

void AProjectile::OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	if (Hit.PhysMaterial.IsValid())
	{
		SwitchExpression = StaticEnum<EPhysicalSurface>()->GetIndexByValue(UGameplayStatics::GetSurfaceType(Hit));
	}
	else
	{
		SwitchExpression = 0;
	}
	
	float AppliedDamage = 0.0f;
	const FName AmmoName = StaticEnum<EAmmoType>()->GetValueAsName(AmmoType);
	if (ProjectileEffect.bIsExplosive && ExplosiveProjectileDataTable)
	{
		// If row found
		if (const FExplosiveProjectileInfo* ExplosiveProjectileInfo = ExplosiveProjectileDataTable->FindRow<FExplosiveProjectileInfo>(AmmoName, TEXT("Projectile Info Context"), true))
		{
			SpawnFieldSystem(ExplosiveProjectileInfo->StrainMagnitude, ExplosiveProjectileInfo->ForceMagnitude, ExplosiveProjectileInfo->TorqueMagnitude);

			// Apply radial damage with fall off for explosive projectiles
			const TArray<AActor*> IgnoreActors;
			UGameplayStatics::ApplyRadialDamageWithFalloff(GetWorld(), ExplosiveProjectileInfo->BaseDamage, ExplosiveProjectileInfo->MinimumDamage, Hit.ImpactPoint, ExplosiveProjectileInfo->DamageInnerRadius, ExplosiveProjectileInfo->DamageOuterRadius, 2.0f, DamageType, IgnoreActors, GetOwner(), GetInstigatorController(), ECollisionChannel::ECC_Visibility);
			AppliedDamage = ExplosiveProjectileInfo->BaseDamage;
		}
	}
	else if (ProjectileDataTable)
	{
		// If row found
		if (const FProjectileInfo* ProjectileInfo = ProjectileDataTable->FindRow<FProjectileInfo>(AmmoName, TEXT("Projectile Info Context"), true))
		{
			SpawnFieldSystem(ProjectileInfo->StrainMagnitude, ProjectileInfo->ForceMagnitude, ProjectileInfo->TorqueMagnitude);

			// Apply point damage for nonexplosive projectiles based on surface type
			AppliedDamage = UGameplayStatics::ApplyPointDamage(Hit.GetActor(), CalculatePointDamage(ProjectileInfo), Hit.TraceStart, Hit, GetInstigatorController(), GetOwner(), DamageType);
		}
	}
	
	HitEffect(Hit);
	UAISense_Damage::ReportDamageEvent(GetWorld(), OtherActor, GetInstigator(), AppliedDamage, Hit.TraceStart, Hit.ImpactPoint);
	Destroy();
}

// TODO: Test the destruction without field system
void AProjectile::SpawnFieldSystem(float StrainMagnitude, float ForceMagnitude, float TorqueMagnitude) const
{
	const FTransform Transform = GetActorTransform();
	AProjectileFieldSystemActor* FieldSystem = GetWorld()->SpawnActorDeferred<AProjectileFieldSystemActor>(ProjectileImpactFieldSystem, Transform, nullptr, nullptr, ESpawnActorCollisionHandlingMethod::AlwaysSpawn);
	FieldSystem->StrainMagnitude = StrainMagnitude;
	FieldSystem->ForceMagnitude = ForceMagnitude;
	FieldSystem->TorqueMagnitude = TorqueMagnitude;
	UGameplayStatics::FinishSpawningActor(FieldSystem, Transform);
}

void AProjectile::HitEffect(const FHitResult& HitResult) const
{
	const auto [Decal, Particle, Sound] = GetHitInfo();

	const FRotator SpawnRotation = UKismetMathLibrary::MakeRotFromX(HitResult.ImpactNormal);

	// Spawn decal attached to hit component
	UGameplayStatics::SpawnDecalAttached(Decal, DecalSize, HitResult.GetComponent(), HitResult.BoneName, HitResult.ImpactPoint,
		SpawnRotation, EAttachLocation::KeepWorldPosition, DecalLifeSpan);
	
	// Spawn impact emitter
	UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), Particle, HitResult.ImpactPoint, SpawnRotation);

	// Play sound at the impact location
	UGameplayStatics::PlaySoundAtLocation(GetWorld(), Sound, HitResult.ImpactPoint);

	// If the projectile is explosive in addition to the surface impact emitter another emitter spawn for the explosion
	if (ProjectileEffect.bIsExplosive)
	{
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ProjectileEffect.ExplosiveEmitter, HitResult.ImpactPoint, SpawnRotation);
	}
}

float AProjectile::CalculatePointDamage(const FProjectileInfo* ProjectileInfo) const
{
	// Switch on surface types to calculate the appropriate damage
	switch (SwitchExpression)
	{
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
	case 0: default:
		return ProjectileInfo->DefaultDamage;
	}
}

FHitInfo AProjectile::GetHitInfo() const
{
	FHitInfo Result;
	
	// Switch on surface types to calculate the appropriate effect
	switch (SwitchExpression)
	{
	case 1: case 2: case 3: case 4:
		// Head Flesh, Body Flesh, Hand Flesh, Leg Flesh
		Result.Decal = FleshDecal;
		Result.Particle = FleshHitParticle;
		Result.Sound = FleshHitSound;
		return Result;
	case 5:
		// Wood
		Result.Decal = WoodDecal;
		Result.Particle = WoodHitParticle;
		Result.Sound = ObjectHitSound;
		return Result;
	case 6:
		// Metal
		Result.Decal = MetalDecal;
		Result.Particle = MetalHitParticle;
		Result.Sound = ObjectHitSound;
		return Result;
	case 7:
		// Stone
		Result.Decal = StoneDecal;
		Result.Particle = StoneHitParticle;
		Result.Sound = ObjectHitSound;
		return Result;
	case 8:
		// Dirt
		Result.Decal = DirtDecal;
		Result.Particle = DirtHitParticle;
		Result.Sound = ObjectHitSound;
		return Result;
	case 0: default:
		Result.Decal = StoneDecal;
		Result.Particle = StoneHitParticle;
		Result.Sound = ObjectHitSound;
		return Result;
	}
}
