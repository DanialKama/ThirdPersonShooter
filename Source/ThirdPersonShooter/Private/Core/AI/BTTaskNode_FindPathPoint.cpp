// Copyright 2022 Danial Kamali. All Rights Reserved.

#include "Core/AI/BTTaskNode_FindPathPoint.h"
#include "AIController.h"
#include "Actors/PatrolPathActor.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Interfaces/AICharacterInterface.h"
#include "Kismet/KismetMathLibrary.h"

EBTNodeResult::Type UBTTaskNode_FindPathPoint::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	const AAIController* OwnerController = Cast<AAIController>(OwnerComp.GetOwner());
	APawn* ControlledPawn = OwnerController->GetPawn();
	if (ControlledPawn && ControlledPawn->GetClass()->ImplementsInterface(UAICharacterInterface::StaticClass()))
	{
		APatrolPathActor* PatrolPath = IAICharacterInterface::Execute_GetPatrolPath(ControlledPawn);
		if (PatrolPath)
		{
			const int32 PathIndex = OwnerComp.GetBlackboardComponent()->GetValueAsInt(FName("PathIndex"));
			const FVector PathLocation = UKismetMathLibrary::TransformLocation(PatrolPath->GetActorTransform(), PatrolPath->PathPoints[PathIndex]);
			OwnerComp.GetBlackboardComponent()->SetValueAsVector(FName("PathLocation"), PathLocation);
			return EBTNodeResult::Succeeded;
		}
	}
	
	return EBTNodeResult::Failed;
}