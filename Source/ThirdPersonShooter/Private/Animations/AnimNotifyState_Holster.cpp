// Copyright 2022-2023 Danial Kamali. All Rights Reserved.

#include "AnimNotifyState_Holster.h"

#include "Characters/BaseCharacter.h"

UAnimNotifyState_Holster::UAnimNotifyState_Holster(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
#if WITH_EDITORONLY_DATA
	bShouldFireInEditor = false;
	NotifyColor = FColor(162, 157, 155);
#endif
}

void UAnimNotifyState_Holster::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyBegin(MeshComp, Animation, TotalDuration, EventReference);
	
	if (MeshComp && MeshComp->GetOwner())
	{
		if (ABaseCharacter* Character = Cast<ABaseCharacter>(MeshComp->GetOwner()))
		{
			Character->UpdateHolsterWeaponNotifyState(ENotifyState::Begin);
		}
	}
}

void UAnimNotifyState_Holster::NotifyEnd(USkeletalMeshComponent * MeshComp, UAnimSequenceBase * Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyEnd(MeshComp, Animation, EventReference);
	
	if (MeshComp && MeshComp->GetOwner())
	{
		if (ABaseCharacter* Character = Cast<ABaseCharacter>(MeshComp->GetOwner()))
		{
			Character->UpdateHolsterWeaponNotifyState(ENotifyState::End);
		}
	}
}
