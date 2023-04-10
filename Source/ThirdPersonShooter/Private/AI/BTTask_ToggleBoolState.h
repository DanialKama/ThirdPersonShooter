// Copyright 2022-2023 Danial Kamali. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTask_ToggleBoolState.generated.h"

UCLASS()
class UBTTask_ToggleBoolState : public UBTTaskNode
{
	GENERATED_UCLASS_BODY()

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

// Variables
public:
	UPROPERTY(EditAnywhere, Category = "NODE")
	FName KeyName = NAME_None;
};
