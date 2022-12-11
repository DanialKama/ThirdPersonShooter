// Copyright 2022 Danial Kamali. All Rights Reserved.

#include "AnimNotify_Ragdoll.h"
#include "Characters/BaseCharacter.h"

void UAnimNotify_Ragdoll::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	if (MeshComp && MeshComp->GetOwner())
	{
		if (ABaseCharacter* Character = Cast<ABaseCharacter>(MeshComp->GetOwner()))
		{
			Character->DeathMontageHandler(nullptr, false);
		}
	}
}