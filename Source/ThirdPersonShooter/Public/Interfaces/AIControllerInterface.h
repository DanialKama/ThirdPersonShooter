// All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "Structs/AmmoComponentInfoStruct.h"
#include "AIControllerInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI, Blueprintable)
class UAIControllerInterface : public UInterface
{
	GENERATED_BODY()
};

class THIRDPERSONSHOOTER_API IAIControllerInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	// Without Output
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "AIControllerInterface")
	void SetWeaponState(FAmmoComponentInfo AmmoComponentInfo, EWeaponState WeaponState);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "AIControllerInterface")
	void StartPatrol();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "AIControllerInterface")
	void SetAIState(EAIState AIState);
};