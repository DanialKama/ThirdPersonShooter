// Copyright 2022 Danial Kamali. All Rights Reserved.

#include "AnimNotify_InsertMag.h"
#include "Characters/BaseCharacter.h"

void UAnimNotify_InsertMag::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	if (MeshComp && MeshComp->GetOwner())
	{
		if (ABaseCharacter* Character = Cast<ABaseCharacter>(MeshComp->GetOwner()))
		{
			Character->SetReloadNotify(EReloadState::InsertMag);
		}
	}
}