// All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Enums/CharacterEnums.h"
#include "UObject/Interface.h"
#include "CharacterAnimationInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UCharacterAnimationInterface : public UInterface
{
	GENERATED_BODY()
};

class THIRDPERSONSHOOTER_API ICharacterAnimationInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "CharacterAnimationInterface")
	void SetMovementState(EMovementState CurrentMovementState);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "CharacterAnimationInterface")
	void SetArmedState(bool bIsArmed, APickupWeapon* Weapon);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "CharacterAnimationInterface")
	void SetAimedState(bool bIsAimed, APickupWeapon* Weapon);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "CharacterAnimationInterface", meta = (ToolTip = "True when character is dead and mesh is not moving anymore"))
	void SetCompletelyStopMoving(bool bCompletelyStop);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "CharacterAnimationInterface")
	void AddRecoil(FRotator RotationIntensity, float ControlTime);
};