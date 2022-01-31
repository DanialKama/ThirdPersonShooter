// Copyright 2022 Danial Kamali. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "ShooterGameModeBase.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnPlayerDiedSignature, AController*, PlayerController);

UCLASS()
class THIRDPERSONSHOOTER_API AShooterGameModeBase : public AGameModeBase
{
	GENERATED_BODY()

// Functions
protected:
	virtual void BeginPlay() override;
	
	/** Called when Player character has died. */
	UFUNCTION()
	virtual void PlayerDied(AController* PlayerController);

public:
	/** Signature to bind delegate */
	UPROPERTY()
	FOnPlayerDiedSignature OnPlayerDied;
};