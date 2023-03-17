// Copyright 2022-2023 Danial Kamali. All Rights Reserved.

#include "BTTaskNode_Healing.h"
#include "Characters/AICharacter.h"
#include "Characters/BaseCharacter.h"
#include "ShooterAIController.h"
#include "Enums/CharacterEnums.h"
#include "GameFramework/CharacterMovementComponent.h"

UBTTaskNode_Healing::UBTTaskNode_Healing(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	NodeName = "Healing";

	// Initialize variables
	HealingMontage = nullptr;
}

EBTNodeResult::Type UBTTaskNode_Healing::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	AController* OwnerController = Cast<AController>(OwnerComp.GetOwner());
	if (const ABaseCharacter* ControlledPawn = Cast<ABaseCharacter>(OwnerController->GetPawn()))
	{
		// If owner is AI
		if (AShooterAIController* ShooterAIController = Cast<AShooterAIController>(OwnerController))
		{
			ShooterAIController->SetAIState_Implementation(EAIState::UseMed);
			ShooterAIController->ControlledPawn->GetCharacterMovement()->DisableMovement();
			ShooterAIController->ControlledPawn->UseWeapon(false, false);
		}
		
		UAnimInstance* AnimInstance = ControlledPawn->GetMesh()->GetAnimInstance();
		const float MontageLength = AnimInstance->Montage_Play(HealingMontage);
		if (MontageLength > 0.0f)
		{
			AnimInstance->Montage_JumpToSection(FName("Start"));
			FOnMontageEnded EndedDelegate;
			EndedDelegate.BindUObject(ControlledPawn, &ABaseCharacter::HealingMontageHandler);
			AnimInstance->Montage_SetEndDelegate(EndedDelegate, HealingMontage);
			return EBTNodeResult::Succeeded;
		}
	}
	
	return EBTNodeResult::Failed;
}