// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "PickupWeaponInterface.generated.h"

class APickupWeapon;

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UPickupWeaponInterface : public UInterface
{
	GENERATED_BODY()
};

class THIRDPERSONSHOOTER_API IPickupWeaponInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	// With Output
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "WeaponInterface")
	APickupWeapon* GetWeaponReference();
};
