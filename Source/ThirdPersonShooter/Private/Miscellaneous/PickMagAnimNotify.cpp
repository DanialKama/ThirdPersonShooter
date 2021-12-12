// All Rights Reserved.

#include "Miscellaneous/PickMagAnimNotify.h"
#include "Characters/BaseCharacter.h"

void UPickMagAnimNotify::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	if(MeshComp && MeshComp->GetOwner())
	{
		ABaseCharacter* Character = Cast<ABaseCharacter>(MeshComp->GetOwner());
		if(Character)
		{
			Character->SetReloadState(EReloadState::PickMag);
		}
	}
}