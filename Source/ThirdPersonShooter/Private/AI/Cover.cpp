// Copyright 2022-2023 Danial Kamali. All Rights Reserved.

#include "Cover.h"

ACover::ACover()
{
	PrimaryActorTick.bCanEverTick = false;
	PrimaryActorTick.bStartWithTickEnabled = false;
	SetHidden(true);
	SetCanBeDamaged(false);

	SceneComp = CreateDefaultSubobject<USceneComponent>(TEXT("Scene Component"));
	RootComponent = SceneComp;
	SceneComp->Mobility = EComponentMobility::Static;
}
