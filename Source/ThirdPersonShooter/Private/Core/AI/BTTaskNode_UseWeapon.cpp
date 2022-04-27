// Copyright 2022 Danial Kamali. All Rights Reserved.

#include "Core/AI/BTTaskNode_UseWeapon.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Characters/AICharacter.h"
#include "Core/AI/ShooterAIController.h"

UBTTaskNode_UseWeapon::UBTTaskNode_UseWeapon(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	NodeName = "Use Weapon";

	// Initialize variables
	Aim = false;
	Fire = false;
}

EBTNodeResult::Type UBTTaskNode_UseWeapon::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	// If currently not using the weapon
	if (OwnerComp.GetBlackboardComponent()->GetValueAsBool(FName("UseWeapon")) == false)
	{
		if (const AShooterAIController* OwnerController = Cast<AShooterAIController>(OwnerComp.GetOwner()))
		{
			OwnerController->ControlledPawn->UseWeapon(true, true);
			// if (OwnerController->WeaponState != EWeaponState::Reloading)
			// {
			// 	ControlledPawn->UseWeapon(false, false);
			// }
		}
	}
	return EBTNodeResult::Succeeded;
}