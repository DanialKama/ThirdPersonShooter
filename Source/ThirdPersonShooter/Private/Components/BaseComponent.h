// Copyright 2022-2023 Danial Kamali. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "BaseComponent.generated.h"

UCLASS()
class UBaseComponent : public UActorComponent
{
	GENERATED_BODY()

// Functions
public:
	UBaseComponent();

	/** Setup component */
	virtual void Initialize() {}
};
