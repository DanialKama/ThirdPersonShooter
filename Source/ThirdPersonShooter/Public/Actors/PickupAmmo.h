// All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Actors/Pickup.h"
#include "PickupAmmo.generated.h"

class USoundCue;
class USphereComponent;
class UWidgetComponent;

UCLASS()
class THIRDPERSONSHOOTER_API APickupAmmo : public APickup
{
	GENERATED_BODY()

public:
	APickupAmmo();
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UStaticMeshComponent* StaticMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	USphereComponent* SphereCollision;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UWidgetComponent* Widget;

	virtual void SetPickupStatus(EPickupState PickupState) override;

	/** Bitmask / Bitflag Enum */
	UPROPERTY(EditDefaultsOnly, Category = "Defaults", meta = (Bitmask, BitmaskEnum = "EAmmoType"))
	int32 AmmoType = static_cast<int32>(EAmmoType::None);
	
	UPROPERTY(EditDefaultsOnly, Category = "Defaults")
	int32 Amount = 1;

protected:
	virtual void BeginPlay() override;
	
private:
	UFUNCTION()
	void OnBoxBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	
	UFUNCTION()
	void OnBoxEndOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	UPROPERTY(EditDefaultsOnly, Category = "Defaults", meta = (AllowPrivateAccess = "true"))
	USoundCue* PickupSound;
};