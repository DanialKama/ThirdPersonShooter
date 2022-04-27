// Copyright 2022 Danial Kamali. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTaskNode_IncrementPathIndex.generated.h"

UCLASS()
class THIRDPERSONSHOOTER_API UBTTaskNode_IncrementPathIndex : public UBTTaskNode
{
	GENERATED_BODY()

// Functions
public:
	UBTTaskNode_IncrementPathIndex(const FObjectInitializer& ObjectInitializer);

private:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
};