
#include "Actors/Pickup.h"

// Sets default values
APickup::APickup()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
}

// Called when the game starts or when spawned
void APickup::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
// void APickup::Tick(float DeltaTime)
// {
// 	Super::Tick(DeltaTime);
// }

// Interfaces
APickup* APickup::GetPickupReference_Implementation()
{
	return this;
}

void APickup::SetPickupStatus_Implementation(EPickupState PickupState)
{
}
