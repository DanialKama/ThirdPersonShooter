// Copyright 2022-2023 Danial Kamali. All Rights Reserved.

#include "BTTaskNode_ToggleBoolState.h"
#include "BehaviorTree/BlackboardComponent.h"

UBTTaskNode_ToggleBoolState::UBTTaskNode_ToggleBoolState(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	NodeName = "Toggle Bool State (Deprecated)";
}

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