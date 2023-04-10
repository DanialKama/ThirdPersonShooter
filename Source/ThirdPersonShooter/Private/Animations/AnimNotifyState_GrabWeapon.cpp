// Copyright 2022-2023 Danial Kamali. All Rights Reserved.

#include "AnimNotifyState_GrabWeapon.h"

#include "Characters/BaseCharacter.h"

UAnimNotifyState_GrabWeapon::UAnimNotifyState_GrabWeapon(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
#if WITH_EDITORONLY_DATA
	bShouldFireInEditor = false;
	NotifyColor = FColor(171, 195, 164);
#endif
}

void UAnimNotifyState_GrabWeapon::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyBegin(MeshComp, Animation, TotalDuration, EventReference);
	
	if (MeshComp && MeshComp->GetOwner())
	{
		if (ABaseCharacter* Character = Cast<ABaseCharacter>(MeshComp->GetOwner()))
		{
			Character->UpdateGrabWeaponNotifyState(ENotifyState::Begin);
		}
	}
}

void UAnimNotifyState_GrabWeapon::NotifyEnd(USkeletalMeshComponent * MeshComp, UAnimSequenceBase * Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyEnd(MeshComp, Animation, EventReference);
	
	if (MeshComp && MeshComp->GetOwner())
	{
		if (ABaseCharacter* Character = Cast<ABaseCharacter>(MeshComp->GetOwner()))
		{
			Character->UpdateGrabWeaponNotifyState(ENotifyState::End);
		}
	}
}
