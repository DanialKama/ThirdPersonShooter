// Copyright 2022-2023 Danial Kamali. All Rights Reserved.

#include "BTTaskNode_FindPathPoint.h"
#include "AIController.h"
#include "Actors/PatrolPathActor.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Interfaces/AICharacterInterface.h"
#include "Kismet/KismetMathLibrary.h"

UBTTaskNode_FindPathPoint::UBTTaskNode_FindPathPoint(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	NodeName = "Find Path Point";
}

EBTNodeResult::Type UBTTaskNode_FindPathPoint::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	const AAIController* OwnerController = Cast<AAIController>(OwnerComp.GetOwner());
	APawn* ControlledPawn = OwnerController->GetPawn();
	if (ControlledPawn && ControlledPawn->GetClass()->ImplementsInterface(UAICharacterInterface::StaticClass()))
	{
		if (APatrolPathActor* PatrolPath = IAICharacterInterface::Execute_GetPatrolPath(ControlledPawn))
		{
			const int32 PathIndex = OwnerComp.GetBlackboardComponent()->GetValueAsInt(FName("PathIndex"));
			const FVector PathLocation = UKismetMathLibrary::TransformLocation(PatrolPath->GetActorTransform(), PatrolPath->PathPoints[PathIndex]);
			OwnerComp.GetBlackboardComponent()->SetValueAsVector(FName("PathLocation"), PathLocation);
			return EBTNodeResult::Succeeded;
		}
	}
	
	return EBTNodeResult::Failed;
}