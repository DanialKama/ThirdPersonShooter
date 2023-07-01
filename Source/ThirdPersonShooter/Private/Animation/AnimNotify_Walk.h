// Copyright 2022-2023 Danial Kamali. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "AnimNotify_Walk.generated.h"

/**
 * Used to play walking footstep sound
 */
UCLASS()
class UAnimNotify_Walk : public UAnimNotify
{
	GENERATED_BODY()
	
// Functions
public:
	UAnimNotify_Walk();

	virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference) override;
};
