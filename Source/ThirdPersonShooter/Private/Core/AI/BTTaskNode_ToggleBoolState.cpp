// All Rights Reserved.

#include "Core/AI/BTTaskNode_ToggleBoolState.h"
#include "BehaviorTree/BlackboardComponent.h"

EBTNodeResult::Type UBTTaskNode_ToggleBoolState::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	const uint8 KeyID = OwnerComp.GetBlackboardComponent()->GetKeyID(KeyName);
	if (OwnerComp.GetBlackboardComponent()->IsValidKey(KeyID))
	{
		// If boolean is true set it to false and if it is false set it to true
		if (OwnerComp.GetBlackboardComponent()->GetValueAsBool(KeyName))
		{
			OwnerComp.GetBlackboardComponent()->SetValueAsBool(KeyName, false);
		}
		else
		{
			OwnerComp.GetBlackboardComponent()->SetValueAsBool(KeyName, true);
		}
		return EBTNodeResult::Succeeded;
	}
	return EBTNodeResult::Failed;
}