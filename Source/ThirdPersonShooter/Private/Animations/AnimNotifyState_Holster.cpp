// Copyright 2022 Danial Kamali. All Rights Reserved.

#include "Animations/AnimNotifyState_Holster.h"
#include "Characters/BaseCharacter.h"

void UAnimNotifyState_Holster::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration)
{
	if (MeshComp && MeshComp->GetOwner())
	{
		ABaseCharacter* Character = Cast<ABaseCharacter>(MeshComp->GetOwner());
		if (Character)
		{
			Character->UpdateHolsterWeaponNotifyState(ENotifyState::Begin);
		}
	}
}
void UAnimNotifyState_Holster::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	if (MeshComp && MeshComp->GetOwner())
	{
		ABaseCharacter* Character = Cast<ABaseCharacter>(MeshComp->GetOwner());
		if (Character)
		{
			Character->UpdateHolsterWeaponNotifyState(ENotifyState::End);
		}
	}
}