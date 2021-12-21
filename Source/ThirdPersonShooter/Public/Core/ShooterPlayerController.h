// All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Characters/PlayerCharacter.h"
#include "GameFramework/PlayerController.h"
#include "Interfaces/PlayerControllerInterface.h"
#include "ShooterPlayerController.generated.h"

class APlayerCharacter;

UCLASS()
class THIRDPERSONSHOOTER_API AShooterPlayerController : public APlayerController, public IPlayerControllerInterface
{
	GENERATED_BODY()

public:
	// Interfaces
	virtual AShooterPlayerController* GetPlayerControllerReference_Implementation() override;
	virtual void SetWeaponState_Implementation(FAmmoComponentInfo AmmoComponentInfo, EWeaponState WeaponState) override;
	virtual void PlayCameraShake_Implementation(TSubclassOf<UCameraShakeBase> CameraShake) override;

	// Variables
	UPROPERTY(BlueprintReadWrite, Category = "Variables")
	FTransform PlayerTransform;

private:
	// Variables
	UPROPERTY(EditDefaultsOnly, Category = "Defaults", meta = (AllowPrivateAccess = true))
	TSubclassOf<APlayerCharacter> PlayerToSpawn;
};