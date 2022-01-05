// All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "EmptyShell.generated.h"

class UProjectileMovementComponent;
class USoundCue;

UCLASS()
class THIRDPERSONSHOOTER_API AEmptyShell : public AActor
{
	GENERATED_BODY()
	
public:
	AEmptyShell();
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UStaticMeshComponent* StaticMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UProjectileMovementComponent* ProjectileMovement;

protected:
	virtual void BeginPlay() override;

private:
	UFUNCTION()
	void OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

	/** Set simulate physics to true */
	UFUNCTION()
	void StartPhysics() const;

	/** Play the sound of hitting the ground for once */
	uint8 bDoOnce : 1;
	
	UPROPERTY(EditDefaultsOnly, Category = "Defaults", meta = (AllowPrivateAccess = "true"))
	USoundCue* HitSound;
};