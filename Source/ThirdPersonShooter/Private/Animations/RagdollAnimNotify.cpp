// All Rights Reserved.

#include "Animations/RagdollAnimNotify.h"
#include "Characters/BaseCharacter.h"

void URagdollAnimNotify::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	if(MeshComp && MeshComp->GetOwner())
	{
		ABaseCharacter* Character = Cast<ABaseCharacter>(MeshComp->GetOwner());
		if(Character)
		{
			Character->DeathMontageHandler(nullptr, false);
		}
	}
}