// Copyright 2022-2023 Danial Kamali. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTaskNode_ToggleBoolState.generated.h"

UCLASS()
class UBTTaskNode_ToggleBoolState : public UBTTaskNode
{
	GENERATED_BODY()

// Functions
public:
	UBTTaskNode_ToggleBoolState(const FObjectInitializer& ObjectInitializer);

private:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

// Variables
public:
	UPROPERTY(EditAnywhere, Category = "NODE")
	FName KeyName;
};