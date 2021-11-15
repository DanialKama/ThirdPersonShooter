
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

UCLASS()
class THIRDPERSONSHOOTER_API APickup_Weapon : public APickup, public ICommonInterface, public IWeaponInterface
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	APickup_Weapon();

	// Functions
	UFUNCTION(BlueprintCallable, Category = "Weapon")
	void StartFireWeapon();

	UFUNCTION(BlueprintCallable, Category = "Public")
	void StopFireWeapon();
	
	UFUNCTION(BlueprintCallable, Category = "Weapon")
	void RaiseWeapon() const;

	UFUNCTION(BlueprintCallable, Category = "Weapon")
	void LowerWeapon() const;
	
	// Interfaces
	// Without Output
	virtual void SetPickupStatus_Implementation(EPickupState PickupState) override;	// Pickup Interface, Call from character base
	virtual void SetCanFire_Implementation(const bool bInCanFire) override;			// Common Interface, Call from ammo component
	virtual void SetWeaponState_Implementation(EWeaponState WeaponState) override;	// Common Interface, Call from ammo component
	
	// Variables
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Public")
	FWeaponInfo WeaponInfo;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Public|References")
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

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UAmmoComponent* AmmoComponent;

	// Variable
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Effects|Emmiter")
	FVector MuzzleFlashScale = FVector::OneVector;
	
	// Audio
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Effects|Audio")
	USoundBase* ReloadSound;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Effects|Audio")
	USoundCue* RaiseSound;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Effects|Audio")
	USoundBase* LowerSound;

	//Camera shake
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Effects")
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

	UPROPERTY(BlueprintReadWrite, Category = "Private", meta=(AllowPrivateAccess=true))
	uint8 bDrawLineTraceDebug : 1;

	UPROPERTY(BlueprintReadOnly, Category = "Private", meta=(AllowPrivateAccess=true))
	uint8 bOwnerIsAI : 1;

	UPROPERTY(BlueprintReadOnly, Category = "Private", meta=(AllowPrivateAccess=true))
	uint8 bCanFire : 1;

	UPROPERTY(BlueprintReadOnly, Category = "Private|References", meta=(AllowPrivateAccess=true))
	AAIController* OwnerAIController;

	UPROPERTY(BlueprintReadOnly, Category = "Private|References", meta=(AllowPrivateAccess=true))
	AController* OwnerController;

	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "Projectile", meta = (AllowPrivateAccess = "true"))
	TArray<TSubclassOf<AProjectileActor>> Projectile;
	
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "Projectile", meta = (AllowPrivateAccess = "true"))
	TArray<TSubclassOf<AEmptyShellActor>> EmptyShell;

	UPROPERTY(BlueprintReadOnly, Category = "Projectile", meta = (AllowPrivateAccess = "true"))
	AProjectileActor* CurrentProjectile;

	FTimerHandle FireWeaponTimer;

	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "Private", meta = (AllowPrivateAccess = "True"))
	TArray<AActor*> IgnoredActorsByTrace;

	// Interfaces
	IAIControllerInterface* AIControllerInterface;
	IPlayerControllerInterface* PlayerControllerInterface;
};

// TODO check values and if not setting in blueprint then change BlueprintReadWrite to something else