// Copyright 2022-2023 Danial Kamali. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Pickup.h"
#include "Core/Interfaces/CommonInterface.h"
#include "Core/Structures/WeaponInfo.h"
#include "PickupWeapon.generated.h"

class USoundCue;
class AEmptyShell;
class AProjectile;
class AMagazine;

// TODO: Do not use this structure
USTRUCT(BlueprintType)
struct FWeaponDefaults
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly)
	FVector MuzzleFlashScale;
	
	UPROPERTY(EditDefaultsOnly)
	FName MagazineBoneName;

	/** Smaller number = more intensity */
	UPROPERTY(EditDefaultsOnly)
	FRotator RotationIntensity;

	/** Bigger number = faster control */
	UPROPERTY(EditDefaultsOnly)
	float ControlTime;

	/** Bigger number = more feedback */
	UPROPERTY(EditDefaultsOnly)
	float CrosshairRecoil;

	/** Smaller number = more feedback */
	UPROPERTY(EditDefaultsOnly)
	float ControllerPitch;
	
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UCameraShakeBase> CameraShake;
	
	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<USoundBase> ReloadSound;
	
	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<USoundBase> RaiseSound;
	
	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<USoundBase> LowerSound;
	
	UPROPERTY(EditDefaultsOnly)
	TArray<TSubclassOf<AProjectile>> Projectile;
	
	UPROPERTY(EditDefaultsOnly)
	TArray<TSubclassOf<AEmptyShell>> EmptyShell;
	
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<AMagazine> Magazine;

	/** Default constructor */
	FWeaponDefaults()
		: MuzzleFlashScale(FVector::OneVector), MagazineBoneName(NAME_None), RotationIntensity(FRotator(0.0f, 0.0f, -5.0f)),
		ControlTime(0.25f), CrosshairRecoil(5.0f), ControllerPitch(-0.5f)
	{}
};

UCLASS(meta = (DisplayName = "Pick Up Weapon"))
class APickupWeapon : public APickup, public ICommonInterface
{
	GENERATED_BODY()

public:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = true))
	TObjectPtr<USkeletalMeshComponent> SkeletalMesh;

private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = true))
	TObjectPtr<class UBoxComponent> BoxCollision;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = true))
	TObjectPtr<UParticleSystemComponent> MuzzleFlash;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = true))
	TObjectPtr<UAudioComponent> FireSound;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = true))
	TObjectPtr<class UWidgetComponent> Widget;

public:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = true))
	TObjectPtr<class UAmmoComponent> AmmoComponent;

// Functions
public:
	APickupWeapon();
	
	void StartFireWeapon();
	void StopFireWeapon();
	void RaiseWeapon();
	void LowerWeapon() const;
	void SetMagazineVisibility(bool bVisible) const;
	void ReloadWeapon() const;
	bool CanPickupAmmo() const;
	virtual void SetPickUpState(const EPickupState PickupState) override;
	virtual void SetCanFire_Implementation(bool bInCanFire) override;
	virtual void SetWeaponState_Implementation(EWeaponState WeaponState) override;
	
protected:
	virtual void BeginPlay() override;
	
private:
	UFUNCTION()
	void FireWeapon();
	
	/** Play weapon fire sound and muzzle emitter by activating them and playing weapon fire animation */
	void WeaponFireEffect() const;
	
	void SpawnProjectile();
	void ProjectileLineTrace(FVector& OutLocation, FRotator& OutRotation) const;
	
	/** Calculate line trace start and end */
	void CalculateLineTrace(FVector& Start, FVector& End) const;
	
	/** Use in line trace for bullet spread
	 *	@param Radius Circle radius
	 *	@param bIncludesNegative Set Includes Negative to false if do not want negative numbers in output (mostly used for player character
	 */
	FRotator RandomPointInCircle(float Radius, bool bIncludesNegative) const;

	UFUNCTION()
	void CoolDownDelay();
	
	UFUNCTION()
	void ResetAnimationDelay() const;
	
	UFUNCTION()
	void OnBoxBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	
	UFUNCTION()
	void OnBoxEndOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

// Variables
public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Default")
	FWeaponInfo WeaponInfo;
	
	UPROPERTY(EditDefaultsOnly, Category = "Default")
	FWeaponDefaults WeaponDefaults;
	
	/** Use for line trace if the owner is player */
	UPROPERTY()
	class UCameraComponent* CameraComponent;

	/** The current socket that this weapon is assigned to */
	EWeaponToDo CurrentSocket = EWeaponToDo::NoWeapon;
	
private:
	UPROPERTY(EditDefaultsOnly, Category = "Default", meta = (AllowPrivateAccess = true))
	FVector MuzzleFlashScale = FVector::OneVector;
	
	UPROPERTY(EditDefaultsOnly, Category = "Default", meta = (AllowPrivateAccess = true))
	FName MagazineBoneName = NAME_None;

	/** Smaller number = more intensity */
	UPROPERTY(EditDefaultsOnly, Category = "Default", meta = (AllowPrivateAccess = true))
	FRotator RotationIntensity = FRotator(0.0f, 0.0f, -5.0f);

	/** Bigger number = faster control */
	UPROPERTY(EditDefaultsOnly, Category = "Default", meta = (AllowPrivateAccess = true))
	float ControlTime = 0.25f;

	/** Bigger number = more feedback */
	UPROPERTY(EditDefaultsOnly, Category = "Default", meta = (AllowPrivateAccess = true))
	float CrosshairRecoil = 5.0f;

	/** Smaller number = more feedback */
	UPROPERTY(EditDefaultsOnly, Category = "Default", meta = (AllowPrivateAccess = true))
	float ControllerPitch = -0.5f;
	
	UPROPERTY(EditDefaultsOnly, Category = "Default", meta = (AllowPrivateAccess = true))
	TObjectPtr<USoundBase> ReloadSound;
	
	UPROPERTY(EditDefaultsOnly, Category = "Default", meta = (AllowPrivateAccess = true))
	TObjectPtr<USoundBase> RaiseSound;
	
	UPROPERTY(EditDefaultsOnly, Category = "Default", meta = (AllowPrivateAccess = true))
	TObjectPtr<USoundBase> LowerSound;

	UPROPERTY(EditDefaultsOnly, Category = "Default", meta = (AllowPrivateAccess = true))
	TSubclassOf<UCameraShakeBase> CameraShake;
	
	UPROPERTY(EditDefaultsOnly, Category = "Default", meta = (AllowPrivateAccess = true))
	TArray<TSubclassOf<AProjectile>> Projectile;
	
	UPROPERTY(EditDefaultsOnly, Category = "Default", meta = (AllowPrivateAccess = true))
	TArray<TSubclassOf<AEmptyShell>> EmptyShell;
	
	UPROPERTY(EditDefaultsOnly, Category = "Default", meta = (AllowPrivateAccess = true))
	TSubclassOf<AMagazine> Magazine;

public:
	UPROPERTY(EditDefaultsOnly, Category = "Default|Animation", meta = (AllowPrivateAccess = true))
	UAnimMontage* IdleMontage;

	UPROPERTY(EditDefaultsOnly, Category = "Default|Animation", meta = (AllowPrivateAccess = true))
	UAnimMontage* StandUpReloadMontage;

	UPROPERTY(EditDefaultsOnly, Category = "Default|Animation", meta = (AllowPrivateAccess = true))
	UAnimMontage* CrouchReloadMontage;

	UPROPERTY(EditDefaultsOnly, Category = "Default|Animation", meta = (AllowPrivateAccess = true))
	UAnimMontage* HolsterWeaponMontage;

	UPROPERTY(EditDefaultsOnly, Category = "Default|Animation", meta = (AllowPrivateAccess = true))
	UAnimMontage* GrabWeaponMontage;
	
	UPROPERTY(EditDefaultsOnly, Category = "Default|Animation", meta = (AllowPrivateAccess = true))
	UBlendSpace* Movement;

	UPROPERTY(EditDefaultsOnly, Category = "Default|Animation", meta = (AllowPrivateAccess = true))
	UBlendSpace* MovementAim;

	UPROPERTY(EditDefaultsOnly, Category = "Default|Animation", meta = (AllowPrivateAccess = true))
	UBlendSpace* MovementCrouch;

	UPROPERTY(EditDefaultsOnly, Category = "Default|Animation", meta = (AllowPrivateAccess = true))
	UBlendSpace* MovementCrouchAim;

private:
	uint8 bDoOnceFire : 1, bOwnerIsAI : 1, bCanFire : 1, bCharacterInterface : 1, bPlayerControllerInterface : 1, bAIControllerInterface : 1;

	UPROPERTY()
	AProjectile* CurrentProjectile;
	
	UPROPERTY()
	TArray<AActor*> IgnoredActorsByTrace;
	
	UPROPERTY()
	AController* OwnerController;
	
	UPROPERTY()
	class AAIController* AIController;

	EWeaponState CurrentWeaponState = EWeaponState::Idle;

	FTimerHandle FireWeaponTimer;
};
