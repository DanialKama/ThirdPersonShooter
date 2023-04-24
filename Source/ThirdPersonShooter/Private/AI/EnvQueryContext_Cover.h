// Copyright 2022-2023 Danial Kamali. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "AIModule/Classes/EnvironmentQuery/EnvQueryContext.h"
#include "EnvQueryContext_Cover.generated.h"

UCLASS(MinimalAPI)
class UEnvQueryContext_Cover : public UEnvQueryContext
{
	GENERATED_BODY()

	virtual void ProvideContext(FEnvQueryInstance& QueryInstance, FEnvQueryContextData& ContextData) const override;
};
