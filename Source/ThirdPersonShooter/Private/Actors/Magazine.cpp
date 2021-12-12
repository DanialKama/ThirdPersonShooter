// All Rights Reserved.

#include "Actors/Magazine.h"
#include "Kismet/GameplayStatics.h"
#include "Sound/SoundCue.h"

// Sets default values
AMagazine::AMagazine()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	
	// Create components
	StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Magazine"));

	// Setup components attachment
	SetRootComponent(StaticMesh);

	// Set component defaults
	StaticMesh->bApplyImpulseOnDamage = false;
	StaticMesh->SetNotifyRigidBodyCollision(true);
	StaticMesh->SetGenerateOverlapEvents(false);
	StaticMesh->SetCollisionObjectType(ECC_WorldStatic);
	StaticMesh->OnComponentHit.AddDynamic(this, &AMagazine::OnHit);

	// Initialize variables
	bDoOnce = true;
}

// Called when the game starts or when spawned
void AMagazine::BeginPlay()
{
	Super::BeginPlay();

	if(bMagazineIsEmpty)
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
	if(bDoOnce)
	{
		const FCollisionResponseContainer CollisionResponse = OtherComp->GetCollisionResponseToChannels();
		if(CollisionResponse.WorldStatic == ECR_Block)
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