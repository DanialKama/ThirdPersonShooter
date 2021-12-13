// All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Characters/BaseCharacter.h"
#include "PlayerCharacter.generated.h"

class AShooterPlayerController;
class AShooterHUD;
class USpringArmComponent;

UCLASS()
class THIRDPERSONSHOOTER_API APlayerCharacter : public ABaseCharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	APlayerCharacter();
	
	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	// Components
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	USpringArmComponent* SpringArm;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UCameraComponent* TPP;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
private:
	// Functions
	void GamepadAddToYaw(float Value);
	void GamepadAddToPitch(float Value);
	void MouseAddToYaw(float Value);
	void MouseAddToPitch(float Value);
	void AddToForwardMovement(float Value);
	void AddToRightMovement(float Value);
	void StartSprinting();
	void StopSprinting();
	void TryToToggleCrouch();
	/** To not spamming crouch */
	void ResetCrouchByDelay();
	void StopCrouching();
	void TryToStartAiming();
	void TryToStopAiming();
	void SwitchToNextWeapon();
	void SwitchToPreviousWeapon();
	void DoubleTabGate();
	void DoubleTabHandler();
	
	// Variables
	uint8 bTapHeld : 1, bDoubleTabGate : 1, bDoOnceCrouch : 1;
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Defaults", meta = (AllowPrivateAccess = "true"))
	float BaseTurnRate = 45.0f;
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Defaults", meta = (AllowPrivateAccess = "true"))
	float BaseLookUpRate = 45.0f;
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Defaults", meta = (ToolTip = "Use for double tab function to set movement state to run and sprint", AllowPrivateAccess = "true"))
	float TapThreshold = 0.2f;
	UPROPERTY()
	AShooterPlayerController* PlayerController;
	UPROPERTY()
	AShooterHUD* HUD;
	uint8 TabNumber, PreviousTapNumber;
};