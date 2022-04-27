// Copyright 2022 Danial Kamali. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTaskNode_LoopPath.generated.h"

UCLASS()
class THIRDPERSONSHOOTER_API UBTTaskNode_LoopPath : public UBTTaskNode
{
	GENERATED_BODY()

// Functions
public:
	UBTTaskNode_LoopPath(const FObjectInitializer& ObjectInitializer);

private:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
};