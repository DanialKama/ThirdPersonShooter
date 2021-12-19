// All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PatrolPathActor.generated.h"

UCLASS()
class THIRDPERSONSHOOTER_API APatrolPathActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	APatrolPathActor();

	// Components
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UBillboardComponent* Billboard;

	// Variables
	UPROPERTY(BlueprintReadOnly, EditInstanceOnly, Category = "Defaults", meta = (MakeEditWidget = true))
	TArray<FVector> PathPoints;
	
	UPROPERTY(BlueprintReadOnly, EditInstanceOnly, Category = "Defaults")
	uint8 bIsLooping : 1;

	UPROPERTY(BlueprintReadOnly, EditInstanceOnly, Category = "Defaults", meta = (ToolTip = "Wait time on each patrol point", ClampMin = "0.0", UIMin = "0.0"))
	float WaitTime;
};