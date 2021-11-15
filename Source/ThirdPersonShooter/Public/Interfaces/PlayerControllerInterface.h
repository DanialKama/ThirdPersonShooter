
#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "Enums/WeaponEnums.h"
#include "Structs/AmmoComponentInfoStruct.h"
#include "PlayerControllerInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI, Blueprintable)
class UPlayerControllerInterface : public UInterface
{
	GENERATED_BODY()
};

class THIRDPERSONSHOOTER_API IPlayerControllerInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	// Without Output
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "PlayerControllerInterface")
	void SetWeaponState(FAmmoComponentInfo AmmoComponentInfo, EWeaponState WeaponState);
	
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "PlayerControllerInterface")
	void PlayCameraShake(UCameraShakeBase* CameraShake);
};
