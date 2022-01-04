// All Rights Reserved.

#include "Animations/AnimNotify_PickMag.h"
#include "Characters/BaseCharacter.h"

void UAnimNotify_PickMag::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	if (MeshComp && MeshComp->GetOwner())
	{
		ABaseCharacter* Character = Cast<ABaseCharacter>(MeshComp->GetOwner());
		if (Character)
		{
			Character->SetReloadNotify(EReloadState::PickMag);
		}
	}
}