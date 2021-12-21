// All Rights Reserved.

#include "Core/ShooterPlayerController.h"
#include "Characters/PlayerCharacter.h"

AShooterPlayerController* AShooterPlayerController::GetPlayerControllerReference_Implementation()
{
	return this;
}

void AShooterPlayerController::SetWeaponState_Implementation(FAmmoComponentInfo AmmoComponentInfo, EWeaponState WeaponState)
{
	// TODO
}

void AShooterPlayerController::PlayCameraShake_Implementation(TSubclassOf<UCameraShakeBase> CameraShake)
{
	ClientStartCameraShake(CameraShake);
}