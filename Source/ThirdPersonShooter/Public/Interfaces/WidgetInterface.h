// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "Structs/WeaponInfoStruct.h"
#include "WidgetInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UWidgetInterface : public UInterface
{
	GENERATED_BODY()
};

class THIRDPERSONSHOOTER_API IWidgetInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	// Without Output
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "WidgetInterface")
	void SetWeaponInfo(FWeaponInfo WeaponInfo);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "WidgetInterface", meta = (ToolTip = "Do not select multiple for Ammo Type"))
	void SetAmmoInfo(UPARAM(meta = (Bitmask, BitmaskEnum = EAmmoType)) int32 AmmoType, int32 Amount);
	
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "WidgetInterface")
	void UpdateActorHealth(float HealthPercent);
};