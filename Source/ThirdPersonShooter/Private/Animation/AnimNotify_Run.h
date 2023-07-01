// Copyright 2022-2023 Danial Kamali. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "AnimNotify_Run.generated.h"

/**
 * Used to play running footstep sound
 */
UCLASS()
class UAnimNotify_Run : public UAnimNotify
{
	GENERATED_BODY()
	
// Functions
public:
	UAnimNotify_Run();

	virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference) override;
};
