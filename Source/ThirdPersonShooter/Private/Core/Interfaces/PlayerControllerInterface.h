// Copyright 2022-2023 Danial Kamali. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "Core/Structures/AmmoComponentInfo.h"
#include "Core/Enumerations/PickupEnums.h"
#include "PlayerControllerInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI, Blueprintable)
class UPlayerControllerInterface : public UInterface
{
	GENERATED_BODY()
};

class IPlayerControllerInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "PlayerControllerInterface")
	void SetWeaponState(FAmmoComponentInfo AmmoComponentInfo, EWeaponState WeaponState);
	
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "PlayerControllerInterface")
	void PlayCameraShake(TSubclassOf<UCameraShakeBase> CameraShake);
};