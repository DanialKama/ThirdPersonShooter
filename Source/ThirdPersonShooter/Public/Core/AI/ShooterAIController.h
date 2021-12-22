// All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "Enums/PickupEnums.h"
#include "Enums/CharacterEnums.h"
#include "Interfaces/AIControllerInterface.h"
#include "Perception/AIPerceptionTypes.h"
#include "ShooterAIController.generated.h"

class UCustomAIPerceptionComponent;
class UAISenseConfig_Sight;
class UAISenseConfig_Damage;
class UAISenseConfig_Hearing;
class UAISenseConfig_Prediction;
class AAICharacter;
class APatrolPathActor;

UCLASS()
class THIRDPERSONSHOOTER_API AShooterAIController : public AAIController, public IAIControllerInterface
{
	GENERATED_BODY()
	
public:
	// Sets default values for this character's properties
	AShooterAIController();

	// Components
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UCustomAIPerceptionComponent* AIPerception;

	UPROPERTY()
	UAISenseConfig_Damage* AISense_Damage;
	UPROPERTY()
	UAISenseConfig_Sight* AISense_Sight;
	UPROPERTY()
	UAISenseConfig_Hearing* AISense_Hearing;
	UPROPERTY()
	UAISenseConfig_Prediction* AISense_Prediction;

	// Functions
	// Interfaces
	/** Get current weapon state and react accordingly */
	virtual void SetWeaponState_Implementation(FAmmoComponentInfo AmmoComponentInfo, EWeaponState NewWeaponState) override;
	virtual void SetAIState_Implementation(EAIState AIState) override;
	virtual void StartPatrol_Implementation() override;
	virtual AShooterAIController* GetAIControllerReference_Implementation() override;

	// Variables
	UPROPERTY(BlueprintReadOnly, Category = "Defaults")
	AAICharacter* ControlledPawn;
	
protected:
	virtual void BeginPlay() override;

private:
	// Functions
	UFUNCTION()
	void PerceptionUpdated(const TArray<AActor*>& UpdatedActors);
	UFUNCTION()
	void TargetPerceptionUpdated(AActor* Actor, FAIStimulus Stimulus);
	void DamageHandler(FAIStimulus Stimulus);
	void SightHandler(FAIStimulus Stimulus);
	void HearingHandler(FAIStimulus Stimulus);
	void PredictionHandler(FAIStimulus Stimulus);
	/** Gunfight */
	void Fight();
	void SwitchWeapon();
	void TryToReload(bool bNoAmmoLeftToReload);
	
	// Variables
	UPROPERTY(EditDefaultsOnly, Category = "Defaults", meta = (AllowPrivateAccess = true))
	UBehaviorTree* BehaviorTree;
	UPROPERTY()
	UBlackboardComponent* AIBlackboard;
	UPROPERTY()
	APatrolPathActor* PatrolPath;
	EWeaponState WeaponState;
};