// Copyright 2022 Danial Kamali. All Rights Reserved.

#include "Animations/AnimNotifyState_Reload.h"
#include "Characters/BaseCharacter.h"

void UAnimNotifyState_Reload::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration)
{
	if (MeshComp && MeshComp->GetOwner())
	{
		ABaseCharacter* Character = Cast<ABaseCharacter>(MeshComp->GetOwner());
		if (Character)
		{
			Character->SetReloadNotify(EReloadState::StartReload);
		}
	}
}

void UAnimNotifyState_Reload::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	if (MeshComp && MeshComp->GetOwner())
	{
		ABaseCharacter* Character = Cast<ABaseCharacter>(MeshComp->GetOwner());
		if (Character)
		{
			Character->SetReloadNotify(EReloadState::EndReload);
		}
	}
}