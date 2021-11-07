﻿
#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "Enums/WeaponEnums.h"
#include "PlayerControllerInterface.generated.h"

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
	// Functions
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "WithOutput")
	void SetWeaponState(int32 AmmoInfo, EWeaponState WeaponState); // TODO Replace AmmoInfo with structure
};
