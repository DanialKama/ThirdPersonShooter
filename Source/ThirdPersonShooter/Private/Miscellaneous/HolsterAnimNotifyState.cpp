// Copyright Epic Games, Inc. All Rights Reserved.

#include "Miscellaneous/HolsterAnimNotifyState.h"
#include "Characters/BaseCharacter.h"

void UHolsterAnimNotifyState::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration)
{
	if(MeshComp && MeshComp->GetOwner())
	{
		ABaseCharacter* Character = Cast<ABaseCharacter>(MeshComp->GetOwner());
		if(Character)
		{
			Character->SetHolsterState(EMontageState::Start);
		}
	}
}
void UHolsterAnimNotifyState::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	if(MeshComp && MeshComp->GetOwner())
	{
		ABaseCharacter* Character = Cast<ABaseCharacter>(MeshComp->GetOwner());
		if(Character)
		{
			Character->SetHolsterState(EMontageState::End);
		}
	}
}