// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "PlayerCharacterInterface.generated.h"

class APickup;

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UPlayerCharacterInterface : public UInterface
{
	GENERATED_BODY()
};

class THIRDPERSONSHOOTER_API IPlayerCharacterInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	// With Output
	// UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "CharacterInterface", meta = (ToolTip = "Get parent character reference"))
	// PlayerCharacter* GetPlayerCharacterReference(); TODO complete it after player character class implemented
	
	// Without Output
};