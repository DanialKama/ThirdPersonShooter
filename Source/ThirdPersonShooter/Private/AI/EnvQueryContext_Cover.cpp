// Copyright 2022-2023 Danial Kamali. All Rights Reserved.

#include "EnvQueryContext_Cover.h"

#include "AIModule/Classes/EnvironmentQuery/Items/EnvQueryItemType_Actor.h"
#include "Cover.h"
#include "Kismet/GameplayStatics.h"

void UEnvQueryContext_Cover::ProvideContext(FEnvQueryInstance& QueryInstance, FEnvQueryContextData& ContextData) const
{
	TArray<AActor*> ActorSet;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ACover::StaticClass(), ActorSet);

	UEnvQueryItemType_Actor::SetContextHelper(ContextData, ActorSet);
}
