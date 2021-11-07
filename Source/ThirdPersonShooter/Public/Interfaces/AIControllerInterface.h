
#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "Enums/WeaponEnums.h"
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
	// Functions
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "WithOutput")
	void SetWeaponState(int32 AmmoInfo, EWeaponState WeaponState); // TODO Replace AmmoInfo with structure
};
