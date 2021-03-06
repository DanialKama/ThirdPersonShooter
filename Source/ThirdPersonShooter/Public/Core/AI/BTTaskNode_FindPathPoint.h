// Copyright 2022 Danial Kamali. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTaskNode_FindPathPoint.generated.h"

UCLASS()
class THIRDPERSONSHOOTER_API UBTTaskNode_FindPathPoint : public UBTTaskNode
{
	GENERATED_BODY()

// Functions
public:
	UBTTaskNode_FindPathPoint(const FObjectInitializer& ObjectInitializer);

private:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
};