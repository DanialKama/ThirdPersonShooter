// All Rights Reserved.

#include "Animations/StandUpAnimNotify.h"
#include "Characters/BaseCharacter.h"

void UStandUpAnimNotify::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	if(MeshComp && MeshComp->GetOwner())
	{
		ABaseCharacter* Character = Cast<ABaseCharacter>(MeshComp->GetOwner());
		if(Character)
		{
			// Set interrupt to true to character movement set to walk
			Character->StanUpMontageHandler(nullptr, true);
		}
	}
}