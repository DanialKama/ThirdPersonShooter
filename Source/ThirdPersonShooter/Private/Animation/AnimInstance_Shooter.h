// Copyright 2022-2023 Danial Kamali. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "AnimInstance_Shooter.generated.h"

/**
 * Shooter Animation Instance
 */
UCLASS(meta = (DisplayName = "Shooter Anim Instance"))
class UAnimInstance_Shooter : public UAnimInstance
{
	GENERATED_BODY()
	
// Functions
public:
	UAnimInstance_Shooter();
	
	virtual void NativeInitializeAnimation() override;
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;
	
	void PlayFootstepSound(bool bIsWalking) const;

private:
	void LoadAssets();

// Variables
private:
	UPROPERTY(EditDefaultsOnly, Category = "Default", meta = (AllowPrivateAccess = true))
	TSoftObjectPtr<USoundBase> WalkOnDirt;

	UPROPERTY()
	USoundBase* WalkOnDirtLoaded;

	UPROPERTY(EditDefaultsOnly, Category = "Default", meta = (AllowPrivateAccess = true))
	TSoftObjectPtr<USoundBase> WalkOnStone;

	UPROPERTY()
	USoundBase* WalkOnStoneLoaded;

	UPROPERTY(EditDefaultsOnly, Category = "Default", meta = (AllowPrivateAccess = true))
	TSoftObjectPtr<USoundBase> WalkOnWood;

	UPROPERTY()
	USoundBase* WalkOnWoodLoaded;

	UPROPERTY(EditDefaultsOnly, Category = "Default", meta = (AllowPrivateAccess = true))
	TSoftObjectPtr<USoundBase> RunOnDirt;

	UPROPERTY()
	USoundBase* RunOnDirtLoaded;

	UPROPERTY(EditDefaultsOnly, Category = "Default", meta = (AllowPrivateAccess = true))
	TSoftObjectPtr<USoundBase> RunOnStone;

	UPROPERTY()
	USoundBase* RunOnStoneLoaded;

	UPROPERTY(EditDefaultsOnly, Category = "Default", meta = (AllowPrivateAccess = true))
	TSoftObjectPtr<USoundBase> RunOnWood;

	UPROPERTY()
	USoundBase* RunOnWoodLoaded;

	UPROPERTY(BlueprintReadOnly, meta = (AllowPrivateAccess = true))
	uint8 bIsFalling : 1;

	UPROPERTY(BlueprintReadOnly, meta = (AllowPrivateAccess = true))
	uint8 bIsCrouched : 1;

	UPROPERTY(BlueprintReadOnly, meta = (AllowPrivateAccess = true))
	uint8 bIsArmed : 1;

	UPROPERTY(BlueprintReadOnly, meta = (AllowPrivateAccess = true))
	uint8 bIsAiming : 1;

	UPROPERTY(BlueprintReadOnly, meta = (AllowPrivateAccess = true))
	float Speed = 0.0f;

	UPROPERTY(BlueprintReadOnly, meta = (AllowPrivateAccess = true))
	float Direction = 0.0f;

	UPROPERTY(BlueprintReadOnly, meta = (AllowPrivateAccess = true))
	float AimPitch = 0.0f;

public:
	UPROPERTY(BlueprintReadOnly)
	float RecoilAlpha = 0.0f;

	/** Recoil rotation intensity */
	UPROPERTY(BlueprintReadOnly)
	FRotator RecoilRotation = FRotator::ZeroRotator;

	float RecoilControlTime = 0.0f;

	UPROPERTY(BlueprintReadOnly)
	UBlendSpace* Movement;

	UPROPERTY(BlueprintReadOnly)
	UBlendSpace* MovementAim;

	UPROPERTY(BlueprintReadOnly)
	UBlendSpace* MovementCrouch;

	UPROPERTY(BlueprintReadOnly)
	UBlendSpace* MovementCrouchAim;

private:
	UPROPERTY(BlueprintReadOnly, meta = (AllowPrivateAccess = true))
	FVector LeftHandLocation = FVector::ZeroVector;
	
	UPROPERTY()
	class ABaseCharacter* CharacterOwner;
};
