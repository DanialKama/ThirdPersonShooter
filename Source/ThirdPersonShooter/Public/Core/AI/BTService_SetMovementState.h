// Copyright 2022 Danial Kamali. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Services/BTService_BlackboardBase.h"
#include "Enums/CharacterEnums.h"
#include "BTService_SetMovementState.generated.h"

UCLASS()
class THIRDPERSONSHOOTER_API UBTService_SetMovementState : public UBTService_BlackboardBase
{
	GENERATED_BODY()

// Functions
public:
	UBTService_SetMovementState(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

private:
	virtual void OnBecomeRelevant(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

// Variables
public:
	UPROPERTY(EditAnywhere, Category = "Node")
	EMovementState MovementState;
	
	UPROPERTY(EditAnywhere, Category = "Node")
	uint8 bRelatedToCrouch : 1;

	UPROPERTY(EditAnywhere, Category = "Node")
	uint8 bRelatedToProne : 1;
};