// All Rights Reserved.

#include "Animations/RemoveMagAnimNotify.h"
#include "Characters/BaseCharacter.h"

void URemoveMagAnimNotify::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	if (MeshComp && MeshComp->GetOwner())
	{
		ABaseCharacter* Character = Cast<ABaseCharacter>(MeshComp->GetOwner());
		if (Character)
		{
			Character->SetReloadNotify(EReloadState::RemoveMag);
		}
	}
}