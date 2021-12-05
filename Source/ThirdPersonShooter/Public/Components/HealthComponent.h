// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "BaseComponent.h"
#include "HealthComponent.generated.h"

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class THIRDPERSONSHOOTER_API UHealthComponent : public UBaseComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UHealthComponent();

	// Variables
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Defaults")
	uint8 bCanRecoverHealth : 1;
	
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Defaults")
	float MaxHealth = 100.0f;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Defaults")
	float DefaultHealth = 100.0f;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Defaults", meta = (ToolTip = "If health goes lower than this value health component start notifying the owner"))
	float LowHealth = 25.0f;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Defaults")
	float HealingAmount = 10.0f;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Defaults", meta = (ToolTip = "Per Second"))
	float HealthRecoveryRate = 0.25f;
	
protected:
	virtual void SetupComponent() override;

private:
	// Functions
	void TakeAnyDamage(AActor* DamagedActor, float Damage, const class UDamageType* DamageType, class AController* InstigatedBy, AActor* DamageCauser);
	void TakePointDamage(AActor* DamagedActor, float Damage, class AController* InstigatedBy, FVector HitLocation, class UPrimitiveComponent* FHitComponent, FName BoneName, FVector ShotFromDirection, const class UDamageType* DamageType, AActor* DamageCauser);
	void TakeRadialDamage(AActor* DamagedActor, float Damage, const class UDamageType* DamageType, FVector Origin, FHitResult HitInfo, class AController* InstigatedBy, AActor* DamageCauser);

	// Variables
	float CurrentHealth = 0.0f;	// At begin play Current Health is equal to Default Health
	FName HitBoneName = TEXT("None");
	FVector ShotOrigin = FVector::ZeroVector;	// direction of the shot
	FTimerHandle HealthRecoveryTimer;
};