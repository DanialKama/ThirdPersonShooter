// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Actors/Pickup.h"
#include "Interfaces/PickupAmmoInterface.h"
#include "PickupAmmo.generated.h"

class USoundCue;
class USphereComponent;
class UWidgetComponent;

UCLASS()
class THIRDPERSONSHOOTER_API APickupAmmo : public APickup, public IPickupAmmoInterface
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	APickupAmmo();

	// Interfaces
	// Without Output
	virtual void SetPickupStatus_Implementation(EPickupState PickupState) override;	// Pickup Interface, Call from character base
	virtual APickupAmmo* GetPickupAmmoReference_Implementation() override;			// Pickup Ammo Interface, Call from character base

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
private:
	//Components
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta=(AllowPrivateAccess = "true"))
	TObjectPtr<UStaticMeshComponent> StaticMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta=(AllowPrivateAccess = "true"))
	TObjectPtr<USphereComponent> SphereCollision;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta=(AllowPrivateAccess = "true"))
	TObjectPtr<UWidgetComponent> Widget;
	
	// Overlaps
	UFUNCTION()
	void OnBoxBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
		UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void OnBoxEndOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
		UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	// Variables
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Defaults", meta = (AllowPrivateAccess = "true", Bitmask, BitmaskEnum = "EAmmoType"))
	int32 AmmoType = static_cast<int32>(EAmmoType::None);

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Defaults", meta = (AllowPrivateAccess = "true"))
	int32 Amount = 1;

	// Audio
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Defaults", meta = (AllowPrivateAccess = "true"))
	USoundCue* PickupSound;
};