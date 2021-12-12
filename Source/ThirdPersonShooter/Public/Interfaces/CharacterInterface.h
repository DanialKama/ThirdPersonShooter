// All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Enums/CharacterEnums.h"
#include "UObject/Interface.h"
#include "Enums/PickupEnums.h"
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
	// UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "CharacterInterface", meta = (ToolTip = "Get character base reference"))
	// BaseCharacter* GetCharacterReference(); TODO complete it after base character class implemented

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "CharacterInterface")
	bool IsPlayer();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "CharacterInterface")
	FGameplayTag GetTeamTag();
	
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "CharacterInterface")
	EWeaponToDo CanPickupAmmo(UPARAM(meta = (Bitmask, BitmaskEnum = EAmmoType)) int32 AmmoType);
	
	// Without Output
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "CharacterInterface")
	void SetMovementState(EMovementState CurrentMovementState, bool bRelatedToCrouch, bool bRelatedToProne);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "CharacterInterface")
	void SetHealthLevel(float Health);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "CharacterInterface", meta = (ToolTip = "Stamina should be between 0 and 1"))
	void SetStaminaLevel(float Stamina, bool bIsFull);
	
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "CharacterInterface")
	void SetPickup(EItemType NewPickupType, APickup* NewPickup);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "CharacterInterface")
	void AddRecoil(FRotator RotationIntensity, float ControlTime, float CrosshairRecoil, float ControllerPitch);
};