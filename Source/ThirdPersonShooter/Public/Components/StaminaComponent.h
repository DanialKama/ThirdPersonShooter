// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "BaseComponent.h"
#include "StaminaComponent.generated.h"

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class THIRDPERSONSHOOTER_API UStaminaComponent : public UBaseComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UStaminaComponent();

	UFUNCTION(BlueprintCallable, Category = "StaminaComponent")
	void StopStaminaDrain();

	UFUNCTION(BlueprintCallable, Category = "StaminaComponent")
	void StartRunning();

	UFUNCTION(BlueprintCallable, Category = "StaminaComponent")
	void StartSprinting();
	
protected:
	virtual void SetupComponent() override;

	// Variables
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Defaults", meta = (ClampMin = "0.0", UIMin = "0.0"))
	float MaxStamina = 100.0f;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Defaults", meta = (ClampMin = "0.0", UIMin = "0.0"))
	float DefaultStamina = 100.0f;

	UPROPERTY(BlueprintReadOnly, Category = "Defaults", meta = (ToolTip = "At begin play Current Stamina is equal to Default Stamina"))
	float CurrentStamina = 0.0f;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Defaults", meta = (ClampMin = "0.0", UIMin = "0.0"))
	float RestoreStaminaAmount = 25.0f;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Defaults", meta = (ToolTip = "Per second", ClampMin = "0.0", UIMin = "0.0"))
	float StaminaRestoreRate = 1.0f;
	
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Defaults", meta = (ClampMin = "0.0", UIMin = "0.0"))
	float StartStaminaRecoveryDelay = 5.0f;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Defaults", meta = (ClampMin = "0.0", UIMin = "0.0"))
	float RunStaminaDrainAmount = 1.0f;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Defaults", meta = (ClampMin = "0.0", UIMin = "0.0"))
	float SprintStaminaDrainAmount = 3.0f;

private:
	// Functions
	UFUNCTION()
	void RestoreStamina();
	UFUNCTION()
	void RunningDrainStamina();
	UFUNCTION()
	void SprintingDrainStamina();
	
	// Variables
	uint8 bCharacterInterface : 1, bDoOnce : 1;
	FTimerHandle RestoreTimer, RunningTimer, SprintingTimer;
};
