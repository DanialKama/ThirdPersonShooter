// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "Enums/PickupEnums.h"
#include "Enums/WeaponEnums.h"
#include "CharacterInterface.generated.h"

class APickup;

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UCharacterInterface : public UInterface
{
	GENERATED_BODY()
};

class THIRDPERSONSHOOTER_API ICharacterInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	// With Output
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "CharacterInterface")
	EWeaponToDo CanPickupAmmo(UPARAM(meta = (Bitmask, BitmaskEnum = EAmmoType)) int32 AmmoType);
	
	// Without Output
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "CharacterInterface")
	void SetPickup(EItemType PickupType, APickup* PickupReference);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "CharacterInterface")
	void AddRecoil(FRotator RotationIntensity, float ControlTime, float CrosshairRecoil);
};
