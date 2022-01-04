// All Rights Reserved.

#include "Animations/AnimNotify_InsertMag.h"
#include "Characters/BaseCharacter.h"

void UAnimNotify_InsertMag::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	if (MeshComp && MeshComp->GetOwner())
	{
		ABaseCharacter* Character = Cast<ABaseCharacter>(MeshComp->GetOwner());
		if (Character)
		{
			Character->SetReloadNotify(EReloadState::InsertMag);
		}
	}
}