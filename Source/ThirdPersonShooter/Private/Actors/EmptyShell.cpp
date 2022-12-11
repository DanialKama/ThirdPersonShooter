// Copyright 2022 Danial Kamali. All Rights Reserved.

#include "EmptyShell.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Sound/SoundCue.h"

AEmptyShell::AEmptyShell()
{
	PrimaryActorTick.bCanEverTick = false;
	InitialLifeSpan = 3.0f;

	StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Empty Shell"));
	SetRootComponent(StaticMesh);
	StaticMesh->SetComponentTickEnabled(false);
	StaticMesh->SetSimulatePhysics(true);
	StaticMesh->bApplyImpulseOnDamage = false;
	StaticMesh->CanCharacterStepUpOn = ECB_No;
	StaticMesh->SetCollisionProfileName("EmptyShell");
	StaticMesh->SetNotifyRigidBodyCollision(true);
	StaticMesh->SetGenerateOverlapEvents(false);
	StaticMesh->OnComponentHit.AddDynamic(this, &AEmptyShell::OnHit);
	
	ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("Projectile Movement"));
	ProjectileMovement->InitialSpeed = 250.0f;
	ProjectileMovement->MaxSpeed = 250.0f;
	ProjectileMovement->ProjectileGravityScale = 2.0f;
	ProjectileMovement->Velocity = FVector(-1.0f, 0.0f, 0.0f);
	
	// Initialize variables
	bDoOnce = true;
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