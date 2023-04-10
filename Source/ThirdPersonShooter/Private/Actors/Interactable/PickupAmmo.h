// Copyright 2022-2023 Danial Kamali. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Pickup.h"
#include "PickupAmmo.generated.h"

UCLASS(meta = (DisplayName = "Pick Up Ammo"))
class APickupAmmo : public APickup
{
	GENERATED_BODY()

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = true))
	TObjectPtr<UStaticMeshComponent> StaticMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = true))
	TObjectPtr<class USphereComponent> SphereCollision;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = true))
	TObjectPtr<class UWidgetComponent> Widget;

// Functions
public:
	APickupAmmo();

	virtual void SetPickUpState(const EPickupState PickupState) override;

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
	UPROPERTY(EditDefaultsOnly, Category = "Default", meta = (Bitmask, BitmaskEnum = EAmmoType))
	int32 AmmoType = 0;
	
	UPROPERTY(EditDefaultsOnly, Category = "Default")
	int32 Amount = 1;
	
private:
	UPROPERTY(EditDefaultsOnly, Category = "Default", meta = (AllowPrivateAccess = true))
	TObjectPtr<USoundBase> PickupSound;
};
