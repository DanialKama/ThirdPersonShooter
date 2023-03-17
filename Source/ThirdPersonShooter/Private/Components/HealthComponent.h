// Copyright 2022-2023 Danial Kamali. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "BaseComponent.h"
#include "HealthComponent.generated.h"

UCLASS()
class UHealthComponent : public UBaseComponent
{
	GENERATED_BODY()

// Functions
public:	
	UHealthComponent();

	virtual void Initialize() override;
	
	void Healing(float HealthDifference);

	/** Only start when stamina is full */
	UFUNCTION()
	void StartHealthRecovery();
	
	/** Stop when stamina is not full */
	void StopHealthRecovery();

private:
	UFUNCTION()
	void TakeAnyDamage(AActor* DamagedActor, float Damage, const UDamageType* DamageType, AController* InstigatedBy, AActor* DamageCauser);
	
	UFUNCTION()
	void TakePointDamage(AActor* DamagedActor, float Damage, AController* InstigatedBy, FVector HitLocation, UPrimitiveComponent* FHitComponent, FName BoneName, FVector ShotFromDirection, const UDamageType* DamageType, AActor* DamageCauser);

	UFUNCTION()
	void TakeRadialDamage(AActor* DamagedActor, float Damage, const UDamageType* DamageType, FVector Origin, FHitResult HitInfo, AController* InstigatedBy, AActor* DamageCauser);

	void UpdateHealthOnDamage(float Damage, FName BoneName, FVector ShotFromDirection);

	UFUNCTION()
	void RecoverHealth();

// Variables
public:
	UPROPERTY(EditAnywhere, Category = "Defaults")
	uint8 bCanRecoverHealth : 1;
	
	UPROPERTY(EditDefaultsOnly, Category = "Defaults", meta = (ClampMin = "0.0", UIMin = "0.0"))
	float MaxHealth;
	
	UPROPERTY(EditDefaultsOnly, Category = "Defaults", meta = (ClampMin = "0.0", UIMin = "0.0"))
	float DefaultHealth;
	
	FName HitBoneName;
	
	/** Direction of the shot */
	FVector ShotOrigin;
	
private:
	UPROPERTY(EditDefaultsOnly, Category = "Defaults", meta = (ToolTip = "If health goes lower than this value health component start notifying the owner", ClampMin = "0.0", UIMin = "0.0", AllowPrivateAccess = true))
	float LowHealth;
	
	UPROPERTY(EditDefaultsOnly, Category = "Defaults", meta = (ClampMin = "0.0", UIMin = "0.0", AllowPrivateAccess = true))
	float HealingAmount;
	
	UPROPERTY(EditDefaultsOnly, Category = "Defaults", meta = (ToolTip = "Per Second", ClampMin = "0.0", UIMin = "0.0", AllowPrivateAccess = true))
	float HealthRecoveryRate;
	
	UPROPERTY(EditDefaultsOnly, Category = "Defaults", meta = (ClampMin = "0.0", UIMin = "0.0", AllowPrivateAccess = true))
	float StartHealthRecoveryDelay;
	
	uint8 bCommonInterface : 1, bCharacterInterface : 1;

	/** At begin play Current Health is equal to Default Health */
	float CurrentHealth;
	
	FTimerHandle HealthRecoveryTimer;
};