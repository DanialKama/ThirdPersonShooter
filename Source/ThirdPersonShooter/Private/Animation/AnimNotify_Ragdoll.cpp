// Copyright 2022-2023 Danial Kamali. All Rights Reserved.

#include "AnimNotify_Ragdoll.h"

#include "Characters/BaseCharacter.h"

UAnimNotify_Ragdoll::UAnimNotify_Ragdoll(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
#if WITH_EDITORONLY_DATA
	bShouldFireInEditor = false;
	NotifyColor = FColor(156, 152, 148);
#endif
}

void UAnimNotify_Ragdoll::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation, EventReference);
	
	if (MeshComp && MeshComp->GetOwner())
	{
		if (ABaseCharacter* Character = Cast<ABaseCharacter>(MeshComp->GetOwner()))
		{
			Character->DeathMontageHandler(nullptr, false);
		}
	}
}
