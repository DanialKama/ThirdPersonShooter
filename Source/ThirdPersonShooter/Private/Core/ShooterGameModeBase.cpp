// Copyright 2022 Danial Kamali. All Rights Reserved.

#include "Core/ShooterGameModeBase.h"

void AShooterGameModeBase::BeginPlay()
{
	Super::BeginPlay();

	// Bind player died delegate to the Game Mode's PlayerDied function.
	if (OnPlayerDied.IsBound() == false)
	{
		OnPlayerDied.AddDynamic(this, &AShooterGameModeBase::PlayerDied);
	}
}

void AShooterGameModeBase::PlayerDied(AController* PlayerController)
{
	RestartPlayer(PlayerController);
}