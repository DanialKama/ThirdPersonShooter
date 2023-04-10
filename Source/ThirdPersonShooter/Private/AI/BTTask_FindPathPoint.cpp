// Copyright 2022-2023 Danial Kamali. All Rights Reserved.

#include "BTTask_FindPathPoint.h"

#include "Actors/NonInteractive/PatrolPathActor.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Core/Interfaces/AICharacterInterface.h"
#include "Kismet/KismetMathLibrary.h"

UBTTask_FindPathPoint::UBTTask_FindPathPoint(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	NodeName = "Find Path Point";
}

EBTNodeResult::Type UBTTask_FindPathPoint::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
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
