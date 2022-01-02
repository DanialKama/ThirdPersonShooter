// All Rights Reserved.

#include "Actors/Projectile.h"
#include "Actors/ProjectileFieldSystemActor.h"
#include "Engine/DataTable.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "Particles/ParticleSystemComponent.h"
#include "Sound/SoundCue.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Perception/AISense_Damage.h"
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

	// Initialize components
	StaticMesh->SetNotifyRigidBodyCollision(true);
	StaticMesh->SetGenerateOverlapEvents(false);
	StaticMesh->bReturnMaterialOnMove = true;
	StaticMesh->OnComponentHit.AddDynamic(this, &AProjectile::OnHit);

	ProjectileMovement->InitialSpeed = 2000.0f;
	ProjectileMovement->MaxSpeed = 2000.0f;

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

void AProjectile::OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	float AppliedDamage = 0.0f;
	if (Hit.PhysMaterial.IsValid())
	{
		SwitchExpression = StaticEnum<EPhysicalSurface>()->GetIndexByValue(UGameplayStatics::GetSurfaceType(Hit));
	}
	else
	{
		SwitchExpression = 0;
	}

	const FName AmmoName = StaticEnum<EAmmoType>()->GetValueAsName(AmmoType);
	
	if (ProjectileEffect.bIsExplosive && ExplosiveProjectileDataTable)
	{
		const FExplosiveProjectileInfo* ExplosiveProjectileInfo = ExplosiveProjectileDataTable->FindRow<FExplosiveProjectileInfo>(AmmoName, TEXT("Projectile Info Context"), true);
		if (ExplosiveProjectileInfo)
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
		const FProjectileInfo* ProjectileInfo = ProjectileDataTable->FindRow<FProjectileInfo>(AmmoName, TEXT("Projectile Info Context"), true);
		if (ProjectileInfo)
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

void AProjectile::SpawnFieldSystem(float StrainMagnitude, float ForceMagnitude, float TorqueMagnitude) const
{
	const FTransform Transform = GetActorTransform();
	AProjectileFieldSystemActor* FieldSystem = GetWorld()->SpawnActorDeferred<AProjectileFieldSystemActor>(ProjectileImpactFieldSystem, Transform, nullptr, nullptr, ESpawnActorCollisionHandlingMethod::AlwaysSpawn);
	FieldSystem->StrainMagnitude = StrainMagnitude;
	FieldSystem->ForceMagnitude = ForceMagnitude;
	FieldSystem->TorqueMagnitude = TorqueMagnitude;
	UGameplayStatics::FinishSpawningActor(FieldSystem, Transform);
}

void AProjectile::HitEffect(const FHitResult HitResult) const
{
	UParticleSystem* Emitter;
	USoundCue* Sound;
	UMaterialInterface* Decal;
	FVector DecalSize;
	float DecalLifeSpan;
	
	CalculateProjectileHitInfo(Emitter, Sound, Decal, DecalSize, DecalLifeSpan);

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
	if (ProjectileEffect.bIsExplosive)
	{
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ProjectileEffect.ExplosiveEmitter, HitResult.ImpactPoint, SpawnRotation);
	}
}

float AProjectile::CalculatePointDamage(const FProjectileInfo* ProjectileInfo) const
{
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

void AProjectile::CalculateProjectileHitInfo(UParticleSystem*& Emitter,
	USoundCue*& Sound, UMaterialInterface*& Decal, FVector& DecalSize, float& DecalLifeSpan) const
{
	// Switch on surface type to calculate the appropriate effect
	switch (SwitchExpression)
	{
	case 0:
		// Default
		Emitter			= ProjectileEffect.StoneHitEmitter;
		Sound			= ProjectileEffect.ObjectHitSound;
		Decal			= ProjectileEffect.StoneDecal;
		DecalSize		= ProjectileEffect.ObjectDecalSize;
		DecalLifeSpan	= ProjectileEffect.ObjectDecalLifeSpan;
		break;
	case 1:
		// Head Flesh
		Emitter			= ProjectileEffect.FleshHitEmitter;
		Sound			= ProjectileEffect.FleshHitSound;
		Decal			= ProjectileEffect.FleshDecal;
		DecalSize		= ProjectileEffect.FleshDecalSize;
		DecalLifeSpan	= ProjectileEffect.FleshDecalLifeSpan;
		break;
	case 2:
		// Body Flesh
		Emitter			= ProjectileEffect.FleshHitEmitter;
		Sound			= ProjectileEffect.FleshHitSound;
		Decal			= ProjectileEffect.FleshDecal;
		DecalSize		= ProjectileEffect.FleshDecalSize;
		DecalLifeSpan	= ProjectileEffect.FleshDecalLifeSpan;
		break;
	case 3:
		// Hand Flesh
		Emitter			= ProjectileEffect.FleshHitEmitter;
		Sound			= ProjectileEffect.FleshHitSound;
		Decal			= ProjectileEffect.FleshDecal;
		DecalSize		= ProjectileEffect.FleshDecalSize;
		DecalLifeSpan	= ProjectileEffect.FleshDecalLifeSpan;
		break;
	case 4:
		// Leg Flesh
		Emitter			= ProjectileEffect.FleshHitEmitter;
		Sound			= ProjectileEffect.FleshHitSound;
		Decal			= ProjectileEffect.FleshDecal;
		DecalSize		= ProjectileEffect.FleshDecalSize;
		DecalLifeSpan	= ProjectileEffect.FleshDecalLifeSpan;
		break;
	case 5:
		// Wood
		Emitter			= ProjectileEffect.WoodHitEmitter;
		Sound			= ProjectileEffect.ObjectHitSound;
		Decal			= ProjectileEffect.WoodDecal;
		DecalSize		= ProjectileEffect.ObjectDecalSize;
		DecalLifeSpan	= ProjectileEffect.ObjectDecalLifeSpan;
		break;
	case 6:
		// Metal
		Emitter			= ProjectileEffect.MetalHitEmitter;
		Sound			= ProjectileEffect.ObjectHitSound;
		Decal			= ProjectileEffect.MetalDecal;
		DecalSize		= ProjectileEffect.ObjectDecalSize;
		DecalLifeSpan	= ProjectileEffect.ObjectDecalLifeSpan;
		break;
	case 7:
		// Stone
		Emitter			= ProjectileEffect.StoneHitEmitter;
		Sound			= ProjectileEffect.ObjectHitSound;
		Decal			= ProjectileEffect.StoneDecal;
		DecalSize		= ProjectileEffect.ObjectDecalSize;
		DecalLifeSpan	= ProjectileEffect.ObjectDecalLifeSpan;
		break;
	case 8:
		// Dirt
		Emitter			= ProjectileEffect.DirtHitEmitter;
		Sound			= ProjectileEffect.ObjectHitSound;
		Decal			= ProjectileEffect.DirtDecal;
		DecalSize		= ProjectileEffect.ObjectDecalSize;
		DecalLifeSpan	= ProjectileEffect.ObjectDecalLifeSpan;
		break;
	default:
		Emitter			= ProjectileEffect.StoneHitEmitter;
		Sound			= ProjectileEffect.ObjectHitSound;
		Decal			= ProjectileEffect.StoneDecal;
		DecalSize		= ProjectileEffect.ObjectDecalSize;
		DecalLifeSpan	= ProjectileEffect.ObjectDecalLifeSpan;
	}
}