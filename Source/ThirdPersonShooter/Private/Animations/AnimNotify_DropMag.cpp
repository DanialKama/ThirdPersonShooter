// Copyright 2022 Danial Kamali. All Rights Reserved.

#include "Animations/AnimNotify_DropMag.h"
#include "Characters/BaseCharacter.h"

void UAnimNotify_DropMag::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	if (MeshComp && MeshComp->GetOwner())
	{
		ABaseCharacter* Character = Cast<ABaseCharacter>(MeshComp->GetOwner());
		if (Character)
		{
			Character->SetReloadNotify(EReloadState::DropMag);
		}
	}
}