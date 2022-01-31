// Copyright 2022 Danial Kamali. All Rights Reserved.

#include "Animations/AnimNotify_Ragdoll.h"
#include "Characters/BaseCharacter.h"

void UAnimNotify_Ragdoll::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	if (MeshComp && MeshComp->GetOwner())
	{
		ABaseCharacter* Character = Cast<ABaseCharacter>(MeshComp->GetOwner());
		if (Character)
		{
			Character->DeathMontageHandler(nullptr, false);
		}
	}
}