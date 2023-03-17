// Copyright 2022-2023 Danial Kamali. All Rights Reserved.

#include "EmptyShell.h"

#include "GameFramework/ProjectileMovementComponent.h"
#include "Kismet/GameplayStatics.h"

AEmptyShell::AEmptyShell()
{
	PrimaryActorTick.bCanEverTick = false;
	PrimaryActorTick.bStartWithTickEnabled = false;
	SetCanBeDamaged(false);
	InitialLifeSpan = 3.0f;

	StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Empty Shell"));
	RootComponent = StaticMesh;
	StaticMesh->SetSimulatePhysics(true);
	StaticMesh->bApplyImpulseOnDamage = false;
	StaticMesh->CanCharacterStepUpOn = ECB_No;
	StaticMesh->SetCollisionProfileName("EmptyShell");
	StaticMesh->SetNotifyRigidBodyCollision(true);
	StaticMesh->SetGenerateOverlapEvents(false);
	StaticMesh->SetCanEverAffectNavigation(false);

	// TODO: Use impulse instead of projectile movement
	ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("Projectile Movement"));
	ProjectileMovement->InitialSpeed = 250.0f;
	ProjectileMovement->MaxSpeed = 250.0f;
	ProjectileMovement->ProjectileGravityScale = 2.0f;
	ProjectileMovement->Velocity = FVector(-1.0f, 0.0f, 0.0f);
	
	// Initialize variables
	bDoOnce = true;
}

void AEmptyShell::BeginPlay()
{
	Super::BeginPlay();

	StaticMesh->OnComponentHit.AddDynamic(this, &AEmptyShell::OnHit);
}

void AEmptyShell::OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	if (bDoOnce)
	{
		bDoOnce = false;
		StaticMesh->SetNotifyRigidBodyCollision(false);

		UGameplayStatics::PlaySoundAtLocation(GetWorld(), HitSound, StaticMesh->GetComponentLocation());
	}
}
