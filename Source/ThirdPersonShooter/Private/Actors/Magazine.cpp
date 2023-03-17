// Copyright 2022-2023 Danial Kamali. All Rights Reserved.

#include "Magazine.h"

#include "Kismet/GameplayStatics.h"

AMagazine::AMagazine()
{
	PrimaryActorTick.bCanEverTick = false;
	PrimaryActorTick.bStartWithTickEnabled = false;
	SetCanBeDamaged(false);
	
	StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Magazine"));
	RootComponent = StaticMesh;
	StaticMesh->bApplyImpulseOnDamage = false;
	StaticMesh->SetCollisionProfileName("Magazine");	// TODO: Update the collision profile
	StaticMesh->CanCharacterStepUpOn = ECB_No;
	StaticMesh->SetCanEverAffectNavigation(false);
	StaticMesh->SetGenerateOverlapEvents(false);
	StaticMesh->SetNotifyRigidBodyCollision(true);

	// Initialize variables
	bDoOnceHit = true;
	bIsEmpty = false;
}

void AMagazine::BeginPlay()
{
	Super::BeginPlay();

	if (bIsEmpty)
	{
		StaticMesh->SetStaticMesh(EmptyMagazine);
	}
	else
	{
		StaticMesh->SetStaticMesh(FullMagazine);
	}

	StaticMesh->OnComponentHit.AddDynamic(this, &AMagazine::OnHit);
}

void AMagazine::OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	if (bDoOnceHit)
	{
		bDoOnceHit = false;
		StaticMesh->SetNotifyRigidBodyCollision(false);
		
		UGameplayStatics::PlaySoundAtLocation(GetWorld(), HitSound, StaticMesh->GetComponentLocation());
	}
}
