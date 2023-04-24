// Copyright 2022-2023 Danial Kamali. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PointOfInterest.generated.h"

/**
 * Used to specify the best points on the map that AI can move to
 */
UCLASS()
class APointOfInterest : public AActor
{
	GENERATED_BODY()

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = true))
	TObjectPtr<USceneComponent> SceneComp;

public:
	/** Sets default values for this actor's properties */
	APointOfInterest();
};
