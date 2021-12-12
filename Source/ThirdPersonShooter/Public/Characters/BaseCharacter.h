// All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "GameFramework/Character.h"
#include "Interfaces/CharacterInterface.h"
#include "Interfaces/CommonInterface.h"
#include "BaseCharacter.generated.h"

class UPhysicsConstraintComponent;
class UHealthComponent;
class UStaminaComponent;
class APickup;
class APickupWeapon;
class APickupAmmo;
class AMagazine;
class UCameraComponent;

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

UCLASS()
class THIRDPERSONSHOOTER_API ABaseCharacter : public ACharacter, public ICharacterInterface, public ICommonInterface
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ABaseCharacter();
	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	
	virtual void Landed(const FHitResult& Hit) override;
	
	// Components
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UCapsuleComponent* FallCapsule;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UStaticMeshComponent* Root1;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UStaticMeshComponent* Hinge1;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UPhysicsConstraintComponent* PhysicsConstraint1;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UStaticMeshComponent* Root2;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UStaticMeshComponent* Hinge2;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UPhysicsConstraintComponent* PhysicsConstraint2;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UHealthComponent* HealthComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UStaminaComponent* StaminaComponent;

	//Functions
	void SetArmedState(bool bArmedState);
	/** Call from Set Current Weapon and use in player character to exit aim mode */
	void ResetAim();
	void StartFireWeapon() const;
	void StopFireWeapon() const;
	/** Reload Weapon based on movement state and weapon type */
	void ReloadWeapon();
	void HolsterWeapon();
	void SwitchToPrimary();
	void SwitchToSecondary();
	void SwitchToSidearm();
	void DropItem();

	/** Call from anim notify */
	void SetReloadState(EReloadState ReloadState);
	/** Call from anim notify */
	void UpdateHolsterWeaponNotifyState(ENotifyState NotifyState);
	/** Call from anim notify */
	void UpdateGrabWeaponNotifyState(ENotifyState NotifyState);

	// Interfaces
	virtual void SetMovementState_Implementation(EMovementState CurrentMovementState, bool bRelatedToCrouch, bool bRelatedToProne) override;
	virtual void Interact_Implementation() override;
	virtual void SetPickup_Implementation(EItemType NewPickupType, APickup* NewPickup) override;
	/** Health Recovery based on Stamina level */
	virtual void SetStaminaLevel_Implementation(float Stamina, bool bIsFull) override;
	virtual void AddRecoil_Implementation(FRotator RotationIntensity, float ControlTime, float CrosshairRecoil, float ControllerPitch) override;
	virtual EWeaponToDo CanPickupAmmo_Implementation(int32 AmmoType) override;
	
	// Variables
	uint8 bIsAlive : 1, bIsAimed : 1;
	FGameplayTag TeamTag;
	UPROPERTY()
	APickupWeapon* PrimaryWeapon;
	UPROPERTY()
	APickupWeapon* SecondaryWeapon;
	UPROPERTY()
	APickupWeapon* SidearmWeapon;
	EWeaponToDo CurrentHoldingWeapon = EWeaponToDo::NoWeapon;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	//Functions
	void StartJump();
	void ToggleCrouch();
	void SetCurrentWeapon(APickupWeapon* NewCurrentWeapon, EWeaponToDo WeaponSlot);
	bool StartAiming();
	void StopAiming();

	// Variables
	EMovementState MovementState = EMovementState::Walk;
	EMovementState PreviousMovementState = EMovementState::Walk;
	UPROPERTY()
	APickupWeapon* CurrentWeapon;
	UPROPERTY()
	UCameraComponent* ChildCameraComponent;

private:
	// Functions
	void UpdateMovementState();
	void PickupWeapon(APickup* NewWeapon);
	void AddWeapon(APickupWeapon* WeaponToEquip, EWeaponToDo TargetSlot);
	
	/** Replace the input weapon with a similar weapon
	to be persistence between level streaming.
	(all-important information will be transferred to the new weapon)
	Because when the player picks up a weapon from a streamed level
	and then that level unloads the weapon will be destroyed,
	so instead of just attaching the current weapon from the streamed level
	we spawn a new weapon that includes in persistent level and does not destroy on level unloads.*/
	APickupWeapon* SpawnAndReplaceWeapon(APickupWeapon* WeaponToSpawn);
	void DropWeapon(EWeaponToDo WeaponToDrop);
	void PickupAmmo(APickup* NewAmmo);
	void SpawnMagazine(const APickupWeapon* Weapon);
	/** Override in child class */
	void ResetReload();
	void SwitchWeaponHandler(APickupWeapon* WeaponToSwitch, EWeaponToDo TargetWeapon, bool bSwitchWeapon);
	/** Override by AI character */
	virtual void SwitchIsEnded();
	void AttachToPhysicsConstraint(APickupWeapon* WeaponToAttach, EWeaponToDo TargetWeapon) const;

	void PlayIdleAnimation();
	void CharacterIsOnMove();
	void CheckForFalling();
	void CachePose();
	void CalculateCapsuleLocation();
	void ReloadWeaponMontageHandler(UAnimMontage* AnimMontage, bool bInterrupted);
	void HolsterWeaponMontageHandler(UAnimMontage* AnimMontage, bool bInterrupted);

	// Overlaps
	UFUNCTION()
	void OnFallCapsuleBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
		UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	
	// Variables
	uint8 bDoOnceStopped : 1, bDoOnceMoving : 1, bCharacterAnimationInterface : 1, bRagdollState : 1, bIsArmed : 1, bDoOnceReload : 1;
	
	UPROPERTY()
	UAnimInstance* AnimInstance;
	UPROPERTY()
	TArray<UMaterialInstanceDynamic*> MaterialInstance;
	UPROPERTY()
	APickup* Pickup;
	UPROPERTY()
	AActor* Interactable;
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Defaults", meta = (ToolTip = "Lenght of this array should be equal to weapon types", AllowPrivateAccess = "true"))
	TArray<UAnimMontage*> ArmedIdleAnimations;
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Defaults", meta = (AllowPrivateAccess = "true"))
	TArray<UAnimMontage*> IdleAnimations;
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Defaults", meta = (ToolTip = "Lenght of this array should be equal to weapon types", AllowPrivateAccess = "true"))
	TArray<UAnimMontage*> StandUpReloadAnimations;
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Defaults", meta = (ToolTip = "Lenght of this array should be equal to weapon types", AllowPrivateAccess = "true"))
	TArray<UAnimMontage*> CrouchReloadAnimations;
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Defaults", meta = (ToolTip = "Lenght of this array should be equal to weapon types", AllowPrivateAccess = "true"))
	TArray<UAnimMontage*> ProneReloadAnimations;
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Defaults", meta = (ToolTip = "Lenght of this array should be equal to weapon types", AllowPrivateAccess = "true"))
	TArray<UAnimMontage*> HolsterWeaponAnimations;
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Defaults", meta = (ToolTip = "Lenght of this array should be equal to weapon types", AllowPrivateAccess = "true"))
	TArray<UAnimMontage*> GrabWeaponAnimations;

	EItemType PickupType = EItemType::Weapon;
	EWeaponType WeaponType = EWeaponType::Pistol;	// Set when equip weapon and when change current weapon
	FGameplayTagContainer CharacterTagContainer;
	FVector MeshLocationOffset = FVector(0.0f, 0.0f, 90.0f), MeshLocation;
	FTimerHandle IdleTimer;
	int32 PrimaryWeaponSupportedAmmo = static_cast<int32>(EAmmoType::None);
	int32 SecondaryWeaponSupportedAmmo = static_cast<int32>(EAmmoType::None);
	int32 SidearmWeaponSupportedAmmo = static_cast<int32>(EAmmoType::None);
	UPROPERTY()
	AMagazine* Magazine;
	UPROPERTY()
	APickupWeapon* GrabbedWeapon;
	EWeaponToDo WeaponToGrab = EWeaponToDo::PrimaryWeapon;
	EWeaponType HolsterWeaponType;
};