// All Rights Reserved.

#include "Miscellaneous/GrabWeaponAnimNotifyState.h"
#include "Characters/BaseCharacter.h"

void UGrabWeaponAnimNotifyState::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration)
{
	if(MeshComp && MeshComp->GetOwner())
	{
		ABaseCharacter* Character = Cast<ABaseCharacter>(MeshComp->GetOwner());
		if(Character)
		{
			Character->UpdateGrabWeaponNotifyState(ENotifyState::Begin);
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
			Character->UpdateGrabWeaponNotifyState(ENotifyState::End);
		}
	}
}