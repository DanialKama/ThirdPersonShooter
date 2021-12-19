// All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Characters/BaseCharacter.h"
#include "Components/TimelineComponent.h"
#include "Interfaces/PlayerCharacterInterface.h"
#include "PlayerCharacter.generated.h"

class AShooterPlayerController;
class AShooterHUD;
class USpringArmComponent;
class UCurveFloat;

UCLASS()
class THIRDPERSONSHOOTER_API APlayerCharacter : public ABaseCharacter, public IPlayerCharacterInterface
{
	GENERATED_BODY()

	// ReSharper disable once CppUE4ProbableMemoryIssuesWithUObject
	UTimelineComponent* AimTimeline;
	
public:
	// Sets default values for this character's properties
	APlayerCharacter();
	
	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	virtual void Destroyed() override;

	// Components
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	USpringArmComponent* SpringArm;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UCameraComponent* TPP;

	//Functions
	virtual void ResetAim() override;
	// Interfaces
	virtual void SetHealthState_Implementation(EHealthState HealthState) override;
	virtual void SetHealthLevel_Implementation(float Health) override;
	virtual void SetStaminaLevel_Implementation(float Stamina, bool bIsFull) override;
	virtual void AddRecoil_Implementation(FRotator RotationIntensity, float ControlTime, float CrosshairRecoil, float ControllerPitch) override;
	virtual APlayerCharacter* GetPlayerCharacterReference_Implementation() override;
	virtual bool IsPlayer_Implementation() override;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Functions
	virtual void SetCurrentWeapon(APickupWeapon* NewCurrentWeapon, EWeaponToDo WeaponSlot) override;
	
private:
	// Functions
	void GamepadAddToYaw(float AxisValue);
	void GamepadAddToPitch(float AxisValue);
	void AddToForwardMovement(float AxisValue);
	void AddToRightMovement(float AxisValue);
	void StartSprinting();
	void StopSprinting();
	void TryToToggleCrouch();
	/** To not spamming crouch */
	void ResetCrouchByDelay();
	void ResetCrouch();
	void TryToStartAiming();
	void SwitchToNextWeapon();
	void SwitchToPreviousWeapon();
	void DoubleTabGate();
	void DoubleTabHandler();
	UFUNCTION()
	void AimTimeLineUpdate(float Value);
	UFUNCTION()
	void AimTimeLineFinished();
	
	// Variables
	uint8 bTapHeld : 1, bDoubleTabGate : 1, bDoOnceCrouch : 1;
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Defaults", meta = (AllowPrivateAccess = "true"))
	float BaseTurnRate = 45.0f;
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Defaults", meta = (AllowPrivateAccess = "true"))
	float BaseLookUpRate = 45.0f;
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Defaults", meta = (ToolTip = "Use for double tab function to set movement state to run and sprint", AllowPrivateAccess = "true"))
	float TapThreshold = 0.2f;
	UPROPERTY(EditDefaultsOnly, Category = "Defaults", meta = (AllowPrivateAccess = "true"))
	UCurveFloat* AimFloatCurve;
	UPROPERTY()
	AShooterPlayerController* PlayerController;
	UPROPERTY()
	AShooterHUD* HUD;
	uint8 TabNumber, PreviousTapNumber;
	/** Enum data indicating the direction the Timeline is playing */
	TEnumAsByte<ETimelineDirection::Type> Direction;
	FTimerHandle StartSprintingTimer;
};