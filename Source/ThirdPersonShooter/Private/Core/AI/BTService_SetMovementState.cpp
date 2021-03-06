// Copyright 2022 Danial Kamali. All Rights Reserved.

#include "Core/AI/BTService_SetMovementState.h"
#include "Characters/AICharacter.h"
#include "Core/AI/ShooterAIController.h"

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