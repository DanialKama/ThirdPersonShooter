
#pragma once

#include "CoreMinimal.h"
#include "Pickup.h"
// #include "Interfaces/WeaponInterface.h"
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

UCLASS()
class THIRDPERSONSHOOTER_API APickup_Weapon : public APickup, public ICommonInterface
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
	virtual void SetPickupStatus_Implementation(EPickupState PickupState) override;

	virtual void SetCanFire_Implementation(const bool bInCanFire) override;

	virtual void SetWeaponState_Implementation(EWeaponState WeaponState) override;

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

	// Variables
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Projectile")
	TArray<AEmptyShellActor*> EmptyShell;
	
	// Audio
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Effects|Audio")
	USoundBase* FleshImpactSound;
	
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Effects|Audio")
	USoundBase* WoodImpactSound;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Effects|Audio")
	USoundBase* MetalImpactSound;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Effects|Audio")
	USoundBase* StoneImpactSound;

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
	void WeaponFireEffect();

	UFUNCTION(BlueprintCallable, Category = "Weapon")
	void SpawnProjectile();
	
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

	UPROPERTY(EditDefaultsOnly, Category = "Private")
	uint8 bOwnerIsAI : 1;

	UPROPERTY(EditDefaultsOnly, Category = "Private")
	uint8 bCanFire : 1;

	UPROPERTY(BlueprintReadOnly, Category = "Private|References", meta=(AllowPrivateAccess=true))
	AAIController* OwnerAIController;

	UPROPERTY(BlueprintReadOnly, Category = "Private|References", meta=(AllowPrivateAccess=true))
	AController* OwnerController;

	FTimerHandle FireWeaponTimer;

	// Interfaces
	IAIControllerInterface* AIControllerInterface;
	IPlayerControllerInterface* PlayerControllerInterface;
};
