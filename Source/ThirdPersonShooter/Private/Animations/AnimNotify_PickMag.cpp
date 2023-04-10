// Copyright 2022-2023 Danial Kamali. All Rights Reserved.

#include "AnimNotify_PickMag.h"

#include "Characters/BaseCharacter.h"

UAnimNotify_PickMag::UAnimNotify_PickMag(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
#if WITH_EDITORONLY_DATA
	bShouldFireInEditor = false;
	NotifyColor = FColor(192, 183, 178);
#endif
}

void UAnimNotify_PickMag::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation, EventReference);
	
	if (MeshComp && MeshComp->GetOwner())
	{
		if (ABaseCharacter* Character = Cast<ABaseCharacter>(MeshComp->GetOwner()))
		{
			Character->SetReloadNotify(EReloadState::PickMag);
		}
	}
}
