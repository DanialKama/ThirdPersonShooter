// Copyright 2022-2023 Danial Kamali. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "BaseCharacter.h"
#include "AICharacter.generated.h"

UCLASS(meta = (DisplayName = "AI Character"))
class AAICharacter : public ABaseCharacter
{
	GENERATED_BODY()

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = true))
	TObjectPtr<class UWidgetComponent> Widget;

// Functions
public:
	AAICharacter();

	virtual void PossessedBy(AController* NewController) override;

	/** True to do it, false to stop it (to start fire weapon aim is necessary) */
	void UseWeapon(const bool bAim, const bool bFire);
	
	virtual void ReloadWeapon() override;
	virtual void ResetReload() override;
	virtual void SwitchToPrimary() override;
	virtual void SwitchToSecondary() override;
	virtual void SwitchToSidearm() override;
	virtual void HolsterWeapon() override;
	virtual void SetHealthLevel_Implementation(float Health) override;
	virtual void SetHealthState_Implementation(EHealthState HealthState) override;
	
protected:
	virtual void BeginPlay() override;

	virtual void SwitchIsEnded() override;
	virtual void HealingMontageHandler(UAnimMontage* AnimMontage, bool bInterrupted) const override;

private:
	void SetPrimaryWeapon();
	
	UFUNCTION()
	void SetSidearmWeapon();
	
	/** Show health widget if damage causer is the player and is not in the same team with AI */
	virtual float TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;

	UFUNCTION()
	void HideWidget() const;

	UFUNCTION()
	void TryToResetMovement() const;

// Variables
public:
	UPROPERTY(EditDefaultsOnly, Category = "Default")
	TObjectPtr<UAnimMontage> SurrenderMontage;

	UPROPERTY()
	class ASpawnerAI* Spawner;
	
private:
	/** Primary weapons to spawn and attach */
	UPROPERTY(EditDefaultsOnly, Category = "Default", meta = (AllowPrivateAccess = true))
	TArray<TSubclassOf<APickupWeapon>> PrimaryWeapons;

	/** Sidearm weapons to spawn and attach */
	UPROPERTY(EditDefaultsOnly, Category = "Default", meta = (AllowPrivateAccess = true))
	TArray<TSubclassOf<APickupWeapon>> SidearmWeapons;
	
	UPROPERTY(EditDefaultsOnly, Category = "Default", meta = (ClampMin = 0, UIMin = 0, AllowPrivateAccess = true))
	int32 RespawnDelay = 5;

	uint8 bAIControllerInterface : 1, bWidgetInterface : 1;
	
	UPROPERTY()
	class AShooterAIController* AIController;
	
	FTimerHandle WidgetTimerHandle;
};
