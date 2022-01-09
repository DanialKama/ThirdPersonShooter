// All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Interfaces/CharacterInterface.h"
#include "Interfaces/CommonInterface.h"
#include "BaseCharacter.generated.h"

class UHealthComponent;
class UStaminaComponent;
class UAIPerceptionStimuliSourceComponent;
class APickup;
class APickupWeapon;
class APickupAmmo;
class AMagazine;
class UCameraComponent;
class UTimelineComponent;

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
struct FBodyParts
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly)
	USkeletalMesh* Head = nullptr;
	
	UPROPERTY(EditDefaultsOnly)
	USkeletalMesh* LegLeft = nullptr;
	
	UPROPERTY(EditDefaultsOnly)
	USkeletalMesh* LegRight = nullptr;
	
	UPROPERTY(EditDefaultsOnly)
	USkeletalMesh* ThighAndCalfLeft = nullptr;
	
	UPROPERTY(EditDefaultsOnly)
	USkeletalMesh* ThighAndCalfRight = nullptr;
	
	UPROPERTY(EditDefaultsOnly)
	USkeletalMesh* ThighLeft = nullptr;
	
	UPROPERTY(EditDefaultsOnly)
	USkeletalMesh* ThighRight = nullptr;
	
	UPROPERTY(EditDefaultsOnly)
	USkeletalMesh* CalfAndFootLeft = nullptr;
	
	UPROPERTY(EditDefaultsOnly)
	USkeletalMesh* CalfAndFootRight = nullptr;
	
	UPROPERTY(EditDefaultsOnly)
	USkeletalMesh* CalfLeft = nullptr;
	
	UPROPERTY(EditDefaultsOnly)
	USkeletalMesh* CalfRight = nullptr;
	
	UPROPERTY(EditDefaultsOnly)
	USkeletalMesh* FootLeft = nullptr;
	
	UPROPERTY(EditDefaultsOnly)
	USkeletalMesh* FootRight = nullptr;
	
	UPROPERTY(EditDefaultsOnly)
	USkeletalMesh* ArmLeft = nullptr;
	
	UPROPERTY(EditDefaultsOnly)
	USkeletalMesh* ArmRight = nullptr;
	
	UPROPERTY(EditDefaultsOnly)
	USkeletalMesh* UpperArmAndLowerArmLeft = nullptr;
	
	UPROPERTY(EditDefaultsOnly)
	USkeletalMesh* UpperArmAndLowerArmRight = nullptr;
	
	UPROPERTY(EditDefaultsOnly)
	USkeletalMesh* UpperArmLeft = nullptr;
	
	UPROPERTY(EditDefaultsOnly)
	USkeletalMesh* UpperArmRight = nullptr;
	
	UPROPERTY(EditDefaultsOnly)
	USkeletalMesh* LowerArmAndHandLeft = nullptr;
	
	UPROPERTY(EditDefaultsOnly)
	USkeletalMesh* LowerArmAndHandRight = nullptr;
	
	UPROPERTY(EditDefaultsOnly)
	USkeletalMesh* LowerArmLeft = nullptr;
	
	UPROPERTY(EditDefaultsOnly)
	USkeletalMesh* LowerArmRight = nullptr;
	
	UPROPERTY(EditDefaultsOnly)
	USkeletalMesh* HandLeft = nullptr;
	
	UPROPERTY(EditDefaultsOnly)
	USkeletalMesh* HandRight = nullptr;
};

UCLASS()
class THIRDPERSONSHOOTER_API ABaseCharacter : public ACharacter, public ICharacterInterface, public ICommonInterface
{
	GENERATED_BODY()
	
	// ReSharper disable once CppUE4ProbableMemoryIssuesWithUObject
	UTimelineComponent* DeathTimeline;
	
public:
	ABaseCharacter();
	
	virtual void Tick(float DeltaTime) override;
	virtual void Landed(const FHitResult& Hit) override;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UCapsuleComponent* FallCapsule;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UHealthComponent* HealthComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UStaminaComponent* StaminaComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UAIPerceptionStimuliSourceComponent* StimuliSource;

	void SetArmedState(bool bArmedState);
	bool SetAimState(bool bIsAiming);
	
	/** Call from Set Current Weapon and use in player character to exit aim mode */
	virtual void ResetAim();
	
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

	/** Call from anim notify */
	void SetReloadNotify(EReloadState ReloadState);
	
	/** Call from anim notify */
	void UpdateHolsterWeaponNotifyState(ENotifyState NotifyState);
	
	/** Call from anim notify */
	void UpdateGrabWeaponNotifyState(ENotifyState NotifyState);
	
	/** Call from anim notify and montage delegate*/
	UFUNCTION()
	virtual void HealingMontageHandler(UAnimMontage* AnimMontage, bool bInterrupted) const;
	
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
	virtual FGameplayTag GetTeamTag_Implementation() override;
	
	UPROPERTY(EditDefaultsOnly, Category = "Defaults")
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
	virtual void BeginPlay() override;
	
	void ToggleCrouch();
	virtual void SetCurrentWeapon(APickupWeapon* NewCurrentWeapon, EWeaponToDo WeaponSlot);
	virtual void ResetReload();
	virtual void SwitchIsEnded();
	
	UFUNCTION()
	virtual void StartDestroy();
	
	virtual void Destroyed() override;
	
	UPROPERTY()
	UAnimInstance* AnimInstance;
	
	UPROPERTY()
	UCameraComponent* ChildCameraComponent;

	uint8 bIsAlive : 1, bIsAimed : 1, bCharacterAnimationInterface : 1;
	EMovementState MovementState = EMovementState::Walk;
	EMovementState PreviousMovementState = EMovementState::Walk;

private:
	/** Use to perform two frame delay to cache the pose */
	UFUNCTION()
	void OneFrameDelay();
	
	void UpdateMovementState();
	void CharacterIsOnMove();
	void PickupWeapon(APickup* NewWeapon);
	void AddWeapon(APickupWeapon* WeaponToEquip, EWeaponToDo TargetSlot);
	
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

	/** Apply fall damage / Toggle ragdoll / Stand up the character */
	UFUNCTION()
	void OnFallCapsuleBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	
	uint8 bDoOnceStopped : 1, bDoOnceMoving : 1, bRagdollState : 1, bIsArmed : 1, bDoOnceReload : 1, bDoOnceDeath : 1, bCanReload : 1, bCanHolster : 1;

	UPROPERTY()
	TArray<UMaterialInstanceDynamic*> MaterialInstances;
	
	UPROPERTY()
	APickup* Pickup;
	
	UPROPERTY()
	AActor* Interactable;
	
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Defaults", meta = (ToolTip = "Lenght of this array should be equal to weapon types", AllowPrivateAccess = "true"))
	TArray<UAnimMontage*> ArmedIdleMontages;
	
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Defaults", meta = (AllowPrivateAccess = "true"))
	TArray<UAnimMontage*> IdleMontages;
	
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Defaults", meta = (ToolTip = "Lenght of this array should be equal to weapon types", AllowPrivateAccess = "true"))
	TArray<UAnimMontage*> StandUpReloadMontages;
	
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Defaults", meta = (ToolTip = "Lenght of this array should be equal to weapon types", AllowPrivateAccess = "true"))
	TArray<UAnimMontage*> CrouchReloadMontages;
	
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Defaults", meta = (ToolTip = "Lenght of this array should be equal to weapon types", AllowPrivateAccess = "true"))
	TArray<UAnimMontage*> ProneReloadMontages;
	
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Defaults", meta = (ToolTip = "Lenght of this array should be equal to weapon types", AllowPrivateAccess = "true"))
	TArray<UAnimMontage*> HolsterWeaponMontages;
	
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Defaults", meta = (ToolTip = "Lenght of this array should be equal to weapon types", AllowPrivateAccess = "true"))
	TArray<UAnimMontage*> GrabWeaponMontages;
	
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Defaults", meta = (AllowPrivateAccess = "true"))
	TArray<UAnimMontage*> StandUpFromFrontMontages;
	
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Defaults", meta = (AllowPrivateAccess = "true"))
	TArray<UAnimMontage*> StandUpFromBackMontages;
	
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Defaults", meta = (AllowPrivateAccess = "true"))
	TArray<UAnimMontage*> StandUpDeathMontages;
	
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Defaults", meta = (AllowPrivateAccess = "true"))
	TArray<UAnimMontage*> CrouchDeathMontages;
	
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Defaults", meta = (AllowPrivateAccess = "true"))
	TArray<UAnimMontage*> ProneDeathMontages;
	
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Defaults", meta = (ToolTip = "Bigger value = more damage apply to character when fall", ClampMin = "0.0", UIMin = "0.0", AllowPrivateAccess = "true"))
	float FallDamageMultiplier = 0.025;
	
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Defaults", meta = (ClampMin = "0.0", UIMin = "0.0", AllowPrivateAccess = "true"))
	float MinVelocityToApplyFallDamage = 1250.0f;
	
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Defaults", meta = (ToolTip = "After falling how long should wait until standing up", ClampMin = "0.0", UIMin = "0.0", AllowPrivateAccess = "true"))
	float StandingDelay = 2.5;
	
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Defaults", meta = (ToolTip = "After death how long take to destroy the character + 5 second dither", ClampMin = "0.0", UIMin = "0.0", AllowPrivateAccess = "true"))
	float DeathLifeSpan = 5.0f;
	
	UPROPERTY(EditDefaultsOnly, Category = "Defaults", meta = (AllowPrivateAccess = "true"))
	UCurveFloat* FadeFloatCurve;
	
	UPROPERTY(EditDefaultsOnly, Category = "Defaults", meta = (AllowPrivateAccess = "true"))
	FBodyParts BodyParts;
	
	EItemType PickupType = EItemType::Weapon;
	
	/** It sets when equipping a weapon and when changing the current weapon */
	EWeaponType WeaponType;
	
	FGameplayTagContainer CharacterTagContainer;
	FVector MeshLocation, MeshLocationOffset = FVector(0.0f, 0.0f, 90.0f);
	
	int32 PrimaryWeaponSupportedAmmo = static_cast<int32>(EAmmoType::None);
	int32 SecondaryWeaponSupportedAmmo = static_cast<int32>(EAmmoType::None);
	int32 SidearmWeaponSupportedAmmo = static_cast<int32>(EAmmoType::None);
	
	UPROPERTY()
	AMagazine* Magazine;
	
	UPROPERTY()
	APickupWeapon* GrabbedWeapon;
	
	EWeaponToDo WeaponToGrab;
	EWeaponType WeaponToHolsterType;
	EWeaponType WeaponToSwitchType;
	
	UPROPERTY()
	UAnimMontage* StandUpMontage;
	
	UPROPERTY()
	UAnimMontage* IdleMontage;
	
	/** Use for two frame delay */
	uint8 DelayedFrames = 0;
	
	FTimerHandle IdleTimer, CheckForFallingTimer;
};