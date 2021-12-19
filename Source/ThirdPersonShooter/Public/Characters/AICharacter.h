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
	virtual void ReloadWeapon() override;
	// Interfaces
	/** True to do it, false to stop it (to start fire weapon aim is necessary) */
	virtual void UseWeapon_Implementation(bool bAim, bool bFire) override;
	virtual void SetHealthLevel_Implementation(float Health) override;
	virtual void SetHealthState_Implementation(EHealthState HealthState) override;

	// Variables
	UPROPERTY()
	ARespawnActor* RespawnHandler;
	
protected:
	virtual void BeginPlay() override;

private:
	// Functions
	void SetPrimaryWeapon();
	void SetSidearmWeapon();
	virtual void ResetReload() override;
	/** Show health widget if damage causer is the player and is not in the same team with AI */
	virtual float TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;
	void HideWidget() const;
	
	// Variables
	uint8 bAIControllerInterface : 1, bWidgetInterface : 1, bReloadGate : 1;
	UPROPERTY()
	AAIController* AIController;
	UPROPERTY()
	UBlackboardComponent* Blackboard;
	UPROPERTY(EditInstanceOnly, Category = "Defaults")
	AActor* PatrolPath;
	UPROPERTY(EditDefaultsOnly, Category = "Defaults", meta = (AllowPrivateAccess = "true"))
	float RespawnTime = 5.0f;
};