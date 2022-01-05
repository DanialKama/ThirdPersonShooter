// All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Characters/BaseCharacter.h"
#include "Interfaces/AICharacterInterface.h"
#include "AICharacter.generated.h"

class AShooterAIController;
class UWidgetComponent;
class UBlackboardComponent;
class ARespawnActor;

UCLASS()
class THIRDPERSONSHOOTER_API AAICharacter : public ABaseCharacter, public IAICharacterInterface
{
	GENERATED_BODY()

public:
	AAICharacter();
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UWidgetComponent* Widget;

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

	UPROPERTY()
	ARespawnActor* RespawnHandler;
	
	UPROPERTY(EditDefaultsOnly, Category = "Defaults")
	UAnimMontage* SurrenderMontage;
	
protected:
	virtual void BeginPlay() override;
	
	virtual void SwitchIsEnded() override;
	virtual void HealingMontageHandler(UAnimMontage* AnimMontage, bool bInterrupted) const override;

private:
	virtual void PossessedBy(AController* NewController) override;
	void SetPrimaryWeapon();
	
	UFUNCTION()
	void SetSidearmWeapon();
	
	/** Show health widget if damage causer is the player and is not in the same team with AI */
	virtual float TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;

	UFUNCTION()
	void HideWidget() const;

	UFUNCTION()
	void TryToResetMovement() const;
	
	uint8 bAIControllerInterface : 1, bWidgetInterface : 1, bReloadGate : 1;
	
	UPROPERTY()
	AShooterAIController* AIController;
	
	UPROPERTY(EditInstanceOnly, Category = "Defaults")
	APatrolPathActor* PatrolPath;
	
	UPROPERTY(EditDefaultsOnly, Category = "Defaults", meta = (ToolTip = "Primary weapons to spawn and attach", AllowPrivateAccess = "true"))
	TArray<TSubclassOf<APickupWeapon>> PrimaryWeapons;
	
	UPROPERTY(EditDefaultsOnly, Category = "Defaults", meta = (ToolTip = "Sidearm weapons to spawn and attach", AllowPrivateAccess = "true"))
	TArray<TSubclassOf<APickupWeapon>> SidearmWeapons;
	
	UPROPERTY(EditDefaultsOnly, Category = "Defaults", meta = (AllowPrivateAccess = "true"))
	float RespawnTime = 5.0f;
	
	UPROPERTY()
	UBlackboardComponent* AIBlackboard;
	
	FTimerHandle WidgetTimerHandle;
};