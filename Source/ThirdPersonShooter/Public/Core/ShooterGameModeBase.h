// All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "ShooterGameModeBase.generated.h"

class APlayerCharacter;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnPlayerDiedSignature, AController*, PlayerController);

UCLASS()
class THIRDPERSONSHOOTER_API AShooterGameModeBase : public AGameModeBase
{
	GENERATED_BODY()

public:
	/** Signature to bind delegate */
	UPROPERTY()
	FOnPlayerDiedSignature OnPlayerDied;
	
protected:
	virtual void BeginPlay() override;
	
	/** Called when Player character has died. */
	UFUNCTION()
	virtual void PlayerDied(AController* PlayerController);
};