// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Pickup.h"
#include "Interfaces/WeaponInterface.h"
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
struct FWeaponInfoNew
{
	GENERATED_BODY()
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Structs")
	EWeaponType WeaponType = EWeaponType::Pistol;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Structs")
	FString Name = TEXT("Weapon");

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Structs")
	float Range = 4000.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Structs")
	bool bIsAutomatic = false;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Structs")
	float TimeBetweenShots = 0.5f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Structs")
	float CoolDownTime = 0.5f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Structs", meta = (Bitmask, BitmaskEnum = "EAmmoType"))
	int32 AmmoType = static_cast<int32>(EAmmoType::None);

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Structs", meta = (ToolTip = "Min Fire Offset is only for AI"))
	float MinFireOffset = -10.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Structs", meta = (ToolTip = "Max Fire Offset is only for AI"))
	float MaxFireOffset = 10.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Structs")
	UCurveFloat* WeaponSpreadCurve = nullptr;
};

UCLASS()
class THIRDPERSONSHOOTER_API APickupWeapon : public APickup, public ICommonInterface, public IWeaponInterface
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	APickupWeapon();

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
	FWeaponInfo WeaponInfoOld;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Defaults")
	FWeaponInfoNew WeaponInfo;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Defaults", meta = (ToolTip = "use in line trace for player"))
	UCameraComponent* CameraComponent;
	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
private:
	//Components
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<USkeletalMeshComponent> SkeletalMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UBoxComponent> BoxCollision;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UParticleSystemComponent> MuzzleFlash;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UAudioComponent> FireSound;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UWidgetComponent> Widget;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UAmmoComponent> AmmoComponent;

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
	FVector MuzzleFlashScale = FVector::OneVector;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Defaults", meta = (AllowPrivateAccess = "true"))
	FName MagazineBoneName;

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

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Defaults", meta = (AllowPrivateAccess = "true"))
	uint8 bDrawDebugLineTrace : 1;
	
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Defaults", meta = (AllowPrivateAccess = "true"))
	TArray<TSubclassOf<AProjectile>> Projectile;
	
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Defaults", meta = (AllowPrivateAccess = "true"))
	TArray<TSubclassOf<AEmptyShell>> EmptyShell;
	
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Defaults", meta = (AllowPrivateAccess = "true"))
	TSubclassOf<AMagazine> Magazine;

	UPROPERTY(BlueprintReadOnly, Category = "Defaults", meta = (AllowPrivateAccess = "true"))
	AProjectile* CurrentProjectile;

	FTimerHandle FireWeaponTimer;

	UPROPERTY(BlueprintReadOnly, Category = "Defaults", meta = (AllowPrivateAccess = "true"))
	TArray<AActor*> IgnoredActorsByTrace;

	UPROPERTY(BlueprintReadOnly, Category = "Defaults", meta = (AllowPrivateAccess = "true"))
	AController* OwnerController;
};