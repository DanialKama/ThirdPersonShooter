// Copyright 2022 Danial Kamali. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTaskNode_UseWeapon.generated.h"

UCLASS()
class THIRDPERSONSHOOTER_API UBTTaskNode_UseWeapon : public UBTTaskNode
{
	GENERATED_BODY()

// Functions
public:
	UBTTaskNode_UseWeapon(const FObjectInitializer& ObjectInitializer);

private:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

// Variables
public:
	UPROPERTY(EditAnywhere, Category = "NODE")
	uint8 Aim : 1;

	UPROPERTY(EditAnywhere, Category = "NODE")
	uint8 Fire : 1;
};