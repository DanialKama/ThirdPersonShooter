// Copyright 2022 Danial Kamali. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PatrolPathActor.generated.h"

UCLASS()
class THIRDPERSONSHOOTER_API APatrolPathActor : public AActor
{
	GENERATED_BODY()

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = true))
	class UBillboardComponent* Billboard;
	
// Functions
public:
	APatrolPathActor();

// Variables
public:
	UPROPERTY(EditInstanceOnly, Category = "Defaults", meta = (MakeEditWidget = true))
	TArray<FVector> PathPoints;
	
	UPROPERTY(EditInstanceOnly, Category = "Defaults", meta = (ToolTip = "If set to true AI directly move to the first point otherwise AI move back to previous points until reaching the first point"))
	uint8 bIsLooping : 1;
	
	UPROPERTY(EditInstanceOnly, Category = "Defaults", meta = (ToolTip = "Waiting time after reaching every patrol point", ClampMin = "0.0", UIMin = "0.0"))
	float WaitTime;
};