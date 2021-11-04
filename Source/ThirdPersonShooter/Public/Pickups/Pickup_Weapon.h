

#pragma once

#include "CoreMinimal.h"
#include "Pickups/Pickup.h"
#include "Pickup_Weapon.generated.h"

class UBoxComponent;
class UWidgetComponent;

UCLASS()
class THIRDPERSONSHOOTER_API APickup_Weapon : public APickup
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	APickup_Weapon();

	// Interfaces
	virtual void SetPickupStatus_Implementation(EPickupState PickupState) override;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	//Components
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
	USkeletalMeshComponent* SkeletalMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
	UParticleSystemComponent* MuzzleFlash;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
	UAudioComponent* FireSound;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
	UBoxComponent* BoxCollision;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
	UWidgetComponent* Widget;

private:
	UFUNCTION()
	void OnBoxBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
		UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void OnBoxEndOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
		UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	// Variables
	uint8 bDoOnce : 1;
};
