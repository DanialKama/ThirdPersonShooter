// Copyright 2022-2023 Danial Kamali. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "Core/Structures/WeaponInfo.h"
#include "WidgetInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UWidgetInterface : public UInterface
{
	GENERATED_BODY()
};

class IWidgetInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "WidgetInterface")
	void SetWeaponInfo(FWeaponInfo WeaponInfo);

	/** Do not select multiple for Ammo Type */
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "WidgetInterface")
	void SetAmmoInfo(int32 AmmoType, int32 Amount);
	
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "WidgetInterface")
	void UpdateActorHealth(float HealthPercent);
};