// Copyright 2022 Danial Kamali. All Rights Reserved.

#include "AnimNotifyState_GrabWeapon.h"
#include "Characters/BaseCharacter.h"

void UAnimNotifyState_GrabWeapon::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration)
{
	if (MeshComp && MeshComp->GetOwner())
	{
		if (ABaseCharacter* Character = Cast<ABaseCharacter>(MeshComp->GetOwner()))
		{
			Character->UpdateGrabWeaponNotifyState(ENotifyState::Begin);
		}
	}
}
void UAnimNotifyState_GrabWeapon::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	if (MeshComp && MeshComp->GetOwner())
	{
		if (ABaseCharacter* Character = Cast<ABaseCharacter>(MeshComp->GetOwner()))
		{
			Character->UpdateGrabWeaponNotifyState(ENotifyState::End);
		}
	}
}