// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "BaseComponent.generated.h"

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class THIRDPERSONSHOOTER_API UBaseComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UBaseComponent();
	
	// Functions
	UFUNCTION(BlueprintCallable, Category = "Components")
	void Initialize();

protected:
	// Functions
	UFUNCTION(BlueprintCallable, Category = "Components")
	virtual void SetupComponent();
	
	// Variables
	UPROPERTY(BlueprintReadOnly, Category = "Defaults")
	AActor* Owner;
};