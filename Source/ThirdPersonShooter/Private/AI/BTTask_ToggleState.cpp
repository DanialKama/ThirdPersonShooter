// Copyright 2022-2023 Danial Kamali. All Rights Reserved.

#include "BTTask_ToggleState.h"

#include "BehaviorTree/Blackboard/BlackboardKeyType_Bool.h"
#include "BehaviorTree/BlackboardComponent.h"

UBTTask_ToggleState::UBTTask_ToggleState(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	NodeName = "Toggle State";

	BlackboardKey.AddBoolFilter(this, GET_MEMBER_NAME_CHECKED(UBTTask_ToggleState, BlackboardKey));
}

EBTNodeResult::Type UBTTask_ToggleState::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	const bool Value = OwnerComp.GetBlackboardComponent()->GetValue<UBlackboardKeyType_Bool>(BlackboardKey.GetSelectedKeyID());
	
	OwnerComp.GetBlackboardComponent()->SetValue<UBlackboardKeyType_Bool>(BlackboardKey.GetSelectedKeyID(), !Value);

	return EBTNodeResult::Succeeded;
}
