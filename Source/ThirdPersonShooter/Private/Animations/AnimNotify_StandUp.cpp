// Copyright 2022 Danial Kamali. All Rights Reserved.

#include "Animations/AnimNotify_StandUp.h"
#include "Characters/BaseCharacter.h"

void UAnimNotify_StandUp::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	if (MeshComp && MeshComp->GetOwner())
	{
		if (const ABaseCharacter* Character = Cast<ABaseCharacter>(MeshComp->GetOwner()))
		{
			// Set interrupt to true to character movement set to walk
			Character->StanUpMontageHandler(nullptr, true);
		}
	}
}