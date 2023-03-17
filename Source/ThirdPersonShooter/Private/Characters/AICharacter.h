// Copyright 2022-2023 Danial Kamali. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "BaseCharacter.h"
#include "Interfaces/AICharacterInterface.h"
#include "AICharacter.generated.h"

UCLASS()
class AAICharacter : public ABaseCharacter, public IAICharacterInterface
{
	GENERATED_BODY()

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = true))
	class UWidgetComponent* Widget;

// Functions
public:
	AAICharacter();

	/** True to do it, false to stop it (to start fire weapon aim is necessary) */
	void UseWeapon(bool bAim, bool bFire);
	
	virtual void ReloadWeapon() override;
	virtual void ResetReload() override;
	virtual void SwitchToPrimary() override;
	virtual void SwitchToSecondary() override;
	virtual void SwitchToSidearm() override;
	virtual void HolsterWeapon() override;
	virtual void SetHealthLevel_Implementation(float Health) override;
	virtual void SetHealthState_Implementation(EHealthState HealthState) override;
	virtual APatrolPathActor* GetPatrolPath_Implementation() override;
	
protected:
	virtual void BeginPlay() override;
	virtual void PossessedBy(AController* NewController) override;

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
	UPROPERTY(EditDefaultsOnly, Category = "Defaults")
	UAnimMontage* SurrenderMontage;

	UPROPERTY()
	class ARespawnActor* RespawnHandler;
	
private:
	UPROPERTY(EditInstanceOnly, Category = "Defaults")
	APatrolPathActor* PatrolPath;
	
	UPROPERTY(EditDefaultsOnly, Category = "Defaults", meta = (ToolTip = "Primary weapons to spawn and attach", AllowPrivateAccess = "true"))
	TArray<TSubclassOf<APickupWeapon>> PrimaryWeapons;
	
	UPROPERTY(EditDefaultsOnly, Category = "Defaults", meta = (ToolTip = "Sidearm weapons to spawn and attach", AllowPrivateAccess = "true"))
	TArray<TSubclassOf<APickupWeapon>> SidearmWeapons;
	
	UPROPERTY(EditDefaultsOnly, Category = "Defaults", meta = (AllowPrivateAccess = "true"))
	float RespawnTime;

	uint8 bAIControllerInterface : 1, bWidgetInterface : 1;
	
	UPROPERTY()
	class AShooterAIController* AIController;
	
	UPROPERTY()
	class UBlackboardComponent* AIBlackboard;
	
	FTimerHandle WidgetTimerHandle;
};