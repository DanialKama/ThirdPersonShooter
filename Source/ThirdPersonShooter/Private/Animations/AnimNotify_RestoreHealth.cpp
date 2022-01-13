// All Rights Reserved.

#include "Animations/AnimNotify_RestoreHealth.h"
#include "AIController.h"
#include "Characters/BaseCharacter.h"
#include "Components/HealthComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Interfaces/AIControllerInterface.h"

void UAnimNotify_RestoreHealth::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	if (MeshComp && MeshComp->GetOwner())
	{
		ABaseCharacter* Character = Cast<ABaseCharacter>(MeshComp->GetOwner());
		if (Character)
		{
			// Set movement mode to walking if it was set to null
			// During AI healing movement set to null to stop AI from moving and interrupt montage
			if (Character->GetCharacterMovement()->GetMovementName() == "NULL")
			{
				Character->GetCharacterMovement()->SetMovementMode(MOVE_Walking);
			}
			// Full health recovery
			Character->GetHealthComponent()->Healing(Character->GetHealthComponent()->MaxHealth);
			// Set AI state if the owner is AI
			AAIController* AIController = Cast<AAIController>(Character->GetController());
			if (AIController && AIController->GetClass()->ImplementsInterface(UAIControllerInterface::StaticClass()))
			{
				IAIControllerInterface::Execute_SetAIState(AIController, EAIState::Idle);
			}
		}
	}
}