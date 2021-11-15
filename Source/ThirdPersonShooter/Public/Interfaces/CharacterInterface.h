

#pragma once

#include "CoreMinimal.h"
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
	// Without Output
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "CharacterInterface")
	void SetPickup(EItemType PickupType, APickup* PickupReference);	// TODO Rename after testing
	// virtual void SetPickupPure(EItemType PickupType, APickup* PickupReference) = 0;
};
