// Danial Kama

#include "Pickups/Pickup.h"

// Sets default values
APickup::APickup()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	Scene = CreateDefaultSubobject<USceneComponent>(TEXT("Scene"));

	Scene->SetupAttachment(GetRootComponent());
}

// Called when the game starts or when spawned
void APickup::BeginPlay()
{
	Super::BeginPlay();

	PickupOwner = Cast<APawn>(GetOwner());
}

// Called every frame
void APickup::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

// Interfaces
void APickup::GetPickupReference_Implementation(APickup* &Reference)
{
	Reference = this;
}

void APickup::SetPickupStatus_Implementation(EPickupState PickupState)
{
	
}
