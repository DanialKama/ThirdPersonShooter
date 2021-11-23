// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
// #include "Enums/WeaponEnums.h"
#include "WeaponInterface.generated.h"

class APickup_Weapon;

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UWeaponInterface : public UInterface
{
	GENERATED_BODY()
};

class THIRDPERSONSHOOTER_API IWeaponInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	// With Output
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "WeaponInterface")
	APickup_Weapon* GetWeaponReference();
};
