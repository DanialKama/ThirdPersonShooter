// Copyright 2022-2023 Danial Kamali. All Rights Reserved.

#include "AnimNotify_InsertMag.h"

#include "Characters/BaseCharacter.h"

UAnimNotify_InsertMag::UAnimNotify_InsertMag(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
#if WITH_EDITORONLY_DATA
	bShouldFireInEditor = false;
	NotifyColor = FColor(190, 189, 191);
#endif
}

void UAnimNotify_InsertMag::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation, EventReference);
	
	if (MeshComp && MeshComp->GetOwner())
	{
		if (ABaseCharacter* Character = Cast<ABaseCharacter>(MeshComp->GetOwner()))
		{
			Character->SetReloadNotify(EReloadState::InsertMag);
		}
	}
}
