// Copyright 2022-2023 Danial Kamali. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Services/BTService_BlackboardBase.h"
#include "Core/Enumerations/CharacterEnums.h"
#include "BTService_SetMovementState.generated.h"

UCLASS()
class UBTService_SetMovementState : public UBTService
{
	GENERATED_UCLASS_BODY()

// Functions
protected:
	virtual void OnBecomeRelevant(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

// Variables
public:
	UPROPERTY(EditAnywhere, Category = "Node")
	EMovementState MovementState = EMovementState::Walk;
};
