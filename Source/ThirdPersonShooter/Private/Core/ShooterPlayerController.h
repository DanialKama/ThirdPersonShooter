// Copyright 2022-2023 Danial Kamali. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "Interfaces/PlayerControllerInterface.h"
#include "ShooterPlayerController.generated.h"

UCLASS()
class AShooterPlayerController : public APlayerController, public IPlayerControllerInterface
{
	GENERATED_BODY()

// Functions
public:
	virtual void SetWeaponState_Implementation(FAmmoComponentInfo AmmoComponentInfo, EWeaponState WeaponState) override;
	virtual void PlayCameraShake_Implementation(TSubclassOf<UCameraShakeBase> CameraShake) override;

protected:
	virtual void BeginPlay() override;

// Variables
private:
	UPROPERTY()
	class AShooterHUD* ShooterHUD;
};