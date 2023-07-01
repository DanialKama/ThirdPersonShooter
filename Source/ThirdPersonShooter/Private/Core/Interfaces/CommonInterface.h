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

	/** Call from player controller to handle interaction on current object */
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "CommonInterface")
	void Interact();

	/** Health component call it */
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "CommonInterface")
	void SetHealthState(EHealthState HealthState);

	/** Ammo component call it */
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "CommonInterface")
	void SetCanFire(bool bInCanFire);

	// TODO: Delete
	/** Ammo component call it to update weapon state */
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "CommonInterface")
	void SetWeaponState(EWeaponState WeaponState);
};