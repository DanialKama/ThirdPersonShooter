// Copyright 2022-2023 Danial Kamali. All Rights Reserved.

#include "BTTask_DecrementPathIndex.h"

#include "BehaviorTree/BlackboardComponent.h"

UBTTask_DecrementPathIndex::UBTTask_DecrementPathIndex(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	NodeName = "Decrement Path Index";
}

EBTNodeResult::Type UBTTask_DecrementPathIndex::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	// True means moving to the next point, False means moving to the previous point
	// If currently moving to the next path point do not decrement path index
	if (OwnerComp.GetBlackboardComponent()->GetValueAsBool(FName("Direction")))
	{
		return EBTNodeResult::Failed;
	}
	
	const int32 PathIndex = OwnerComp.GetBlackboardComponent()->GetValueAsInt(FName("PathIndex"));
	
	if (PathIndex - 1 >= 0)
	{
		OwnerComp.GetBlackboardComponent()->SetValueAsBool(FName("Direction"), false);
		OwnerComp.GetBlackboardComponent()->SetValueAsInt(FName("PathIndex"), PathIndex - 1);
	}
	else
	{
		OwnerComp.GetBlackboardComponent()->SetValueAsBool(FName("Direction"), true);
	}
	
	return EBTNodeResult::Succeeded;
}
