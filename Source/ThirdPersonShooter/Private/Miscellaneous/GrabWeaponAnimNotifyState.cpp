// Copyright Epic Games, Inc. All Rights Reserved.

#include "Miscellaneous/GrabWeaponAnimNotifyState.h"
#include "Characters/BaseCharacter.h"

void UGrabWeaponAnimNotifyState::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration)
{
	if(MeshComp && MeshComp->GetOwner())
	{
		ABaseCharacter* Character = Cast<ABaseCharacter>(MeshComp->GetOwner());
		if(Character)
		{
			Character->StartGrabWeapon(EMontageState::Start);
		}
	}
}
void UGrabWeaponAnimNotifyState::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	if(MeshComp && MeshComp->GetOwner())
	{
		ABaseCharacter* Character = Cast<ABaseCharacter>(MeshComp->GetOwner());
		if(Character)
		{
			Character->StartGrabWeapon(EMontageState::End);
		}
	}
}