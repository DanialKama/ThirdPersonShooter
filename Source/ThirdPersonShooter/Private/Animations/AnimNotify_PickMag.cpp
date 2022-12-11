// Copyright 2022 Danial Kamali. All Rights Reserved.

#include "AnimNotify_PickMag.h"
#include "Characters/BaseCharacter.h"

void UAnimNotify_PickMag::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	if (MeshComp && MeshComp->GetOwner())
	{
		if (ABaseCharacter* Character = Cast<ABaseCharacter>(MeshComp->GetOwner()))
		{
			Character->SetReloadNotify(EReloadState::PickMag);
		}
	}
}