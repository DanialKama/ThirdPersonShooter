// All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "Interfaces/PlayerControllerInterface.h"
#include "ShooterPlayerController.generated.h"

UCLASS()
class THIRDPERSONSHOOTER_API AShooterPlayerController : public APlayerController, public IPlayerControllerInterface
{
	GENERATED_BODY()

public:
	// Interfaces
	// virtual AShooterPlayerController* GetPlayerControllerReference_Implementation() override;								// Player Controller Interface
	virtual void SetWeaponState_Implementation(FAmmoComponentInfo AmmoComponentInfo, EWeaponState WeaponState) override;	// Player Controller Interface, Call from pickup weapon
	virtual void PlayCameraShake_Implementation(TSubclassOf<UCameraShakeBase> CameraShake) override;									// Player Controller Interface, Call from pickup weapon
	// virtual void RespawnPlayer_Implementation() override;																	// Player Controller Interface, Call from pickup weapon

	// Variables
	UPROPERTY(BlueprintReadWrite, Category = "Variables")
	FTransform PlayerTransform;
};