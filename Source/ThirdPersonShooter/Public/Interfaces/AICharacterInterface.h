// All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "AICharacterInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UAICharacterInterface : public UInterface
{
	GENERATED_BODY()
};

class THIRDPERSONSHOOTER_API IAICharacterInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	// Without Output
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "AICharacterInterface")
	void UseWeapon(bool bAim, bool bFire);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "AICharacterInterface")
	bool SwitchToWeapon(bool SwitchToAvailable, EWeaponToDo WeaponToSwitch);
};