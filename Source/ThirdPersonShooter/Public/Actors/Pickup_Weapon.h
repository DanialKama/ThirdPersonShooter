// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Pickup.h"
#include "Interfaces/WeaponInterface.h"
#include "Interfaces/CommonInterface.h"
#include "Structs/WeaponInfoStruct.h"
#include "Pickup_Weapon.generated.h"

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

UCLASS()
class THIRDPERSONSHOOTER_API APickup_Weapon : public APickup, public ICommonInterface, public IWeaponInterface
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	APickup_Weapon();

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
	// Without Output
	virtual void SetPickupStatus_Implementation(EPickupState PickupState) override;	// Pickup Interface, Call from character base
	virtual void SetCanFire_Implementation(const bool bInCanFire) override;			// Common Interface, Call from ammo component
	virtual void SetWeaponState_Implementation(EWeaponState WeaponState) override;	// Common Interface, Call from ammo component
	// With Output
	virtual APickup_Weapon* GetWeaponReference_Implementation() override;			// Weapon Interface, Call from character base
	
	// Variables
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Defaults")
	FWeaponInfo WeaponInfo;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Defaults", meta = (ToolTip = "use in line trace for player"))
	UCameraComponent* CameraComponent;
	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	//Components
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
	USkeletalMeshComponent* SkeletalMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
	UBoxComponent* BoxCollision;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
	UParticleSystemComponent* MuzzleFlash;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
	UAudioComponent* FireSound;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
	UWidgetComponent* Widget;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
	UAmmoComponent* AmmoComponent;

	// Variable
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Defaults")
	FVector MuzzleFlashScale = FVector::OneVector;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Defaults")
	FName MagazineBoneName;
	
	// Audio
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Defaults")
	USoundCue* ReloadSound;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Defaults")
	USoundCue* RaiseSound;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Defaults")
	USoundCue* LowerSound;

	//Camera shake
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Defaults")
	TSubclassOf<UCameraShakeBase> CameraShake;
	
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

	UFUNCTION(BlueprintCallable, Category = "PickupWeapon", meta = (ToolTip = "Use in line trace for bullet spread. Set Includes Negative to false if do not want negative numbers in output (Mostly used for player character)"))
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
	uint8 bDoOnceFire : 1;
	uint8 bOwnerIsAI : 1;
	uint8 bCanFire : 1;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Defaults", meta = (AllowPrivateAccess = true))
	uint8 bDrawDebugLineTrace : 1;
	
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Defaults", meta = (AllowPrivateAccess = true))
	TArray<TSubclassOf<AProjectile>> Projectile;
	
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Defaults", meta = (AllowPrivateAccess = true))
	TArray<TSubclassOf<AEmptyShell>> EmptyShell;
	
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Defaults", meta = (AllowPrivateAccess = true))
	TSubclassOf<AMagazine> Magazine;

	UPROPERTY(BlueprintReadOnly, Category = "Defaults", meta = (AllowPrivateAccess = true))
	AProjectile* CurrentProjectile;

	FTimerHandle FireWeaponTimer;

	UPROPERTY(BlueprintReadOnly, Category = "Defaults", meta = (AllowPrivateAccess = True))
	TArray<AActor*> IgnoredActorsByTrace;

	UPROPERTY(BlueprintReadOnly, Category = "Defaults", meta = (AllowPrivateAccess = true))
	AAIController* OwnerAIController;

	UPROPERTY(BlueprintReadOnly, Category = "Defaults", meta = (AllowPrivateAccess = true))
	AController* OwnerController;
	
	// Interfaces
	IAIControllerInterface* AIControllerInterface;
	IPlayerControllerInterface* PlayerControllerInterface;
};