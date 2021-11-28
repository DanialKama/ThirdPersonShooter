
#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "CharacterAnimationInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI, Blueprintable)
class UCharacterAnimationInterface : public UInterface
{
	GENERATED_BODY()
};

class THIRDPERSONSHOOTER_API ICharacterAnimationInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	// Without Output
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "AIControllerInterface")
	void AddRecoil(FRotator RotationIntensity);
};
