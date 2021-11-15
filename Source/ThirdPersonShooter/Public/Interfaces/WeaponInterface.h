

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "Enums/WeaponEnums.h"
#include "WeaponInterface.generated.h"

class APickup_Weapon;

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UWeaponInterface : public UInterface
{
	GENERATED_BODY()
};

class THIRDPERSONSHOOTER_API IWeaponInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	// With Output
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "WeaponInterface", meta = (ToolTip = "Get hand location based on socket location"))
	FVector GetLeftHandLocation();
	
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "WeaponInterface", meta = (ToolTip = "Get hand location based on socket location"))
	FVector GetLeftHandAimLocation();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "WeaponInterface")
	TArray<EAmmoType> GetSupportedAmmo();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "WeaponInterface")
	APickup_Weapon* GetWeaponReference();
	
	// Without Output
	// UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "WeaponInterface")
};
