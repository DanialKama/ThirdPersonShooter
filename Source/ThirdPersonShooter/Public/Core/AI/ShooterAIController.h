// All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "Enums/PickupEnums.h"
#include "Enums/CharacterEnums.h"
#include "EnvironmentQuery/EnvQuery.h"
#include "Interfaces/AIControllerInterface.h"
#include "Perception/AIPerceptionTypes.h"
#include "ShooterAIController.generated.h"

class UBehaviorTreeComponent;
class UAIPerceptionComponent;
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
	
	// ReSharper disable once CppUE4ProbableMemoryIssuesWithUObject
	UAISenseConfig_Damage* AISense_Damage;
	// ReSharper disable once CppUE4ProbableMemoryIssuesWithUObject
	UAISenseConfig_Sight* AISense_Sight;
	// ReSharper disable once CppUE4ProbableMemoryIssuesWithUObject
	UAISenseConfig_Hearing* AISense_Hearing;
	// ReSharper disable once CppUE4ProbableMemoryIssuesWithUObject
	UAISenseConfig_Prediction* AISense_Prediction;
	
public:
	// Sets default values for this character's properties
	AShooterAIController();

	// Components
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UAIPerceptionComponent* AIPerception;

	// Functions
	void TryToUseWeapon();
	/** Get current weapon state and react accordingly */
	virtual void SetWeaponState_Implementation(FAmmoComponentInfo AmmoComponentInfo, EWeaponState NewWeaponState) override;
	virtual void SetAIState_Implementation(EAIState NewAIState) override;

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
	void HandleSight(AActor* UpdatedActor, FAIStimulus Stimulus);
	void HandleDamage(AActor* UpdatedActor, FAIStimulus Stimulus);
	void HandleHearing(AActor* UpdatedActor, FAIStimulus Stimulus);
	void HandlePrediction(FAIStimulus Stimulus) const;
	void HandleTeam(const AActor* UpdatedActor);
	/** Start or resume gunfight */
	void Fight();
	void SwitchWeapon();
	void TryToReload(bool bNoAmmoLeftToReload);
	/** Return nearest actor as actor object reference and distance to it */
	static float FindNearestOfTwoActor(AActor* Actor1, AActor* Actor2, FVector CurrentLocation, AActor* &CloserActor);
	/** Return to normal behavior */
	UFUNCTION()
	void BackToRoutine();
	void StartPatrolling();
	void HandleQueryResult(TSharedPtr<FEnvQueryResult> Result);
	UFUNCTION()
	void Surrender();
	
	// Variables
	uint8 bIsDisarm : 1, bHasPath : 1, bAICharacterInterface : 1, bDoOnceHelp : 1;
	UPROPERTY(EditDefaultsOnly, Category = "Defaults", meta = (AllowPrivateAccess = true))
	UBehaviorTree* BehaviorTree;
	UPROPERTY(EditDefaultsOnly, Category = "Defaults", meta = (AllowPrivateAccess = true))
	UEnvQuery* CanReachTarget;
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
	FTimerHandle BackToRoutineTimer;
};