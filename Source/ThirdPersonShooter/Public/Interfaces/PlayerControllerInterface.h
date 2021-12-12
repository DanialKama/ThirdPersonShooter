// All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "Structs/AmmoComponentInfoStruct.h"
#include "Enums/PickupEnums.h"
#include "PlayerControllerInterface.generated.h"

class AShooterPlayerController;

// This class does not need to be modified.
UINTERFACE(MinimalAPI, Blueprintable)
class UPlayerControllerInterface : public UInterface
{
	GENERATED_BODY()
};

class THIRDPERSONSHOOTER_API IPlayerControllerInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	// With Output
	// UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "PlayerControllerInterface")
	// AShooterPlayerController* GetPlayerControllerReference();
	
	// Without Output
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "PlayerControllerInterface")
	void SetWeaponState(FAmmoComponentInfo AmmoComponentInfo, EWeaponState WeaponState);
	
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "PlayerControllerInterface")
	void PlayCameraShake(TSubclassOf<UCameraShakeBase> CameraShake);
	
	// UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "PlayerControllerInterface")
	// void RespawnPlayer();
};