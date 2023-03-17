// Copyright 2022-2023 Danial Kamali. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTaskNode_Healing.generated.h"

UCLASS()
class UBTTaskNode_Healing : public UBTTaskNode
{
	GENERATED_BODY()

// Functions
public:
	UBTTaskNode_Healing(const FObjectInitializer& ObjectInitializer);

private:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

// Variables
public:
	UPROPERTY(EditAnywhere, Category = "NODE")
	UAnimMontage* HealingMontage;
};