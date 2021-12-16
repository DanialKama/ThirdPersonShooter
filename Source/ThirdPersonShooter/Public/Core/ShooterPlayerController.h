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
	// Functions
	UFUNCTION(BlueprintCallable, Category = "ShooterPlayerController")
	void RespawnPlayer() ;
	// Interfaces
	virtual AShooterPlayerController* GetPlayerControllerReference_Implementation() override;
	virtual void SetWeaponState_Implementation(FAmmoComponentInfo AmmoComponentInfo, EWeaponState WeaponState) override;
	virtual void PlayCameraShake_Implementation(TSubclassOf<UCameraShakeBase> CameraShake) override;

	// Variables
	UPROPERTY(BlueprintReadWrite, Category = "Variables")
	FTransform PlayerTransform;
};