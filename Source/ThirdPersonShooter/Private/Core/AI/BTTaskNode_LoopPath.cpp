// All Rights Reserved.

#include "Core/AI/BTTaskNode_LoopPath.h"
#include "BehaviorTree/BlackboardComponent.h"

EBTNodeResult::Type UBTTaskNode_LoopPath::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	UE_LOG(LogTemp, Warning, TEXT("LOOP PATH"));
	if (OwnerComp.GetBlackboardComponent()->GetValueAsBool(FName("LoopPath")))
	{
		OwnerComp.GetBlackboardComponent()->SetValueAsInt(FName("PathIndex"), 0);
		OwnerComp.GetBlackboardComponent()->SetValueAsBool(FName("Direction"), true);
		return EBTNodeResult::Succeeded;
	}

	return EBTNodeResult::Failed;
}