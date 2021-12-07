// Copyright Epic Games, Inc. All Rights Reserved.

#include "Actors/EmptyShell.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Sound/SoundCue.h"

// Sets default values
AEmptyShell::AEmptyShell()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	// Create components
	StaticMesh			= CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Empty Shell"));
	ProjectileMovement	= CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("Projectile Movement"));

	// Setup components attachment
	SetRootComponent(StaticMesh);

	// Set component defaults
	StaticMesh->SetNotifyRigidBodyCollision(true);
	StaticMesh->SetGenerateOverlapEvents(false);
	StaticMesh->SetCollisionObjectType(ECC_GameTraceChannel2);
	StaticMesh->OnComponentHit.AddDynamic(this, &AEmptyShell::OnHit);

	ProjectileMovement->InitialSpeed = 250.0f;
	ProjectileMovement->MaxSpeed = 250.0f;
	ProjectileMovement->ProjectileGravityScale = 2.0f;
	ProjectileMovement->Velocity = FVector(-1.0f, 0.0f, 0.0f);

	// Initialize variables
	bDoOnce = true;
}

// Called when the game starts or when spawned
void AEmptyShell::BeginPlay()
{
	Super::BeginPlay();
	
	// Simulate physics with a small delay to not collide with weapon but collide with ground
	FTimerHandle SimulatePhysicsTimer;
	GetWorldTimerManager().SetTimer(SimulatePhysicsTimer, this, &AEmptyShell::StartPhysics, 0.2f);
	StartPhysics();
}

void AEmptyShell::OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	if(bDoOnce)
	{
		const FCollisionResponseContainer CollisionResponse = OtherComp->GetCollisionResponseToChannels();
		if(CollisionResponse.WorldStatic == ECR_Block)
		{
			UGameplayStatics::PlaySoundAtLocation(GetWorld(), HitSound, GetActorLocation());
			bDoOnce = false;
		}
		else
		{
			bDoOnce = true;
		}
	}
}

void AEmptyShell::StartPhysics() const
{
	StaticMesh->SetSimulatePhysics(true);
}