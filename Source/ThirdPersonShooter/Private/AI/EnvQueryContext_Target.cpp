// Copyright 2022-2023 Danial Kamali. All Rights Reserved.

#include "EnvQueryContext_Target.h"

#include "AIModule/Classes/AIController.h"
#include "AIModule/Classes/EnvironmentQuery/EnvQueryTypes.h"
#include "AIModule/Classes/EnvironmentQuery/Items/EnvQueryItemType_Actor.h"
#include "GameFramework/Actor.h"
#include "GameFramework/Pawn.h"

void UEnvQueryContext_Target::ProvideContext(FEnvQueryInstance& QueryInstance, FEnvQueryContextData& ContextData) const
{
	UObject* QuerierObject = QueryInstance.Owner.Get();
	AActor* QuerierActor = Cast<AActor>(QuerierObject);
	const APawn* AsPawn = Cast<APawn>(QuerierActor);
	const AAIController* AIController = Cast<AAIController>(AsPawn->GetController());

	UEnvQueryItemType_Actor::SetContextHelper(ContextData, AIController->GetFocusActor());
}
