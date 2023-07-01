// Copyright 2022-2023 Danial Kamali. All Rights Reserved.

#include "AnimNotify_Run.h"

#include "AnimInstance_Shooter.h"

UAnimNotify_Run::UAnimNotify_Run() : Super()
{
#if WITH_EDITORONLY_DATA
	NotifyColor = FColor::Green;
	bShouldFireInEditor = false;
#endif
}

void UAnimNotify_Run::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation, EventReference);

	const UAnimInstance_Shooter* AnimInstance = Cast<UAnimInstance_Shooter>(MeshComp->AnimScriptInstance);
	AnimInstance->PlayFootstepSound(false);
}
