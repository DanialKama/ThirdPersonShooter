// Copyright 2022 Danial Kamali. All Rights Reserved.

#include "Magazine.h"
#include "Kismet/GameplayStatics.h"
#include "Sound/SoundCue.h"

AMagazine::AMagazine()
{
	PrimaryActorTick.bCanEverTick = false;
	
	StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Magazine"));
	SetRootComponent(StaticMesh);
	StaticMesh->SetComponentTickEnabled(false);
	StaticMesh->bApplyImpulseOnDamage = false;
	StaticMesh->SetNotifyRigidBodyCollision(true);
	StaticMesh->SetCollisionProfileName("Magazine");
	StaticMesh->CanCharacterStepUpOn = ECB_No;
	StaticMesh->SetGenerateOverlapEvents(false);
	StaticMesh->OnComponentHit.AddDynamic(this, &AMagazine::OnHit);

	// Initialize variables
	bDoOnce = true;
	bMagazineIsEmpty = false;
}

void AMagazine::BeginPlay()
{
	Super::BeginPlay();

	if (bMagazineIsEmpty)
	{
		StaticMesh->SetStaticMesh(EmptyMagazine);
	}
	else
	{
		StaticMesh->SetStaticMesh(FullMagazine);
	}
}

void AMagazine::OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	if (bDoOnce)
	{
		const FCollisionResponseContainer CollisionResponse = OtherComp->GetCollisionResponseToChannels();
		if (CollisionResponse.WorldStatic == ECR_Block)
		{
			FCollisionResponseContainer NewResponse;
			NewResponse.Pawn = ECR_Ignore;
			NewResponse.PhysicsBody = ECR_Ignore;
			StaticMesh->SetCollisionResponseToChannels(NewResponse);
			UGameplayStatics::PlaySoundAtLocation(GetWorld(), HitSound, GetActorLocation());
			bDoOnce = false;
		}
		else
		{
			bDoOnce = true;
		}
	}
}