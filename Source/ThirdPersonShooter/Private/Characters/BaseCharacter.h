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

/** Character base class */
UCLASS(Abstract, meta = (DisplayName = "Base Character"))
class ABaseCharacter : public ACharacter, public ICharacterInterface, public ICommonInterface
{
	GENERATED_BODY()

public:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = true))
	TObjectPtr<class UHealthComponent> HealthComponent;

private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = true))
	TObjectPtr<class UStaminaComponent> StaminaComponent;

	UPROPERTY()
	TObjectPtr<class UAIPerceptionStimuliSourceComponent> StimuliSource;
	
	UPROPERTY()
	TObjectPtr<class UTimelineComponent> DeathTimeline;

// Functions
public:
	ABaseCharacter();

	virtual void Landed(const FHitResult& Hit) override;

	void SetMovementState(const EMovementState NewState);
	void SetMovementState(const float NewWalkSpeed, const float NewJumpZVelocity);
	
	void SetArmedState(const bool bArmedState);
	bool SetAimState(bool bNewState);
	
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
	void DropCurrentObject();
	
	void MeleeAttack();

	/** Call from anim notify */
	void SetReloadNotify(EReloadState ReloadState);
	
	/** Call from anim notify */
	void UpdateHolsterWeaponNotifyState(ENotifyState NotifyState);
	
	/** Call from anim notify */
	void UpdateGrabWeaponNotifyState(const ENotifyState NotifyState);

	UFUNCTION()
	void MeleeMontageHandler(UAnimMontage* AnimMontage, bool bInterrupted);
	
	/** Call from anim notify and montage delegate*/
	UFUNCTION()
	virtual void HealingMontageHandler(UAnimMontage* AnimMontage, bool bInterrupted) const {}
	
	/** Call from anim notify, Death function, and montage delegate */
	UFUNCTION()
	void DeathMontageHandler(UAnimMontage* AnimMontage, bool bInterrupted);
	
	/** Health Recovery based on Stamina level */
	virtual void SetHealthState_Implementation(EHealthState HealthState) override;

	virtual void SetStaminaLevel_Implementation(float Stamina, bool bIsFull) override;
	virtual void Interact_Implementation() override;
	virtual void SetPickup_Implementation(EItemType NewPickupType, APickup* NewPickup) override;
	virtual EWeaponToDo CanPickupAmmo_Implementation(int32 AmmoType) override;
	virtual void AddRecoil_Implementation(FRotator RotationIntensity, float ControlTime, float CrosshairRecoil, float ControllerPitch) override;
	virtual FGameplayTag GetTeamTag_Implementation() override { return TeamTag; }

protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
	virtual void Destroyed() override;
	
	virtual void SetCurrentWeapon(APickupWeapon* NewWeapon);
	virtual void ResetReload();
	virtual void SwitchIsEnded();
	
	UFUNCTION()
	virtual void StartDestroy();

private:
	void PickupWeapon(APickup* NewWeapon);
	void AddWeapon(APickupWeapon* WeaponToEquip, const EWeaponToDo TargetSlot);
	
	/** Attach to skeletal mesh */
	void AttachWeapon(APickupWeapon* WeaponToAttach, const EWeaponToDo TargetSocket) const;
	
	void DropWeapon(EWeaponToDo WeaponToDrop);
	void PickupAmmo(APickup* NewAmmo);
	void SpawnMagazine(const APickupWeapon* Weapon, bool bIsNew);
	void SwitchWeaponHandler(APickupWeapon* WeaponToSwitch, const EWeaponToDo TargetWeapon, const bool bSwitchWeapon);
	
	void DismembermentInitiate(FVector ShotOrigin, FName HitBone);
	USkeletalMeshComponent* DismembermentLeftLeg(FName HitBone);
	USkeletalMeshComponent* DismembermentRightLeg(FName HitBone);
	USkeletalMeshComponent* DismembermentLeftHand(FName HitBone);
	USkeletalMeshComponent* DismembermentRightHand(FName HitBone);
	
	/** Add skeletal mesh component */
	USkeletalMeshComponent* SpawnBodyPart(FName Name, USkeletalMesh* SkeletalMesh);

	void KillCharacter();

	UFUNCTION()
	void StartDeathLifeSpan();
	
	UFUNCTION()
	void DeathTimeLineUpdate(float Value);
	
	UFUNCTION()
	void PlayIdleAnimation();
	
	UFUNCTION()
	void ReloadWeaponMontageHandler(UAnimMontage* AnimMontage, bool bInterrupted);
	
	UFUNCTION()
	void HolsterWeaponMontageHandler(UAnimMontage* AnimMontage, bool bInterrupted);

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
	
protected:
	UPROPERTY()
	class UCameraComponent* ChildCameraComponent;

	uint8 bIsAlive : 1;
	
public:
	float WalkSpeed = 0.0f;
	float WalkJumpZVelocity = 0.0f;
	
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Default", meta = (AllowPrivateAccess = true, ClampMin = 0.0, UIMin = 0.0))
	float RunningSpeed = 300.0f;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Default", meta = (AllowPrivateAccess = true, ClampMin = 0.0, UIMin = 0.0))
	float RunningJumpZVelocity = 360.0f;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Default", meta = (AllowPrivateAccess = true, ClampMin = 0.0, UIMin = 0.0))
	float SprintSpeed = 600.0f;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Default", meta = (AllowPrivateAccess = true, ClampMin = 0.0, UIMin = 0.0))
	float SprintJumpZVelocity = 420.0f;

private:
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Default", meta = (AllowPrivateAccess = true))
	TArray<TObjectPtr<UAnimMontage>> IdleMontages;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Default", meta = (AllowPrivateAccess = true, ClampMin = 0.0, UIMin = 0.0))
	float MeleeDamage = 75.0f;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Default", meta = (AllowPrivateAccess = true))
	TArray<TObjectPtr<UAnimMontage>> MeleeAttackMontages;
	
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Default", meta = (AllowPrivateAccess = true))
	TArray<TSoftObjectPtr<UAnimMontage>> StandUpDeathMontages;
	
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Default", meta = (AllowPrivateAccess = true))
	TArray<TSoftObjectPtr<UAnimMontage>> CrouchDeathMontages;

	/** After death how long take to destroy the character + 5 second dither */
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Default", meta = (ClampMin = 0.0, UIMin = 0.0, AllowPrivateAccess = true))
	float DeathLifeSpan = 5.0f;
	
	UPROPERTY(EditDefaultsOnly, Category = "Default", meta = (AllowPrivateAccess = true))
	TSoftObjectPtr<UCurveFloat> FadeFloatCurve;

public:
	uint8 bIsAiming : 1;
	
private:
	uint8 bDoOnceStopped : 1, bDoOnceMoving : 1, bDoOnceReload : 1, bCanReload : 1, bCanHolster : 1;

	UPROPERTY()
	TArray<UMaterialInstanceDynamic*> MaterialInstances;
	
	UPROPERTY()
	APickup* Pickup;
	
	UPROPERTY()
	AActor* Interactable;
	
	EItemType PickupType = EItemType::Weapon;
	
	FGameplayTagContainer CharacterTagContainer;
	
	UPROPERTY()
	class AMagazine* Magazine;
	
	UPROPERTY()
	APickupWeapon* GrabbedWeapon;
	
	EWeaponToDo WeaponToGrab = EWeaponToDo::NoWeapon;
	EWeaponType WeaponToHolsterType = EWeaponType::Rifle, WeaponToSwitchType = EWeaponType::Rifle;
	
	FTimerHandle IdleTimer;
};
