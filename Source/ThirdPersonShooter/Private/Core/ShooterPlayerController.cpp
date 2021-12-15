// All Rights Reserved.

#include "Core/ShooterPlayerController.h"
#include "Characters/PlayerCharacter.h"

AShooterPlayerController* AShooterPlayerController::GetPlayerControllerReference_Implementation()
{
	return this;
}

void AShooterPlayerController::RespawnPlayer_Implementation()
{
	const FVector Location = PlayerTransform.GetLocation();
	const FRotator Rotation(PlayerTransform.GetRotation());
	FActorSpawnParameters ActorSpawnParameters;
	ActorSpawnParameters.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;
	APlayerCharacter* NewPlayer = GetWorld()->SpawnActor<APlayerCharacter>(APlayerCharacter::StaticClass(), Location, Rotation, ActorSpawnParameters);
	NewPlayer->PossessedBy(this);
}

void AShooterPlayerController::SetWeaponState_Implementation(FAmmoComponentInfo AmmoComponentInfo, EWeaponState WeaponState)
{
}

void AShooterPlayerController::PlayCameraShake_Implementation(TSubclassOf<UCameraShakeBase> CameraShake)
{
	ClientStartCameraShake(CameraShake);
}