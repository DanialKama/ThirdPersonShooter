// Copyright 2022-2023 Danial Kamali. All Rights Reserved.

#include "AnimNotify_StandUp.h"

#include "Characters/BaseCharacter.h"

UAnimNotify_StandUp::UAnimNotify_StandUp(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
#if WITH_EDITORONLY_DATA
	bShouldFireInEditor = false;
	NotifyColor = FColor(171, 159, 158);
#endif
}

void UAnimNotify_StandUp::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation, EventReference);
	
	if (MeshComp && MeshComp->GetOwner())
	{
		if (const ABaseCharacter* Character = Cast<ABaseCharacter>(MeshComp->GetOwner()))
		{
			// Set interrupt to true to character movement set to walk
			Character->StanUpMontageHandler(nullptr, true);
		}
	}
}
