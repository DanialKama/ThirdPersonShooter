// All Rights Reserved.

#include "Animations/PickMagAnimNotify.h"
#include "Characters/BaseCharacter.h"

void UPickMagAnimNotify::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
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