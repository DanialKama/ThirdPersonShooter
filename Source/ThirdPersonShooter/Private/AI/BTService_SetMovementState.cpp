// Copyright 2022-2023 Danial Kamali. All Rights Reserved.

#include "BTService_SetMovementState.h"
#include "Characters/AICharacter.h"
#include "ShooterAIController.h"

UBTService_SetMovementState::UBTService_SetMovementState(const FObjectInitializer& ObjectInitializer) 
: Super(ObjectInitializer)
{
	NodeName = "Set Movement State";
	bNotifyTick = false;
	bTickIntervals = false;
	bNotifyBecomeRelevant = true;

	// Initialize variables
	MovementState = EMovementState::Walk;
	bRelatedToCrouch = false;
	bRelatedToProne = false;
}

void UBTService_SetMovementState::OnBecomeRelevant(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	if (const AShooterAIController* Owner = Cast<AShooterAIController>(OwnerComp.GetOwner()))
	{
		Owner->ControlledPawn->SetMovementState_Implementation(MovementState, bRelatedToCrouch, bRelatedToProne);
	}
}