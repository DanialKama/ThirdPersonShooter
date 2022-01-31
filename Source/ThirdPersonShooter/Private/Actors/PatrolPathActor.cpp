// Copyright 2022 Danial Kamali. All Rights Reserved.

#include "Actors/PatrolPathActor.h"
#include "Components/BillboardComponent.h"

APatrolPathActor::APatrolPathActor()
{
	PrimaryActorTick.bCanEverTick = false;

	Billboard = CreateDefaultSubobject<UBillboardComponent>(TEXT("Billboard"));
	SetRootComponent(Billboard);
	Billboard->SetComponentTickEnabled(false);
	Billboard->bIsScreenSizeScaled = true;

	// Initialize variables
	bIsLooping = true;
	WaitTime = 2.0f;
}