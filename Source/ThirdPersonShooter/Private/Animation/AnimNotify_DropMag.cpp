// Copyright 2022-2023 Danial Kamali. All Rights Reserved.

#include "AnimNotify_DropMag.h"

#include "Characters/BaseCharacter.h"

UAnimNotify_DropMag::UAnimNotify_DropMag(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
#if WITH_EDITORONLY_DATA
	bShouldFireInEditor = false;
	NotifyColor = FColor(162, 186, 209);
#endif
}

void UAnimNotify_DropMag::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation, EventReference);
	
	if (MeshComp && MeshComp->GetOwner())
	{
		if (ABaseCharacter* Character = Cast<ABaseCharacter>(MeshComp->GetOwner()))
		{
			Character->SetReloadNotify(EReloadState::DropMag);
		}
	}
}
