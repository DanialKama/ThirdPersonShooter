// All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "AICharacterInterface.generated.h"

class AAICharacter;
class APatrolPathActor;

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UAICharacterInterface : public UInterface
{
	GENERATED_BODY()
};

class THIRDPERSONSHOOTER_API IAICharacterInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	// With Output
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "AICharacterInterface")
	APatrolPathActor* GetPatrolPath();
};