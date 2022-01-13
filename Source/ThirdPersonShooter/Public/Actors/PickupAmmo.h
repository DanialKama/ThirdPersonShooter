// All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Actors/Pickup.h"
#include "PickupAmmo.generated.h"

UCLASS()
class THIRDPERSONSHOOTER_API APickupAmmo : public APickup
{
	GENERATED_BODY()

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = true))
	UStaticMeshComponent* StaticMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = true))
	class USphereComponent* SphereCollision;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = true))
	class UWidgetComponent* Widget;

// Functions
public:
	APickupAmmo();

	virtual void SetPickupStatus(EPickupState PickupState) override;

protected:
	virtual void BeginPlay() override;
	
private:
	UFUNCTION()
	void OnBoxBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	
	UFUNCTION()
	void OnBoxEndOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

// Variables
public:
	/** Bitmask / Bitflag Enum */
	UPROPERTY(EditDefaultsOnly, Category = "Defaults", meta = (Bitmask, BitmaskEnum = "EAmmoType"))
	int32 AmmoType;
	
	UPROPERTY(EditDefaultsOnly, Category = "Defaults")
	int32 Amount;
	
private:
	UPROPERTY(EditDefaultsOnly, Category = "Defaults", meta = (AllowPrivateAccess = "true"))
	class USoundCue* PickupSound;
};