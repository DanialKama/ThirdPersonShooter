// All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "BaseComponent.h"
#include "StaminaComponent.generated.h"

UCLASS()
class THIRDPERSONSHOOTER_API UStaminaComponent : public UBaseComponent
{
	GENERATED_BODY()

public:
	UStaminaComponent();

	UFUNCTION(BlueprintCallable, Category = "StaminaComponent")
	void StopStaminaDrain();

	UFUNCTION(BlueprintCallable, Category = "StaminaComponent")
	void StartRunning();

	UFUNCTION(BlueprintCallable, Category = "StaminaComponent")
	void StartSprinting();

	/** At begin play Current Stamina is equal to Default Stamina */
	float CurrentStamina = 0.0f;
	
protected:
	virtual void SetupComponent() override;

	UPROPERTY(EditDefaultsOnly, Category = "Defaults", meta = (ClampMin = "0.0", UIMin = "0.0"))
	float MaxStamina = 100.0f;

	UPROPERTY(EditDefaultsOnly, Category = "Defaults", meta = (ClampMin = "0.0", UIMin = "0.0"))
	float DefaultStamina = 100.0f;

	UPROPERTY(EditDefaultsOnly, Category = "Defaults", meta = (ClampMin = "0.0", UIMin = "0.0"))
	float RestoreStaminaAmount = 25.0f;

	UPROPERTY(EditDefaultsOnly, Category = "Defaults", meta = (ToolTip = "Per second", ClampMin = "0.0", UIMin = "0.0"))
	float StaminaRestoreRate = 1.0f;
	
	UPROPERTY(EditDefaultsOnly, Category = "Defaults", meta = (ClampMin = "0.0", UIMin = "0.0"))
	float StartStaminaRecoveryDelay = 5.0f;

	UPROPERTY(EditDefaultsOnly, Category = "Defaults", meta = (ClampMin = "0.0", UIMin = "0.0"))
	float RunStaminaDrainAmount = 1.0f;

	UPROPERTY(EditDefaultsOnly, Category = "Defaults", meta = (ClampMin = "0.0", UIMin = "0.0"))
	float SprintStaminaDrainAmount = 3.0f;

private:
	UFUNCTION()
	void RestoreStamina();
	
	UFUNCTION()
	void RunningDrainStamina();
	
	UFUNCTION()
	void SprintingDrainStamina();
	
	uint8 bCharacterInterface : 1, bDoOnce : 1;
	FTimerHandle RestoreTimer, RunningTimer, SprintingTimer;
};