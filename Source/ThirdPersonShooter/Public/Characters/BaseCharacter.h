// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "GameFramework/Character.h"
#include "Interfaces/CharacterInterface.h"
#include "Interfaces/CommonInterface.h"
#include "BaseCharacter.generated.h"

class UPhysicsConstraintComponent;
class UHealthComponent;
class UStaminaComponent;
class APickup;
class APickupWeapon;
class APickupAmmo;

UCLASS()
class THIRDPERSONSHOOTER_API ABaseCharacter : public ACharacter, public ICharacterInterface, public ICommonInterface
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ABaseCharacter();
	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	
	// Components
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UCapsuleComponent* FallCapsule;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UStaticMeshComponent* Root1;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UStaticMeshComponent* Hinge1;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UPhysicsConstraintComponent* PhysicsConstraint1;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UStaticMeshComponent* Root2;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UStaticMeshComponent* Hinge2;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UPhysicsConstraintComponent* PhysicsConstraint2;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UStaticMeshComponent* Root3;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UStaticMeshComponent* Hinge3;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UPhysicsConstraintComponent* PhysicsConstraint3;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UHealthComponent* HealthComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UStaminaComponent* StaminaComponent;

	//Functions
	// Interfaces
	virtual void SetMovementState_Implementation(EMovementState InMovementState, bool bRelatedToCrouch, bool bRelatedToProne) override;
	virtual void Interact_Implementation() override;
	// Variables
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Defaults")
	uint8 bIsAlive : 1;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Defaults")
	FGameplayTag TeamTag;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Variables
	EMovementState MovementState = EMovementState::Walk;
	EMovementState PreviousMovementState = EMovementState::Walk;

private:
	// Functions
	void PlayIdleAnimation();
	void CharacterIsOnMove();
	void CalculateCapsuleLocation();
	void PickupWeapon(APickupWeapon* NewWeapon);
	void PickupAmmo(APickupAmmo* NewAmmo);
	// Overlaps
	UFUNCTION()
	void OnFallCapsuleBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
		UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	
	// Variables
	uint8 bDoOnceStopped : 1, bDoOnceMoving : 1, bCharacterAnimationInterface : 1, bRagdollState : 1;
	
	UPROPERTY()
	UAnimInstance* AnimInstance;
	UPROPERTY()
	TArray<UMaterialInstanceDynamic*> MaterialInstance;
	UPROPERTY()
	APickup* Pickup;

	EItemType ItemType = EItemType::Weapon;
	FGameplayTagContainer CharacterTagContainer;
	FVector MeshLocationOffset = FVector(0.0f, 0.0f, 90.0f), MeshLocation;
	FTimerHandle IdleTimer;
};
