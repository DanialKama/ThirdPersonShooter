// Copyright 2022-2023 Danial Kamali. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "Core/Enumerations/PickupEnums.h"
#include "GameplayTagContainer.h"
#include "Core/Enumerations/CharacterEnums.h"
#include "CommonInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UCommonInterface : public UInterface
{
	GENERATED_BODY()
};

class ICommonInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "CommonInterface")
	FGameplayTag GetTeamTag();

	// TODO: Use pawn controller
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "CommonInterface")
	bool IsPlayer();
	
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "CommonInterface", meta = (ToolTip = "Call from player controller to handle interaction on current object"))
	void Interact();
	
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "CommonInterface", meta = (ToolTip = "Health component call it"))
	void SetHealthState(EHealthState HealthState);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "CommonInterface", meta = (ToolTip = "Ammo component call it"))
	void SetCanFire(bool bInCanFire);
	
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "CommonInterface", meta = (ToolTip = "Ammo component call it to update weapon state"))
	void SetWeaponState(EWeaponState WeaponState);
};