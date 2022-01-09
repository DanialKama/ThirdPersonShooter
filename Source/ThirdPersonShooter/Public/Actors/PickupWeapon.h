// All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Pickup.h"
#include "Interfaces/CommonInterface.h"
#include "Structs/WeaponInfoStruct.h"
#include "PickupWeapon.generated.h"

class UBoxComponent;
class UWidgetComponent;
class USoundCue;
class UCameraComponent;
class UAmmoComponent;
class AAIController;
class IAIControllerInterface;
class AEmptyShell;
class AProjectile;
class AMagazine;

USTRUCT(BlueprintType)
struct FWeaponDefaults
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly)
	FVector MuzzleFlashScale = FVector::OneVector;
	
	UPROPERTY(EditDefaultsOnly)
	FName MagazineBoneName = FName("None");
	
	UPROPERTY(EditDefaultsOnly, meta = (ToolTip = "Smaller number = more intensity"))
	FRotator RotationIntensity = FRotator(0.0f, 0.0f, -5.0f);
	
	UPROPERTY(EditDefaultsOnly, meta = (ToolTip = "Bigger number = faster control"))
	float ControlTime = 0.25f;
	
	UPROPERTY(EditDefaultsOnly, meta = (ToolTip = "Bigger number = more fedback"))
	float CrosshairRecoil = 5.0f;
	
	UPROPERTY(EditDefaultsOnly, meta = (ToolTip = "Smaller number = more fedback"))
	float ControllerPitch = -0.5f;
	
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UCameraShakeBase> CameraShake;
	
	UPROPERTY(EditDefaultsOnly)
	USoundCue* ReloadSound = nullptr;
	
	UPROPERTY(EditDefaultsOnly)
	USoundCue* RaiseSound = nullptr;
	
	UPROPERTY(EditDefaultsOnly)
	USoundCue* LowerSound = nullptr;
	
	UPROPERTY(EditDefaultsOnly)
	TArray<TSubclassOf<AProjectile>> Projectile;
	
	UPROPERTY(EditDefaultsOnly)
	TArray<TSubclassOf<AEmptyShell>> EmptyShell;
	
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<AMagazine> Magazine;
};

UCLASS()
class THIRDPERSONSHOOTER_API APickupWeapon : public APickup, public ICommonInterface
{
	GENERATED_BODY()

public:
	APickupWeapon();
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	USkeletalMeshComponent* SkeletalMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UBoxComponent* BoxCollision;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UParticleSystemComponent* MuzzleFlash;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UAudioComponent* FireSound;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UWidgetComponent* Widget;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UAmmoComponent* AmmoComponent;

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
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Defaults")
	FWeaponInfo WeaponInfo;
	
	UPROPERTY(EditDefaultsOnly, Category = "Defaults")
	FWeaponDefaults WeaponDefaults;
	
	/** use for line trace if the owner is player */
	UPROPERTY()
	UCameraComponent* CameraComponent;
	
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

	uint8 bDoOnceFire : 1, bOwnerIsAI : 1, bCanFire : 1, bCharacterInterface : 1, bPlayerControllerInterface : 1, bAIControllerInterface : 1;

	UPROPERTY(EditDefaultsOnly, Category = "Defaults", meta = (AllowPrivateAccess = true))
	uint8 bDrawDebugLineTrace : 1;

	UPROPERTY()
	AProjectile* CurrentProjectile;
	
	UPROPERTY()
	TArray<AActor*> IgnoredActorsByTrace;
	
	UPROPERTY()
	AController* OwnerController;
	
	UPROPERTY()
	AAIController* AIController;

	FTimerHandle FireWeaponTimer;
};