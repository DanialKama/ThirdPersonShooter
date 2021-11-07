
#pragma once

#include "CoreMinimal.h"
#include "Pickup.h"
// #include "Interfaces/WeaponInterface.h"
#include "Interfaces/CommonInterface.h"
#include "Pickup_Weapon.generated.h"

class UBoxComponent;
class UWidgetComponent;
class USoundCue;
class UCameraComponent;
class UAmmoComponent;
class AAIController;
class IAIControllerInterface;

UCLASS()
class THIRDPERSONSHOOTER_API APickup_Weapon : public APickup, public ICommonInterface
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	APickup_Weapon();

	// Functions
	UFUNCTION(BlueprintCallable, Category = "Public")
	void RaiseWeapon() const;

	UFUNCTION(BlueprintCallable, Category = "Public")
	void LowerWeapon() const;
	
	// Interfaces
	virtual void SetPickupStatus_Implementation(EPickupState PickupState) override;

	virtual void SetCanFire_Implementation(const bool bInCanFire) override;

	virtual void SetWeaponState_Implementation(EWeaponState WeaponState) override;

	// Variables
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
	// Audio
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Audio")
	USoundBase* FleshImpactSound;
	
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Audio")
	USoundBase* WoodImpactSound;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Audio")
	USoundBase* MetalImpactSound;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Audio")
	USoundBase* StoneImpactSound;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Audio")
	USoundBase* ReloadSound;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Audio")
	USoundCue* RaiseSound;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Audio")
	USoundBase* LowerSound;
	
private:
	// Functions
	UFUNCTION()
	void OnBoxBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
		UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void OnBoxEndOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
		UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	// Variables
	uint8 bDoOnce : 1;

	UPROPERTY(EditDefaultsOnly, Category = "Private")
	uint8 bOwnerIsAI : 1;

	UPROPERTY(EditDefaultsOnly, Category = "Private")
	uint8 bCanFire : 1;

	UPROPERTY(EditDefaultsOnly, Category = "Private|References")
	AAIController* OwnerAIController;

	UPROPERTY(EditDefaultsOnly, Category = "Private|References")
	AController* OwnerController;

	// Interfaces
	IAIControllerInterface* AIControllerInterface;
};
