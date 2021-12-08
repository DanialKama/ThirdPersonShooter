// Copyright Epic Games, Inc. All Rights Reserved.

#include "Miscellaneous/InsertMagAnimNotify.h"
#include "Characters/BaseCharacter.h"

void UInsertMagAnimNotify::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	if(MeshComp && MeshComp->GetOwner())
	{
		ABaseCharacter* Character = Cast<ABaseCharacter>(MeshComp->GetOwner());
		if(Character)
		{
			Character->SetReloadState(EReloadState::InsertMag);
		}
	}
}