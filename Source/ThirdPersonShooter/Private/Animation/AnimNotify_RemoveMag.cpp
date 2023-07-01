// Copyright 2022-2023 Danial Kamali. All Rights Reserved.

#include "AnimNotify_RemoveMag.h"

#include "Characters/BaseCharacter.h"

UAnimNotify_RemoveMag::UAnimNotify_RemoveMag(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
#if WITH_EDITORONLY_DATA
	bShouldFireInEditor = false;
	NotifyColor = FColor(221, 188, 190);
#endif
}

void UAnimNotify_RemoveMag::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation, EventReference);
	
	if (MeshComp && MeshComp->GetOwner())
	{
		if (ABaseCharacter* Character = Cast<ABaseCharacter>(MeshComp->GetOwner()))
		{
			Character->SetReloadNotify(EReloadState::RemoveMag);
		}
	}
}
