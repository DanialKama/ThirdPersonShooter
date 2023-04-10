// Copyright 2022-2023 Danial Kamali. All Rights Reserved.

#include "AnimNotifyState_Reload.h"

#include "Characters/BaseCharacter.h"

UAnimNotifyState_Reload::UAnimNotifyState_Reload(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
#if WITH_EDITORONLY_DATA
	bShouldFireInEditor = false;
	NotifyColor = FColor(166, 153, 142);
#endif
}

void UAnimNotifyState_Reload::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyBegin(MeshComp, Animation, TotalDuration, EventReference);
	
	if (MeshComp && MeshComp->GetOwner())
	{
		if (ABaseCharacter* Character = Cast<ABaseCharacter>(MeshComp->GetOwner()))
		{
			Character->SetReloadNotify(EReloadState::StartReload);
		}
	}
}

void UAnimNotifyState_Reload::NotifyEnd(USkeletalMeshComponent * MeshComp, UAnimSequenceBase * Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyEnd(MeshComp, Animation, EventReference);
	
	if (MeshComp && MeshComp->GetOwner())
	{
		if (ABaseCharacter* Character = Cast<ABaseCharacter>(MeshComp->GetOwner()))
		{
			Character->SetReloadNotify(EReloadState::EndReload);
		}
	}
}
