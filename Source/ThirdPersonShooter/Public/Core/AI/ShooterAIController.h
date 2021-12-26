// All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "Enums/PickupEnums.h"
#include "Enums/CharacterEnums.h"
#include "Interfaces/AIControllerInterface.h"
#include "Perception/AIPerceptionTypes.h"
#include "ShooterAIController.generated.h"

class UBehaviorTreeComponent;
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
	
	UAISenseConfig_Damage* AISense_Damage;
	UAISenseConfig_Sight* AISense_Sight;
	UAISenseConfig_Hearing* AISense_Hearing;
	UAISenseConfig_Prediction* AISense_Prediction;
	
public:
	// Sets default values for this character's properties
	AShooterAIController();

	// Components
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UCustomAIPerceptionComponent* AIPerception;

	// Functions
	void StartPatrol();
	/*** inline getter | get the AI's blackboard */
	// FORCEINLINE UBlackboardComponent* GetBlackboardComponent() const { return BlackboardComp; }
	// Interfaces
	/** Get current weapon state and react accordingly */
	virtual void SetWeaponState_Implementation(FAmmoComponentInfo AmmoComponentInfo, EWeaponState NewWeaponState) override;
	virtual void SetAIState_Implementation(EAIState NewAIState) override;
	virtual AShooterAIController* GetAIControllerReference_Implementation() override;

	// Variables
	UPROPERTY(BlueprintReadOnly, Category = "Defaults")
	AAICharacter* ControlledPawn;
	
protected:
	virtual void BeginPlay() override;

private:
	// Functions
	virtual void OnPossess(APawn* InPawn) override;
	UFUNCTION()
	void PerceptionUpdated(const TArray<AActor*>& UpdatedActors);
	void SightHandler(AActor* UpdatedActor, FAIStimulus Stimulus);
	void DamageHandler(AActor* UpdatedActor, FAIStimulus Stimulus);
	void HearingHandler(FAIStimulus Stimulus);
	void PredictionHandler(FAIStimulus Stimulus);
	/** Gunfight */
	void Fight();
	void SwitchWeapon() const;
	void TryToReload(bool bNoAmmoLeftToReload) const;
	void TryToUseWeapon();
	/** Return nearest actor as actor object reference and distance to it */
	float FindNearestOfTwoActor(AActor* Actor1, AActor* Actor2, FVector CurrentLocation, AActor* &CloserActor);
	
	// Variables
	UPROPERTY(EditDefaultsOnly, Category = "Defaults", meta = (AllowPrivateAccess = true))
	UBehaviorTree* BehaviorTree;
	UPROPERTY()
	UBehaviorTreeComponent* BehaviorTreeComp;
	UPROPERTY()
	UBlackboardComponent* BlackboardComp;
	UPROPERTY()
	APatrolPathActor* PatrolPath;
	EWeaponState WeaponState;
	UPROPERTY()
	AActor* Attacker;
	EAIState AIState;
};