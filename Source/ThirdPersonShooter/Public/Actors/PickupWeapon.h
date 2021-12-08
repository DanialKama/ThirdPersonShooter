// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Pickup.h"
#include "Interfaces/PickupWeaponInterface.h"
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
class IPlayerControllerInterface;
class AEmptyShell;
class AProjectile;
class AMagazine;

USTRUCT(BlueprintType)
struct FWeaponDefaults
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Defaults", meta = (AllowPrivateAccess = "true"))
	FVector MuzzleFlashScale = FVector::OneVector;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Defaults", meta = (AllowPrivateAccess = "true"))
	FName MagazineBoneName = TEXT("None");

	// Recoil
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Defaults", meta = (AllowPrivateAccess = "true", ToolTip = "Smaller number = more intensity"))
	FRotator RotationIntensity = FRotator(0.0f, 0.0f, -5.0f);
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Defaults", meta = (AllowPrivateAccess = "true", ToolTip = "Bigger number = faster control"))
	float ControlTime = 0.25f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Defaults", meta = (AllowPrivateAccess = "true", ToolTip = "Bigger number = more fedback"))
	float CrosshairRecoil = 5.0f;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Defaults", meta = (AllowPrivateAccess = "true", ToolTip = "Smaller number = more fedback"))
	float ControllerPitch = -0.5f;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Defaults", meta = (AllowPrivateAccess = "true"))
	TSubclassOf<UCameraShakeBase> CameraShake;
	
	// Audio
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Defaults", meta = (AllowPrivateAccess = "true"))
	USoundCue* ReloadSound;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Defaults", meta = (AllowPrivateAccess = "true"))
	USoundCue* RaiseSound;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Defaults", meta = (AllowPrivateAccess = "true"))
	USoundCue* LowerSound;
	
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Defaults", meta = (AllowPrivateAccess = "true"))
	TArray<TSubclassOf<AProjectile>> Projectile;
	
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Defaults", meta = (AllowPrivateAccess = "true"))
	TArray<TSubclassOf<AEmptyShell>> EmptyShell;
	
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Defaults", meta = (AllowPrivateAccess = "true"))
	TSubclassOf<AMagazine> Magazine;
};

UCLASS()
class THIRDPERSONSHOOTER_API APickupWeapon : public APickup, public ICommonInterface, public IPickupWeaponInterface
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	APickupWeapon();
	
	//Components
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

	// Functions
	UFUNCTION(BlueprintCallable, Category = "PickupWeapon")
	void StartFireWeapon();

	UFUNCTION(BlueprintCallable, Category = "PickupWeapon")
	void StopFireWeapon();
	
	UFUNCTION(BlueprintCallable, Category = "PickupWeapon")
	void RaiseWeapon() const;

	UFUNCTION(BlueprintCallable, Category = "PickupWeapon")
	void LowerWeapon() const;

	UFUNCTION(BlueprintCallable, Category = "PickupWeapon")
	void SetMagazineVisibility(bool bVisible) const;

	UFUNCTION(BlueprintCallable, Category = "PickupWeapon")
	void ReloadWeapon() const;

	UFUNCTION(BlueprintCallable, Category = "PickupWeapon")
	bool CanPickupAmmo() const;

	UFUNCTION(BlueprintCallable, Category = "PickupWeapon")
	FVector GetLeftHandLocation() const;

	UFUNCTION(BlueprintCallable, Category = "PickupWeapon")
	FVector GetLeftHandAimLocation() const;
	
	// Interfaces
	virtual void SetPickupStatus_Implementation(const EPickupState PickupState) override;	// Pickup Interface, Call from character base
	virtual APickupWeapon* GetWeaponReference_Implementation() override;					// Weapon Interface, Call from character base
	virtual void SetCanFire_Implementation(const bool bInCanFire) override;					// Common Interface, Call from ammo component
	virtual void SetWeaponState_Implementation(const EWeaponState WeaponState) override;	// Common Interface, Call from ammo component
	
	// Variables
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Defaults")
	FWeaponInfo WeaponInfo;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Defaults", meta = (ToolTip = "use in line trace for player"))
	UCameraComponent* CameraComponent;
	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
private:
	// Functions
	UFUNCTION(BlueprintCallable, Category = "PickupWeapon")
	void FireWeapon();

	UFUNCTION(BlueprintCallable, Category = "PickupWeapon")
	void WeaponFireEffect() const;

	UFUNCTION(BlueprintCallable, Category = "PickupWeapon")
	void SpawnProjectile();

	UFUNCTION(BlueprintCallable, Category = "PickupWeapon")
	void ProjectileLineTrace(FVector& OutLocation, FRotator& OutRotation);

	UFUNCTION(BlueprintCallable, Category = "PickupWeapon")
	void CalculateLineTrace(FVector& OutStart, FVector& OutEnd);

	UFUNCTION(BlueprintCallable, Category = "PickupWeapon", meta = (ToolTip = "Use in line trace for bullet spread. Set Includes Negative to false if do not want negative numbers in output (mostly used for player character)"))
	FRotator RandomPointInCircle(float Radius, bool bIncludesNegative) const;
	
	void CoolDownDelay();
	void ResetAnimationDelay() const;
	
	// Overlaps
	UFUNCTION()
	void OnBoxBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
		UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void OnBoxEndOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
		UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	// Variables
	uint8 bDoOnceFire : 1, bOwnerIsAI : 1, bCanFire : 1, bCharacterInterface : 1, bPlayerControllerInterface : 1, bAIControllerInterface : 1;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Defaults", meta = (AllowPrivateAccess = "true"))
	FWeaponDefaults WeaponDefaults;

	UPROPERTY(EditDefaultsOnly, Category = "Defaults")
	uint8 bDrawDebugLineTrace : 1;
	
	UPROPERTY(BlueprintReadOnly, Category = "Defaults", meta = (AllowPrivateAccess = "true"))
	AProjectile* CurrentProjectile;

	FTimerHandle FireWeaponTimer;

	UPROPERTY(BlueprintReadOnly, Category = "Defaults", meta = (AllowPrivateAccess = "true"))
	TArray<AActor*> IgnoredActorsByTrace;

	UPROPERTY(BlueprintReadOnly, Category = "Defaults", meta = (AllowPrivateAccess = "true"))
	AController* OwnerController;
};