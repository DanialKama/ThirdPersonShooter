// All Rights Reserved.

#include "Actors/PatrolPathActor.h"
#include "Components/BillboardComponent.h"

APatrolPathActor::APatrolPathActor()
{
	PrimaryActorTick.bCanEverTick = false;

	// Create components
	Billboard = CreateDefaultSubobject<UBillboardComponent>(TEXT("Billboard"));

	// Attach components
	SetRootComponent(Billboard);

	// Initialize components
	Billboard->SetComponentTickEnabled(false);
	Billboard->bIsScreenSizeScaled = true;
}