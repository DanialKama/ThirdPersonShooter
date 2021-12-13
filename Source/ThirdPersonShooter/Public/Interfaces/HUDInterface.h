// All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "HUDInterface.generated.h"

class AShooterHUD;

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UHUDInterface : public UInterface
{
	GENERATED_BODY()
};
class THIRDPERSONSHOOTER_API IHUDInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	// With Output
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "HUDInterface")
	AShooterHUD* GetHUDReference();
};