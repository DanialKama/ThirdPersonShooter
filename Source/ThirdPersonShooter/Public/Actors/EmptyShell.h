// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Sound/SoundCue.h"
#include "EmptyShell.generated.h"

UCLASS()
class THIRDPERSONSHOOTER_API AEmptyShell : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AEmptyShell();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Components
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
	UStaticMeshComponent* StaticMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
	UProjectileMovementComponent* ProjectileMovement;

	// Variables
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Defaults")
	USoundCue* HitSound;

private:
	// Functions
	UFUNCTION()
	void OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

	void StartPhysics() const;
	
	// Variables
	uint8 bDoOnce : 1;
};
