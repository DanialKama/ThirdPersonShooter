// Copyright 2022-2023 Danial Kamali. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PatrolPathActor.generated.h"

// TODO: Rename to APatrolPath
UCLASS(meta = (DisplayName = "Patrol Path Actor"))
class APatrolPathActor : public AActor
{
	GENERATED_BODY()

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = true))
	TObjectPtr<USceneComponent> SceneComp;
	
// Functions
public:
	APatrolPathActor();

// Variables
public:
	UPROPERTY(EditInstanceOnly, Category = "Defaults", meta = (MakeEditWidget = true))
	TArray<FVector> PathPoints;

	// TODO: Test if and write a better comment
	/** If set to true AI directly move to the first point otherwise AI move back to previous points until reaching the first point */
	UPROPERTY(EditInstanceOnly, Category = "Defaults", meta = (MakeEditWidget = true))
	uint8 bIsLoop : 1;

	/** Waiting time after reaching every point */
	UPROPERTY(EditInstanceOnly, Category = "Defaults", meta = (ClampMin = "0.0", UIMin = "0.0"))
	float WaitTime = 2.0f;
};
