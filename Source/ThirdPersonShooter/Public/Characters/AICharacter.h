// All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Characters/BaseCharacter.h"
#include "Interfaces/AICharacterInterface.h"
#include "AICharacter.generated.h"

class AAIController;
class UWidgetComponent;
class UBlackboardComponent;
class ARespawnActor;
class APatrolPathActor;

UCLASS()
class THIRDPERSONSHOOTER_API AAICharacter : public ABaseCharacter, public IAICharacterInterface
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AAICharacter();
	
	// Components
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UWidgetComponent* Widget;

	// Functions
	/** True to do it, false to stop it (to start fire weapon aim is necessary) */
	UFUNCTION(BlueprintCallable, Category = "AICharacter")
	void UseWeapon(bool bAim, bool bFire);
	virtual void ReloadWeapon() override;
	virtual void ResetReload() override;
	UFUNCTION(BlueprintCallable, Category = "AICharacter")
	bool SwitchToWeapon(bool SwitchToAvailable, EWeaponToDo WeaponToSwitch);
	virtual void SwitchToPrimary() override;
	virtual void SwitchToSecondary() override;
	virtual void SwitchToSidearm() override;
	virtual void HolsterWeapon() override;
	// Interfaces
	virtual void SetHealthLevel_Implementation(float Health) override;
	virtual void SetHealthState_Implementation(EHealthState HealthState) override;
	virtual AAICharacter* GetAICharacterReference_Implementation() override;
	virtual APatrolPathActor* GetPatrolPath_Implementation() override;

	// Variables
	UPROPERTY()
	ARespawnActor* RespawnHandler;
	UPROPERTY(EditDefaultsOnly, Category = "Defaults")
	UAnimMontage* SurrenderMontage;
	
protected:
	// Functions
	virtual void BeginPlay() override;
	virtual void SwitchIsEnded() override;

private:
	// Functions
	void SetPrimaryWeapon();
	UFUNCTION()
	void SetSidearmWeapon();
	/** Show health widget if damage causer is the player and is not in the same team with AI */
	virtual float TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;
	void HideWidget() const;
	void TryToResetMovement() const;
	
	// Variables
	uint8 bAIControllerInterface : 1, bWidgetInterface : 1, bReloadGate : 1;
	UPROPERTY()
	AAIController* AIController;
	UPROPERTY()
	UBlackboardComponent* ImplementedBlackboard;
	UPROPERTY(EditInstanceOnly, Category = "Defaults")
	APatrolPathActor* PatrolPath;
	UPROPERTY(EditDefaultsOnly, Category = "Defaults", meta = (ToolTip = "Primary weapons to spawn and attach", AllowPrivateAccess = "true"))
	TArray<TSubclassOf<APickupWeapon>> PrimaryWeapons;
	UPROPERTY(EditDefaultsOnly, Category = "Defaults", meta = (ToolTip = "Sidearm weapons to spawn and attach", AllowPrivateAccess = "true"))
	TArray<TSubclassOf<APickupWeapon>> SidearmWeapons;
	UPROPERTY(EditDefaultsOnly, Category = "Defaults", meta = (AllowPrivateAccess = "true"))
	float RespawnTime = 5.0f;
	FTimerHandle WidgetTimerHandle;
};