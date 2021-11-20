// Copyright Epic Games, Inc. All Rights Reserved.

#include "Actors/Magazine.h"

// Sets default values
AMagazine::AMagazine()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
}

// Called when the game starts or when spawned
void AMagazine::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void AMagazine::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

