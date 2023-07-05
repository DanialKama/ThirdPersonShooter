// Copyright 2022-2023 Danial Kamali. All Rights Reserved.

#include "AnimNotify_RestoreHealth.h"

#include "AIController.h"
#include "Characters/BaseCharacter.h"
#include "Components/HealthComponent.h"
#include "Core/Interfaces/AIControllerInterface.h"
#include "GameFramework/CharacterMovementComponent.h"

UAnimNotify_RestoreHealth::UAnimNotify_RestoreHealth(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
#if WITH_EDITORONLY_DATA
	bShouldFireInEditor = false;
	NotifyColor = FColor(177, 188, 188);
#endif
}

void UAnimNotify_RestoreHealth::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation, EventReference);
	
	if (MeshComp && MeshComp->GetOwner())
	{
		if (const ABaseCharacter* Character = Cast<ABaseCharacter>(MeshComp->GetOwner()))
		{
			// Set movement mode to walking if it was set to null
			// During AI healing movement set to null to stop AI from moving and interrupt montage
			if (Character->GetCharacterMovement()->MovementMode == MOVE_None)
			{
				Character->GetCharacterMovement()->SetMovementMode(MOVE_Walking);
			}
			
			// Full health recovery
			Character->HealthComponent->Healing(Character->HealthComponent->MaxHealth);
			
			// Set AI state if the owner is AI
			AAIController* AIController = Cast<AAIController>(Character->GetController());
			
			if (AIController && AIController->GetClass()->ImplementsInterface(UAIControllerInterface::StaticClass()))
			{
				IAIControllerInterface::Execute_SetAIState(AIController, EAIState::Idle);
			}
		}
	}
}
