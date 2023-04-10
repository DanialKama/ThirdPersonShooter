// Copyright 2022-2023 Danial Kamali. All Rights Reserved.

#include "BTTaskNode_IncrementPathIndex.h"

#include "AIController.h"
#include "Actors/NonInteractive/PatrolPathActor.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Core/Interfaces/AICharacterInterface.h"

UBTTaskNode_IncrementPathIndex::UBTTaskNode_IncrementPathIndex(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	NodeName = "Increment Path Index (Deprecated)";
}

EBTNodeResult::Type UBTTaskNode_IncrementPathIndex::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	// True means moving to the next point, False means moving to the previous point
	if (OwnerComp.GetBlackboardComponent()->GetValueAsBool(FName("Direction")))
	{
		const AAIController* OwnerController = Cast<AAIController>(OwnerComp.GetOwner());
		APawn* ControlledPawn = OwnerController->GetPawn();
		if (ControlledPawn && ControlledPawn->GetClass()->ImplementsInterface(UAICharacterInterface::StaticClass()))
		{
			if (const APatrolPathActor* PatrolPath = IAICharacterInterface::Execute_GetPatrolPath(ControlledPawn))
			{
				const int32 PathIndex = OwnerComp.GetBlackboardComponent()->GetValueAsInt(FName("PathIndex"));
				if (PathIndex + 1 <= PatrolPath->PathPoints.Num() - 1)
				{
					OwnerComp.GetBlackboardComponent()->SetValueAsBool(FName("Direction"), true);
					OwnerComp.GetBlackboardComponent()->SetValueAsInt(FName("PathIndex"), PathIndex + 1);
				}
				else
				{
					OwnerComp.GetBlackboardComponent()->SetValueAsBool(FName("Direction"), false);
				}
				return EBTNodeResult::Succeeded;
			}
		}
	}
	
	return EBTNodeResult::Failed;
}
