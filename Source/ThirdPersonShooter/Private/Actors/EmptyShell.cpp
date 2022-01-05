// All Rights Reserved.

#include "Actors/EmptyShell.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Sound/SoundCue.h"

AEmptyShell::AEmptyShell()
{
	PrimaryActorTick.bCanEverTick = false;
	InitialLifeSpan = 3.0f;

	// Create components
	StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Empty Shell"));
	ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("Projectile Movement"));

	// Attach components
	SetRootComponent(StaticMesh);

	// Initialize components
	StaticMesh->SetComponentTickEnabled(false);
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

void AEmptyShell::BeginPlay()
{
	Super::BeginPlay();
	
	// Simulate physics with a small delay to not collide with weapon but collide with ground
	FTimerHandle SimulatePhysicsTimer;
	GetWorld()->GetTimerManager().SetTimer(SimulatePhysicsTimer, this, &AEmptyShell::StartPhysics, 0.2f);
	StartPhysics();
}

void AEmptyShell::StartPhysics() const
{
	StaticMesh->SetSimulatePhysics(true);
}

void AEmptyShell::OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	if (bDoOnce)
	{
		const FCollisionResponseContainer CollisionResponse = OtherComp->GetCollisionResponseToChannels();
		if (CollisionResponse.WorldStatic == ECR_Block)
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