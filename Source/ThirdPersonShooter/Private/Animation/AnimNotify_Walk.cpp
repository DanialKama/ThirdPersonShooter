// Copyright 2022-2023 Danial Kamali. All Rights Reserved.

#include "AnimNotify_Walk.h"

#include "AnimInstance_Shooter.h"

UAnimNotify_Walk::UAnimNotify_Walk() : Super()
{
#if WITH_EDITORONLY_DATA
	NotifyColor = FColor::Green;
	bShouldFireInEditor = false;
#endif
}

void UAnimNotify_Walk::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation, EventReference);

	const UAnimInstance_Shooter* AnimInstance = Cast<UAnimInstance_Shooter>(MeshComp->AnimScriptInstance);
	AnimInstance->PlayFootstepSound(true);
}
