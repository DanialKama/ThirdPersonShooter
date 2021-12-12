// All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "PickupAmmoInterface.generated.h"

class APickupAmmo;

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UPickupAmmoInterface : public UInterface
{
	GENERATED_BODY()
};

class THIRDPERSONSHOOTER_API IPickupAmmoInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	// With Output
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "PickupAmmoInterface")
	APickupAmmo* GetPickupAmmoReference();
};