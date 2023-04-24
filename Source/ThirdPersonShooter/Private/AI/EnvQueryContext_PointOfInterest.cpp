// Copyright 2022-2023 Danial Kamali. All Rights Reserved.

#include "EnvQueryContext_PointOfInterest.h"

#include "AIModule/Classes/EnvironmentQuery/Items/EnvQueryItemType_Actor.h"
#include "Kismet/GameplayStatics.h"
#include "PointOfInterest.h"

void UEnvQueryContext_PointOfInterest::ProvideContext(FEnvQueryInstance& QueryInstance, FEnvQueryContextData& ContextData) const
{
	TArray<AActor*> ActorSet;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), APointOfInterest::StaticClass(), ActorSet);

	UEnvQueryItemType_Actor::SetContextHelper(ContextData, ActorSet);
}
