// Copyright 2022-2023 Danial Kamali. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "BaseComponent.h"
#include "StaminaComponent.generated.h"

UCLASS()
class UStaminaComponent : public UBaseComponent
{
	GENERATED_BODY()

// Functions
public:
	UStaminaComponent();

	virtual void Initialize() override;
	
	UFUNCTION(BlueprintCallable, Category = "StaminaComponent")
	void StopStaminaDrain();

	UFUNCTION(BlueprintCallable, Category = "StaminaComponent")
	void StartRunning();

	UFUNCTION(BlueprintCallable, Category = "StaminaComponent")
	void StartSprinting();

private:
	UFUNCTION()
	void RestoreStamina();
	
	UFUNCTION()
	void RunningDrainStamina();
	
	UFUNCTION()
	void SprintingDrainStamina();

// Variables
public:
	/** At begin play Current Stamina is equal to Default Stamina */
	float CurrentStamina = 0.0f;
	
private:
	UPROPERTY(EditDefaultsOnly, Category = "Default", meta = (ClampMin = "0.0", UIMin = "0.0", AllowPrivateAccess = true))
	float MaxStamina = 100.0f;

	UPROPERTY(EditDefaultsOnly, Category = "Default", meta = (ClampMin = "0.0", UIMin = "0.0", AllowPrivateAccess = true))
	float DefaultStamina = 100.0f;

	UPROPERTY(EditDefaultsOnly, Category = "Default", meta = (ClampMin = "0.0", UIMin = "0.0", AllowPrivateAccess = true))
	float RestoreStaminaAmount = 25.0f;

	/** Per second */
	UPROPERTY(EditDefaultsOnly, Category = "Default", meta = (ClampMin = "0.0", UIMin = "0.0", AllowPrivateAccess = true))
	float StaminaRestoreRate = 1.0f;
	
	UPROPERTY(EditDefaultsOnly, Category = "Default", meta = (ClampMin = "0.0", UIMin = "0.0", AllowPrivateAccess = true))
	float StartStaminaRecoveryDelay = 5.0f;

	UPROPERTY(EditDefaultsOnly, Category = "Default", meta = (ClampMin = "0.0", UIMin = "0.0", AllowPrivateAccess = true))
	float RunStaminaDrainAmount = 1.0f;

	UPROPERTY(EditDefaultsOnly, Category = "Default", meta = (ClampMin = "0.0", UIMin = "0.0", AllowPrivateAccess = true))
	float SprintStaminaDrainAmount = 3.0f;
	
	uint8 bCharacterInterface : 1, bDoOnce : 1;
	
	FTimerHandle RestoreTimer, RunningTimer, SprintingTimer;
};
