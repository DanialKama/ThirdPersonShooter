// Copyright 2022 Danial Kamali. All Rights Reserved.

#include "Core/AI/BTTaskNode_LoopPath.h"
#include "BehaviorTree/BlackboardComponent.h"

UBTTaskNode_LoopPath::UBTTaskNode_LoopPath(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	NodeName = "Loop Path";
}

EBTNodeResult::Type UBTTaskNode_LoopPath::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	if (OwnerComp.GetBlackboardComponent()->GetValueAsBool(FName("LoopPath")))
	{
		OwnerComp.GetBlackboardComponent()->SetValueAsInt(FName("PathIndex"), 0);
		OwnerComp.GetBlackboardComponent()->SetValueAsBool(FName("Direction"), true);
		return EBTNodeResult::Succeeded;
	}
	
	return EBTNodeResult::Failed;
}