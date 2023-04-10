// Copyright 2022-2023 Danial Kamali. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Core/Interfaces/CharacterInterface.h"
#include "Core/Interfaces/CommonInterface.h"
#include "BaseCharacter.generated.h"

class APickupWeapon;

enum class EReloadState : uint8
{
	StartReload,
	RemoveMag,
	DropMag,
	PickMag,
	InsertMag,
	EndReload
};

enum class ENotifyState : uint8
{
	Begin,
	End
};

USTRUCT(BlueprintType)
struct FMeleeInfo
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly)
	float KickDamage;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UDamageType> KickDamageType;

	// Default Constructor
	FMeleeInfo()
	{
		KickDamage = 75.0f;
		KickDamageType = nullptr;
	}
};

// TODO: Simplify it
USTRUCT(BlueprintType)
struct FBodyParts
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly)
	USkeletalMesh* Head;
	
	UPROPERTY(EditDefaultsOnly)
	USkeletalMesh* LegLeft;
	
	UPROPERTY(EditDefaultsOnly)
	USkeletalMesh* LegRight;
	
	UPROPERTY(EditDefaultsOnly)
	USkeletalMesh* ThighAndCalfLeft;
	
	UPROPERTY(EditDefaultsOnly)
	USkeletalMesh* ThighAndCalfRight;
	
	UPROPERTY(EditDefaultsOnly)
	USkeletalMesh* ThighLeft;
	
	UPROPERTY(EditDefaultsOnly)
	USkeletalMesh* ThighRight;
	
	UPROPERTY(EditDefaultsOnly)
	USkeletalMesh* CalfAndFootLeft;
	
	UPROPERTY(EditDefaultsOnly)
	USkeletalMesh* CalfAndFootRight;
	
	UPROPERTY(EditDefaultsOnly)
	USkeletalMesh* CalfLeft;
	
	UPROPERTY(EditDefaultsOnly)
	USkeletalMesh* CalfRight;
	
	UPROPERTY(EditDefaultsOnly)
	USkeletalMesh* FootLeft;
	
	UPROPERTY(EditDefaultsOnly)
	USkeletalMesh* FootRight;
	
	UPROPERTY(EditDefaultsOnly)
	USkeletalMesh* ArmLeft;
	
	UPROPERTY(EditDefaultsOnly)
	USkeletalMesh* ArmRight;
	
	UPROPERTY(EditDefaultsOnly)
	USkeletalMesh* UpperArmAndLowerArmLeft;
	
	UPROPERTY(EditDefaultsOnly)
	USkeletalMesh* UpperArmAndLowerArmRight;
	
	UPROPERTY(EditDefaultsOnly)
	USkeletalMesh* UpperArmLeft;
	
	UPROPERTY(EditDefaultsOnly)
	USkeletalMesh* UpperArmRight;
	
	UPROPERTY(EditDefaultsOnly)
	USkeletalMesh* LowerArmAndHandLeft;
	
	UPROPERTY(EditDefaultsOnly)
	USkeletalMesh* LowerArmAndHandRight;
	
	UPROPERTY(EditDefaultsOnly)
	USkeletalMesh* LowerArmLeft;
	
	UPROPERTY(EditDefaultsOnly)
	USkeletalMesh* LowerArmRight;
	
	UPROPERTY(EditDefaultsOnly)
	USkeletalMesh* HandLeft;
	
	UPROPERTY(EditDefaultsOnly)
	USkeletalMesh* HandRight;

	// Default Constructor
	FBodyParts()
	{
		Head = nullptr;
		LegLeft = nullptr;
		LegRight = nullptr;
		ThighAndCalfLeft = nullptr;
		ThighAndCalfRight = nullptr;
		ThighLeft = nullptr;
		ThighRight = nullptr;
		CalfAndFootLeft = nullptr;
		CalfAndFootRight = nullptr;
		CalfLeft = nullptr;
		CalfRight = nullptr;
		FootLeft = nullptr;
		FootRight = nullptr;
		ArmLeft = nullptr;
		ArmRight = nullptr;
		UpperArmAndLowerArmLeft = nullptr;
		UpperArmAndLowerArmRight = nullptr;
		UpperArmLeft = nullptr;
		UpperArmRight = nullptr;
		LowerArmAndHandLeft = nullptr;
		LowerArmAndHandRight = nullptr;
		LowerArmLeft = nullptr;
		LowerArmRight = nullptr;
		HandLeft = nullptr;
		HandRight = nullptr;
	}
};

/** Character base class */
UCLASS(Abstract, meta = (DisplayName = "Base Character"))
class ABaseCharacter : public ACharacter, public ICharacterInterface, public ICommonInterface
{
	GENERATED_BODY()
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = true))
	TObjectPtr<UCapsuleComponent> FallCapsule;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = true))
	TObjectPtr<UCapsuleComponent> KickCollision;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = true))
	TObjectPtr<class UHealthComponent> HealthComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = true))
	TObjectPtr<class UStaminaComponent> StaminaComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = true))
	TObjectPtr<class UAIPerceptionStimuliSourceComponent> StimuliSource;
	
	UPROPERTY()
	TObjectPtr<class UTimelineComponent> DeathTimeline;

// Functions
public:
	ABaseCharacter();

	void SetArmedState(bool bArmedState);
	bool SetAimState(bool bIsAiming);
	
	/** Call from Set Current Weapon and use in player character to exit aim mode */
	virtual void ResetAim() {}
	
	 /** Reload Weapon based on movement state and weapon type */
	virtual void ReloadWeapon();
	
	virtual void HolsterWeapon();
	virtual void SwitchToPrimary();
	virtual void SwitchToSecondary();
	virtual void SwitchToSidearm();

	UFUNCTION()
	void StartFireWeapon();
	
	void StopFireWeapon();
	void StartJump();
	void DropCurrentObject();
	
	void MeleeAttack();

	/** Call from anim notify */
	void SetReloadNotify(EReloadState ReloadState);
	
	/** Call from anim notify */
	void UpdateHolsterWeaponNotifyState(ENotifyState NotifyState);
	
	/** Call from anim notify */
	void UpdateGrabWeaponNotifyState(ENotifyState NotifyState);

	UFUNCTION()
	void MeleeMontageHandler(UAnimMontage* AnimMontage, bool bInterrupted);
	
	/** Call from anim notify and montage delegate*/
	UFUNCTION()
	virtual void HealingMontageHandler(UAnimMontage* AnimMontage, bool bInterrupted) const {}
	
	UFUNCTION()
	void StanUpMontageHandler(UAnimMontage* AnimMontage, bool bInterrupted) const;
	
	/** Call from anim notify, Death function, and montage delegate */
	UFUNCTION()
	void DeathMontageHandler(UAnimMontage* AnimMontage, bool bInterrupted);
	
	/** Health Recovery based on Stamina level */
	virtual void SetHealthState_Implementation(EHealthState HealthState) override;

	virtual void SetStaminaLevel_Implementation(float Stamina, bool bIsFull) override;
	virtual void SetMovementState_Implementation(EMovementState CurrentMovementState, bool bRelatedToCrouch, bool bRelatedToProne) override;
	virtual void Interact_Implementation() override;
	virtual void SetPickup_Implementation(EItemType NewPickupType, APickup* NewPickup) override;
	virtual EWeaponToDo CanPickupAmmo_Implementation(int32 AmmoType) override;
	virtual void AddRecoil_Implementation(FRotator RotationIntensity, float ControlTime, float CrosshairRecoil, float ControllerPitch) override;
	virtual FGameplayTag GetTeamTag_Implementation() override { return TeamTag; }

	FORCEINLINE UHealthComponent* GetHealthComponent() const { return HealthComponent; }
	
protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
	virtual void Landed(const FHitResult& Hit) override;
	virtual void Destroyed() override;
	
	void ToggleCrouch();
	virtual void SetCurrentWeapon(APickupWeapon* NewCurrentWeapon, EWeaponToDo WeaponSlot);
	virtual void ResetReload();
	virtual void SwitchIsEnded();
	
	UFUNCTION()
	virtual void StartDestroy();

private:
	/** Use to perform two frame delay to cache the pose */
	UFUNCTION()
	void OneFrameDelay();
	
	void UpdateMovementState();
	void CharacterIsOnMove();
	void PickupWeapon(APickup* NewWeapon);
	void AddWeapon(APickupWeapon* WeaponToEquip, EWeaponToDo TargetSlot);

	// TODO: Delete it, We don't need it anymore
	/** Replace the input weapon with a similar weapon
	 * to be persistence between level streaming.
	 * (all-important information will be transferred to the new weapon)
	 * Because when the player picks up a weapon from a streamed level
	 * and then that level unloads the weapon will be destroyed,
	 * so instead of just attaching the current weapon from the streamed level
	 * we spawn a new weapon that includes in persistent level and does not destroy on level unloads.
	 */
	APickupWeapon* SpawnAndReplaceWeapon(APickupWeapon* WeaponToSpawn);
	
	/** Attach to skeletal mesh */
	void AttachWeapon(APickupWeapon* WeaponToAttach, EWeaponToDo TargetWeapon) const;
	
	void DropWeapon(EWeaponToDo WeaponToDrop);
	void PickupAmmo(APickup* NewAmmo);
	void SpawnMagazine(const APickupWeapon* Weapon, bool bIsNew);
	void SwitchWeaponHandler(APickupWeapon* WeaponToSwitch, EWeaponToDo TargetWeapon, bool bSwitchWeapon);
	
	void DismembermentInitiate(FVector ShotOrigin, FName HitBone);
	USkeletalMeshComponent* DismembermentLeftLeg(FName HitBone);
	USkeletalMeshComponent* DismembermentRightLeg(FName HitBone);
	USkeletalMeshComponent* DismembermentLeftHand(FName HitBone);
	USkeletalMeshComponent* DismembermentRightHand(FName HitBone);
	
	/** Add skeletal mesh component */
	USkeletalMeshComponent* SpawnBodyPart(FName Name, USkeletalMesh* SkeletalMesh);

	void ToggleRagdoll(bool bStart);
	void Death();

	UFUNCTION()
	void StartDeathLifeSpan();
	
	UFUNCTION()
	void DeathTimeLineUpdate(float Value);
	
	UFUNCTION()
	void PlayIdleAnimation();
	
	/** After ragdoll, if the character is on the ground then it stands up */
	UFUNCTION()
	void CheckForFalling();
	
	/** Set getup orientation / Set anim montage to play on getup / Save pose snapshot to use in animation blueprint */
	void CachePose();
	
	/** If return is true character is facing up */
	bool CalculateFacingDirection() const;

	void SetGetupOrientation();
	void CalculateCapsuleLocation();

	UFUNCTION()
	void StandUp();
	
	UFUNCTION()
	void ReloadWeaponMontageHandler(UAnimMontage* AnimMontage, bool bInterrupted);
	
	UFUNCTION()
	void HolsterWeaponMontageHandler(UAnimMontage* AnimMontage, bool bInterrupted);

	UFUNCTION()
	void KickCollisionBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	
	/** Apply fall damage / Toggle ragdoll / Stand up the character */
	UFUNCTION()
	void FallCapsuleBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

// Variables
public:
	UPROPERTY(EditDefaultsOnly, Category = "Default")
	FGameplayTag TeamTag;
	
	UPROPERTY()
	APickupWeapon* PrimaryWeapon;
	
	UPROPERTY()
	APickupWeapon* SecondaryWeapon;
	
	UPROPERTY()
	APickupWeapon* SidearmWeapon;
	
	UPROPERTY()
	APickupWeapon* CurrentWeapon;
	
	EWeaponToDo CurrentHoldingWeapon = EWeaponToDo::NoWeapon;
	
protected:
	// TODO: Use the default function to access it every time
	UPROPERTY()
	UAnimInstance* AnimInstance;
	
	UPROPERTY()
	class UCameraComponent* ChildCameraComponent;

	uint8 bIsAlive : 1, bIsAimed : 1, bCharacterAnimationInterface : 1;

	// TODO: Use CMC to get these data
	EMovementState MovementState = EMovementState::Walk, PreviousMovementState = EMovementState::Walk;
	
private:
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Default", meta = (AllowPrivateAccess = true))
	TArray<TObjectPtr<UAnimMontage>> IdleMontages;

	// TODO: Play IdleMontages on the lower body when armed
	/** Length of this array should be equal to weapon types */
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Default", meta = (AllowPrivateAccess = true))
	TArray<TObjectPtr<UAnimMontage>> ArmedIdleMontages;

	// TODO: Store weapon related montages on the weapon actor
	/** Length of this array should be equal to weapon types */
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Default", meta = (AllowPrivateAccess = true))
	TArray<TObjectPtr<UAnimMontage>> StandUpReloadMontages;

	/** Length of this array should be equal to weapon types */
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Default", meta = (AllowPrivateAccess = true))
	TArray<TObjectPtr<UAnimMontage>> CrouchReloadMontages;

	/** Length of this array should be equal to weapon types */
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Default", meta = (AllowPrivateAccess = true))
	TArray<TObjectPtr<UAnimMontage>> ProneReloadMontages;

	/** Length of this array should be equal to weapon types */
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Default", meta = (AllowPrivateAccess = true))
	TArray<TObjectPtr<UAnimMontage>> HolsterWeaponMontages;

	/** Length of this array should be equal to weapon types*/
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Default", meta = (AllowPrivateAccess = true))
	TArray<TObjectPtr<UAnimMontage>> GrabWeaponMontages;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Default", meta = (AllowPrivateAccess = true))
	TArray<TObjectPtr<UAnimMontage>> MeleeAttackMontages;
	
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Default", meta = (AllowPrivateAccess = true))
	TArray<TSoftObjectPtr<UAnimMontage>> StandUpDeathMontages;
	
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Default", meta = (AllowPrivateAccess = true))
	TArray<TSoftObjectPtr<UAnimMontage>> CrouchDeathMontages;

	/** Bigger value = more damage apply to character when fall */
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Default", meta = (ClampMin = "0.0", UIMin = "0.0", AllowPrivateAccess = true))
	float FallDamageMultiplier = 0.025f;
	
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Default", meta = (ClampMin = "0.0", UIMin = "0.0", AllowPrivateAccess = true))
	float MinVelocityToApplyFallDamage = 1250.0f;

	/** After falling how long should wait until standing up */
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Default", meta = (ClampMin = "0.0", UIMin = "0.0", AllowPrivateAccess = true))
	float StandingDelay = 2.5f;

	/** After death how long take to destroy the character + 5 second dither */
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Default", meta = (ClampMin = "0.0", UIMin = "0.0", AllowPrivateAccess = true))
	float DeathLifeSpan = 5.0f;
	
	UPROPERTY(EditDefaultsOnly, Category = "Default", meta = (AllowPrivateAccess = true))
	TSoftObjectPtr<UCurveFloat> FadeFloatCurve;

	/** Melee attack info */
	UPROPERTY(EditDefaultsOnly, Category = "Default", meta = (AllowPrivateAccess = true))
	FMeleeInfo MeleeInfo;
	
	UPROPERTY(EditDefaultsOnly, Category = "Default", meta = (AllowPrivateAccess = true))
	FBodyParts BodyParts;

	uint8 bDoOnceStopped : 1, bDoOnceMoving : 1, bRagdollState : 1, bIsArmed : 1, bDoOnceReload : 1, bDoOnceDeath : 1, bCanReload : 1, bCanHolster : 1;

	UPROPERTY()
	TArray<UMaterialInstanceDynamic*> MaterialInstances;
	
	UPROPERTY()
	APickup* Pickup;
	
	UPROPERTY()
	AActor* Interactable;
	
	EItemType PickupType = EItemType::Weapon;
	
	/** It sets when equipping a weapon and when changing the current weapon */
	EWeaponType WeaponType;
	
	FGameplayTagContainer CharacterTagContainer;

	// TODO: Delete if useless
	FVector MeshLocation, MeshLocationOffset = FVector(0.0f, 0.0f, 90.0f);
	
	int32 PrimaryWeaponSupportedAmmo = 0;
	int32 SecondaryWeaponSupportedAmmo = 0;
	int32 SidearmWeaponSupportedAmmo = 0;
	
	UPROPERTY()
	class AMagazine* Magazine;
	
	UPROPERTY()
	APickupWeapon* GrabbedWeapon;
	
	EWeaponToDo WeaponToGrab = EWeaponToDo::NoWeapon;
	EWeaponType WeaponToHolsterType = EWeaponType::Rifle, WeaponToSwitchType = EWeaponType::Rifle;
	
	UPROPERTY()
	UAnimMontage* StandUpMontage;
	
	UPROPERTY()
	UAnimMontage* IdleMontage;
	
	/** Use for two frame delay */
	uint8 DelayedFrames = 0;
	
	FTimerHandle IdleTimer, CheckForFallingTimer;
};
