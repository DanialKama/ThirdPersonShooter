
#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "Enums/PickupEnums.h"
#include "PickupInterface.generated.h"

class APickup;

// This class does not need to be modified.
UINTERFACE(MinimalAPI, Blueprintable)
class UPickupInterface : public UInterface
{
	GENERATED_BODY()
};

class THIRDPERSONSHOOTER_API IPickupInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	// Functions
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "WithOutput")
	void GetPickupReference(APickup* &Reference);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "WithoutOutput")
	void SetPickupStatus(EPickupState PickupState);
};
