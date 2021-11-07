
#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "Enums/WeaponEnums.h"
#include "CommonInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UCommonInterface : public UInterface
{
	GENERATED_BODY()
};

class THIRDPERSONSHOOTER_API ICommonInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	// Functions
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "WithOutput", meta = (ToolTip = "Health component call it when owner health is zero"))
	void DeathTrigger();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "WithOutput")
	void SetCanFire(const bool bInCanFire);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "WithoutOutput")
	void SetWeaponState(EWeaponState WeaponState);
};
