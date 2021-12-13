// All Rights Reserved.

#include "Animations/ReloadAnimNotifyState.h"
#include "Characters/BaseCharacter.h"

void UReloadAnimNotifyState::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration)
{
	if(MeshComp && MeshComp->GetOwner())
	{
		ABaseCharacter* Character = Cast<ABaseCharacter>(MeshComp->GetOwner());
		if(Character)
		{
			Character->SetReloadNotify(EReloadState::StartReload);
		}
	}
}

void UReloadAnimNotifyState::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	if(MeshComp && MeshComp->GetOwner())
	{
		ABaseCharacter* Character = Cast<ABaseCharacter>(MeshComp->GetOwner());
		if(Character)
		{
			Character->SetReloadNotify(EReloadState::EndReload);
		}
	}
}