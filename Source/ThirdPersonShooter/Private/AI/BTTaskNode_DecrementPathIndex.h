// Copyright 2022-2023 Danial Kamali. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTaskNode_DecrementPathIndex.generated.h"

UCLASS()
class UBTTaskNode_DecrementPathIndex : public UBTTaskNode
{
	GENERATED_BODY()

// Functions
public:
	UBTTaskNode_DecrementPathIndex(const FObjectInitializer& ObjectInitializer);

private:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
};