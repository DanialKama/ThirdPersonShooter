// Copyright 2022 Danial Kamali. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Pickup.h"
#include "Interfaces/CommonInterface.h"
#include "Structs/WeaponInfoStruct.h"
#include "PickupWeapon.generated.h"

class USoundCue;
class AEmptyShell;
class AProjectile;
class AMagazine;

USTRUCT(BlueprintType)
struct FWeaponDefaults
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly)
	FVector MuzzleFlashScale;
	
	UPROPERTY(EditDefaultsOnly)
	FName MagazineBoneName;
	
	UPROPERTY(EditDefaultsOnly, meta = (ToolTip = "Smaller number = more intensity"))
	FRotator RotationIntensity;
	
	UPROPERTY(EditDefaultsOnly, meta = (ToolTip = "Bigger number = faster control"))
	float ControlTime;
	
	UPROPERTY(EditDefaultsOnly, meta = (ToolTip = "Bigger number = more fedback"))
	float CrosshairRecoil;
	
	UPROPERTY(EditDefaultsOnly, meta = (ToolTip = "Smaller number = more fedback"))
	float ControllerPitch;
	
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UCameraShakeBase> CameraShake;
	
	UPROPERTY(EditDefaultsOnly)
	USoundCue* ReloadSound;
	
	UPROPERTY(EditDefaultsOnly)
	USoundCue* RaiseSound;
	
	UPROPERTY(EditDefaultsOnly)
	USoundCue* LowerSound;
	
	UPROPERTY(EditDefaultsOnly)
	TArray<TSubclassOf<AProjectile>> Projectile;
	
	UPROPERTY(EditDefaultsOnly)
	TArray<TSubclassOf<AEmptyShell>> EmptyShell;
	
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<AMagazine> Magazine;

	// Default constructor
	FWeaponDefaults()
	{
		MuzzleFlashScale = FVector::OneVector;
		MagazineBoneName = FName("None");
		RotationIntensity = FRotator(0.0f, 0.0f, -5.0f);
		ControlTime = 0.25f;
		CrosshairRecoil = 5.0f;
		ControllerPitch = -0.5f;
		ReloadSound = nullptr;
		RaiseSound = nullptr;
		LowerSound = nullptr;
	}
};

UCLASS()
class APickupWeapon : public APickup, public ICommonInterface
{
	GENERATED_BODY()

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = true))
	USkeletalMeshComponent* SkeletalMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = true))
	class UBoxComponent* BoxCollision;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = true))
	UParticleSystemComponent* MuzzleFlash;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = true))
	UAudioComponent* FireSound;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = true))
	class UWidgetComponent* Widget;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = true))
	class UAmmoComponent* AmmoComponent;

// Functions
public:
	APickupWeapon();

	/** Location use to adjust character left hand with IK in animation blueprint */
	UFUNCTION(BlueprintCallable, Category = "PickupWeapon")
	FVector GetLeftHandLocation() const;
	
	/**	Location use to adjust character left hand with IK in animation blueprint */
	UFUNCTION(BlueprintCallable, Category = "PickupWeapon")
	FVector GetLeftHandAimLocation() const;
	
	void StartFireWeapon();
	void StopFireWeapon();
	void RaiseWeapon();
	void LowerWeapon() const;
	void SetMagazineVisibility(bool bVisible) const;
	void ReloadWeapon() const;
	bool CanPickupAmmo() const;
	virtual void SetPickupStatus(const EPickupState PickupState) override;
	virtual void SetCanFire_Implementation(bool bInCanFire) override;
	virtual void SetWeaponState_Implementation(EWeaponState WeaponState) override;

	FORCEINLINE USkeletalMeshComponent* GetSkeletalMesh() const { return SkeletalMesh; }
	FORCEINLINE UAmmoComponent* GetAmmoComponent() const { return AmmoComponent; }
	
protected:
	virtual void BeginPlay() override;
	
private:
	UFUNCTION()
	void FireWeapon();
	
	/** Play weapon fire sound and muzzle emitter by activating them and playing weapon fire animation */
	void WeaponFireEffect() const;
	
	void SpawnProjectile();
	void ProjectileLineTrace(FVector& OutLocation, FRotator& OutRotation);
	
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
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Defaults")
	FWeaponInfo WeaponInfo;
	
	UPROPERTY(EditDefaultsOnly, Category = "Defaults")
	FWeaponDefaults WeaponDefaults;
	
	/** Use for line trace if the owner is player */
	UPROPERTY()
	class UCameraComponent* CameraComponent;
	
private:
	UPROPERTY(EditDefaultsOnly, Category = "Defaults", meta = (AllowPrivateAccess = true))
	uint8 bDrawDebugLineTrace : 1;
	
	uint8 bDoOnceFire : 1, bOwnerIsAI : 1, bCanFire : 1, bCharacterInterface : 1, bPlayerControllerInterface : 1, bAIControllerInterface : 1;

	UPROPERTY()
	AProjectile* CurrentProjectile;
	
	UPROPERTY()
	TArray<AActor*> IgnoredActorsByTrace;
	
	UPROPERTY()
	AController* OwnerController;
	
	UPROPERTY()
	class AAIController* AIController;

	EWeaponState CurrentWeaponState;

	FTimerHandle FireWeaponTimer;
};