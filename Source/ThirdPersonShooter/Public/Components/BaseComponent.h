// All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "BaseComponent.generated.h"

UCLASS()
class THIRDPERSONSHOOTER_API UBaseComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UBaseComponent();
	
	void Initialize();

protected:
	virtual void SetupComponent();

	UPROPERTY()
	AActor* Owner;
};