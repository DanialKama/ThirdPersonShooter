// All Rights Reserved.

#include "Core/ShooterPlayerController.h"
#include "Characters/PlayerCharacter.h"

AShooterPlayerController* AShooterPlayerController::GetPlayerControllerReference_Implementation()
{
	return this;
}

void AShooterPlayerController::RespawnPlayer()
{
	const FVector Location = PlayerTransform.GetLocation();
	const FRotator Rotation(PlayerTransform.GetRotation());
	FActorSpawnParameters ActorSpawnParameters;
	ActorSpawnParameters.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;
	APlayerCharacter* NewPlayer = GetWorld()->SpawnActor<APlayerCharacter>(PlayerToSpawn, Location, Rotation, ActorSpawnParameters);
	Possess(NewPlayer);
}

void AShooterPlayerController::SetWeaponState_Implementation(FAmmoComponentInfo AmmoComponentInfo, EWeaponState WeaponState)
{
}

void AShooterPlayerController::PlayCameraShake_Implementation(TSubclassOf<UCameraShakeBase> CameraShake)
{
	ClientStartCameraShake(CameraShake);
}