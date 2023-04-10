// Copyright 2022-2023 Danial Kamali. All Rights Reserved.

#include "BTTask_LoopPath.h"

#include "BehaviorTree/BlackboardComponent.h"

UBTTask_LoopPath::UBTTask_LoopPath(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	NodeName = "Loop Path";
}

EBTNodeResult::Type UBTTask_LoopPath::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	if (OwnerComp.GetBlackboardComponent()->GetValueAsBool(FName("LoopPath")))
	{
		OwnerComp.GetBlackboardComponent()->SetValueAsInt(FName("PathIndex"), 0);
		OwnerComp.GetBlackboardComponent()->SetValueAsBool(FName("Direction"), true);
		
		return EBTNodeResult::Succeeded;
	}
	
	return EBTNodeResult::Failed;
}
