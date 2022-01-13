// All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "BaseComponent.generated.h"

UCLASS()
class THIRDPERSONSHOOTER_API UBaseComponent : public UActorComponent
{
	GENERATED_BODY()

// Functions
public:
	UBaseComponent();

	/** Setup component */
	virtual void Initialize();

// Variables
protected:
	UPROPERTY()
	AActor* Owner;
};