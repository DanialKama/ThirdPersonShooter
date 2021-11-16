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
class AEmptyShellActor;
class AProjectileActor;
class AMagazineActor;

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
	void SetMagazineVisibility(bool Visible) const;

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
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Variables|Public")
	FWeaponInfo WeaponInfo;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Variables|Public", meta = (ToolTip = "use in line trace for player"))
	UCameraComponent* CameraComponent;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Variables|Public")
	TSubclassOf<AMagazineActor> MagazineActor;
	
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

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UAmmoComponent* AmmoComponent;

	// Variable
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Variables|Protected")
	FVector MuzzleFlashScale = FVector::OneVector;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Variables|Protected")
	FName MagazineBoneName;
	
	// Audio
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Variables|Protected")
	USoundBase* ReloadSound;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Variables|Protected")
	USoundCue* RaiseSound;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Variables|Protected")
	USoundBase* LowerSound;

	//Camera shake
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Variables|Protected")
	UCameraShakeBase* CameraShake;
	
private:
	// Functions
	UFUNCTION(BlueprintCallable, Category = "Weapon")
	void FireWeapon();

	UFUNCTION(BlueprintCallable, Category = "Weapon")
	void WeaponFireEffect() const;

	UFUNCTION(BlueprintCallable, Category = "Weapon")
	void SpawnProjectile();

	UFUNCTION(BlueprintCallable, Category = "Weapon")
	void ProjectileLineTrace(FVector& OutLocation, FRotator& OutRotation);

	UFUNCTION(BlueprintCallable, Category = "Weapon")
	void CalculateLineTrace(FVector& OutStart, FVector& OutEnd);

	UFUNCTION(BlueprintCallable, Category = "Weapon", meta = (ToolTip = "Use in line trace for bullet spread. Set Includes Negative to false if do not want negative numbers in output (Mostly used for player character)"))
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
	uint8 bDoOnceWidget : 1;
	uint8 bDoOnceFire : 1;

	UPROPERTY(BlueprintReadWrite, Category = "Variables|Private", meta=(AllowPrivateAccess=true))
	uint8 bDrawLineTraceDebug : 1;

	UPROPERTY(BlueprintReadOnly, Category = "Variables|Private", meta=(AllowPrivateAccess=true))
	uint8 bOwnerIsAI : 1;

	UPROPERTY(BlueprintReadOnly, Category = "Variables|Private", meta=(AllowPrivateAccess=true))
	uint8 bCanFire : 1;

	UPROPERTY(BlueprintReadOnly, Category = "Variables|Private", meta=(AllowPrivateAccess=true))
	AAIController* OwnerAIController;

	UPROPERTY(BlueprintReadOnly, Category = "Variables|Private", meta=(AllowPrivateAccess=true))
	AController* OwnerController;

	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "Variables|Private", meta = (AllowPrivateAccess = "true"))
	TArray<TSubclassOf<AProjectileActor>> Projectile;
	
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "Variables|Private", meta = (AllowPrivateAccess = "true"))
	TArray<TSubclassOf<AEmptyShellActor>> EmptyShell;

	UPROPERTY(BlueprintReadOnly, Category = "Variables|Private", meta = (AllowPrivateAccess = "true"))
	AProjectileActor* CurrentProjectile;

	FTimerHandle FireWeaponTimer;

	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "Variables|Private", meta = (AllowPrivateAccess = "True"))
	TArray<AActor*> IgnoredActorsByTrace;

	// Interfaces
	IAIControllerInterface* AIControllerInterface;
	IPlayerControllerInterface* PlayerControllerInterface;
};