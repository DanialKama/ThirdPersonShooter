// Copyright 2022 Danial Kamali. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "BaseComponent.h"
#include "StaminaComponent.generated.h"

UCLASS()
class THIRDPERSONSHOOTER_API UStaminaComponent : public UBaseComponent
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
	float CurrentStamina;
	
private:
	UPROPERTY(EditDefaultsOnly, Category = "Defaults", meta = (ClampMin = "0.0", UIMin = "0.0", AllowPrivateAccess = true))
	float MaxStamina;

	UPROPERTY(EditDefaultsOnly, Category = "Defaults", meta = (ClampMin = "0.0", UIMin = "0.0", AllowPrivateAccess = true))
	float DefaultStamina;

	UPROPERTY(EditDefaultsOnly, Category = "Defaults", meta = (ClampMin = "0.0", UIMin = "0.0", AllowPrivateAccess = true))
	float RestoreStaminaAmount;

	UPROPERTY(EditDefaultsOnly, Category = "Defaults", meta = (ToolTip = "Per second", ClampMin = "0.0", UIMin = "0.0", AllowPrivateAccess = true))
	float StaminaRestoreRate;
	
	UPROPERTY(EditDefaultsOnly, Category = "Defaults", meta = (ClampMin = "0.0", UIMin = "0.0", AllowPrivateAccess = true))
	float StartStaminaRecoveryDelay;

	UPROPERTY(EditDefaultsOnly, Category = "Defaults", meta = (ClampMin = "0.0", UIMin = "0.0", AllowPrivateAccess = true))
	float RunStaminaDrainAmount;

	UPROPERTY(EditDefaultsOnly, Category = "Defaults", meta = (ClampMin = "0.0", UIMin = "0.0", AllowPrivateAccess = true))
	float SprintStaminaDrainAmount;
	
	uint8 bCharacterInterface : 1, bDoOnce : 1;
	
	FTimerHandle RestoreTimer, RunningTimer, SprintingTimer;
};