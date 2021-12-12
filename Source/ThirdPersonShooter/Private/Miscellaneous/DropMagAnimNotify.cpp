// All Rights Reserved.

#include "Miscellaneous/DropMagAnimNotify.h"
#include "Characters/BaseCharacter.h"

void UDropMagAnimNotify::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	if(MeshComp && MeshComp->GetOwner())
	{
		ABaseCharacter* Character = Cast<ABaseCharacter>(MeshComp->GetOwner());
		if(Character)
		{
			Character->SetReloadState(EReloadState::DropMag);
		}
	}
}