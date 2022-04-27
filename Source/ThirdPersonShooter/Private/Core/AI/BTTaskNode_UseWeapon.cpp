// Copyright 2022 Danial Kamali. All Rights Reserved.

#include "Core/AI/BTTaskNode_UseWeapon.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"

EBTNodeResult::Type UBTTaskNode_UseWeapon::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	// If currently not using the weapon
	if (OwnerComp.GetBlackboardComponent()->GetValueAsBool(FName("UsingWeapon")) == false)
	{
		const AAIController* OwnerController = Cast<AAIController>(OwnerComp.GetOwner());
		APawn* ControlledPawn = OwnerController->GetPawn();
		if (ControlledPawn)
		{
			// TODO
		}
	}
	return EBTNodeResult::Succeeded;
}
