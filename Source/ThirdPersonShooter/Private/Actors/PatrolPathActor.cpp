// Copyright 2022-2023 Danial Kamali. All Rights Reserved.

#include "PatrolPathActor.h"

APatrolPathActor::APatrolPathActor()
{
	PrimaryActorTick.bCanEverTick = false;
	PrimaryActorTick.bStartWithTickEnabled = false;
	SetCanBeDamaged(false);
	SetHidden(true);
	
	SceneComp = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	RootComponent = SceneComp;
	SceneComp->Mobility = EComponentMobility::Static;

	// Initialize variables
	bIsLoop = true;
}
