// Copyright 2022-2023 Danial Kamali. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "BaseCharacter.h"
#include "Components/TimelineComponent.h"
#include "PlayerCharacter.generated.h"

UCLASS()
class APlayerCharacter : public ABaseCharacter
{
	GENERATED_BODY()

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = true))
	class USpringArmComponent* SpringArm;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = true))
	UCameraComponent* Camera;
	
	// ReSharper disable once CppUE4ProbableMemoryIssuesWithUObject
	UTimelineComponent* AimTimeline;

// Functions
public:
	APlayerCharacter();
	
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	virtual void ResetAim() override;
	virtual void SetHealthState_Implementation(EHealthState HealthState) override;
	virtual void SetHealthLevel_Implementation(float Health) override;
	virtual void SetStaminaLevel_Implementation(float Stamina, bool bIsFull) override;
	virtual void AddRecoil_Implementation(FRotator RotationIntensity, float ControlTime, float CrosshairRecoil, float ControllerPitch) override;
	virtual bool IsPlayer_Implementation() override;

protected:
	virtual void BeginPlay() override;
	virtual void Destroyed() override;

	virtual void SetCurrentWeapon(APickupWeapon* NewCurrentWeapon, EWeaponToDo WeaponSlot) override;

private:
	/** Wait for one frame to access a valid player controller */
	UFUNCTION()
	void NextFrameBeginPlay();

	/** Called for forwards/backward input */
	void AddToForwardMovement(float AxisValue);

	/** Called for side to side input */
	void AddToRightMovement(float AxisValue);

	/** 
	 * Called via input to turn at a given rate. 
	 * @param AxisValue	This is a normalized rate, i.e. 1.0 means 100% of desired turn rate
	 */
	void GamepadAddToYaw(float AxisValue);

	/**
	 * Called via input to turn look up/down at a given rate. 
	 * @param AxisValue	This is a normalized rate, i.e. 1.0 means 100% of desired turn rate
	 */
	void GamepadAddToPitch(float AxisValue);
	
	void StartSprinting();
	void StopSprinting();
	void TryToToggleCrouch();
	
	/** To not spamming crouch */
	void ResetCrouchByDelay();

	UFUNCTION()
	void ResetCrouch();

	UFUNCTION()
	void TryToMeleeAttack();
	
	void TryToStartAiming();
	void SwitchToNextWeapon();
	void SwitchToPreviousWeapon();

	UFUNCTION()
	void DoubleTabGate();

	UFUNCTION()
	void DoubleTabHandler();
	
	UFUNCTION()
	void AimTimeLineUpdate(float Value);
	
	UFUNCTION()
	void AimTimeLineFinished();

// Variables
private:
	UPROPERTY(EditDefaultsOnly, Category = "Defaults", meta = (AllowPrivateAccess = "true"))
	float BaseTurnRate;
	
	UPROPERTY(EditDefaultsOnly, Category = "Defaults", meta = (AllowPrivateAccess = "true"))
	float BaseLookUpRate;
	
	UPROPERTY(EditDefaultsOnly, Category = "Defaults", meta = (ToolTip = "Use for double tab function to set movement state to run and sprint", AllowPrivateAccess = "true"))
	float TapThreshold;
	
	UPROPERTY(EditDefaultsOnly, Category = "Defaults", meta = (AllowPrivateAccess = "true"))
	UCurveFloat* AimFloatCurve;

	uint8 bTapHeld : 1, bDoubleTabGate : 1, bDoOnceCrouch : 1;
	
	UPROPERTY()
	class AShooterPlayerController* PlayerController;
	
	UPROPERTY()
	class AShooterHUD* HUDRef;
	
	uint8 TabNumber, PreviousTapNumber;
	
	/** Enum data indicating the direction the Timeline is playing */
	TEnumAsByte<ETimelineDirection::Type> Direction;
	
	FTimerHandle StartSprintingTimer;
};