// All Rights Reserved.

#include "Core/ShooterGameModeBase.h"

void AShooterGameModeBase::BeginPlay()
{
	Super::BeginPlay();

	// Bind our Player died delegate to the Game Mode's PlayerDied function.
	if (!OnPlayerDied.IsBound())
	{
		OnPlayerDied.AddDynamic(this, &AShooterGameModeBase::PlayerDied);
	}
}

void AShooterGameModeBase::PlayerDied(AController* PlayerController)
{
	RestartPlayer(PlayerController);
}