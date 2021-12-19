// All Rights Reserved.

#include "Actors/PatrolPathActor.h"
#include "Components/BillboardComponent.h"

// Sets default values
APatrolPathActor::APatrolPathActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	// Create components
	Billboard = CreateDefaultSubobject<UBillboardComponent>(TEXT("Billboard"));

	// Setup components attachment
	SetRootComponent(Billboard);

	// Initialize components
	Billboard->bIsScreenSizeScaled = true;
}