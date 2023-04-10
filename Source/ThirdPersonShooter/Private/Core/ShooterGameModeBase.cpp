// Copyright 2022-2023 Danial Kamali. All Rights Reserved.

#include "ShooterGameModeBase.h"

void AShooterGameModeBase::BeginPlay()
{
	Super::BeginPlay();

	OnPlayerDied.AddDynamic(this, &AShooterGameModeBase::PlayerDied);
}

// TODO: Player controller should call this
void AShooterGameModeBase::PlayerDied(AController* PlayerController)
{
	RestartPlayer(PlayerController);
}
