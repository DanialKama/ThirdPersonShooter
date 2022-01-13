// All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "Enums/PickupEnums.h"
#include "Enums/CharacterEnums.h"
#include "Interfaces/AIControllerInterface.h"
#include "Perception/AIPerceptionTypes.h"
#include "ShooterAIController.generated.h"

UCLASS()
class THIRDPERSONSHOOTER_API AShooterAIController : public AAIController, public IAIControllerInterface
{
	GENERATED_BODY()

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = true))
	UAIPerceptionComponent* AIPerception;
	
	// ReSharper disable once CppUE4ProbableMemoryIssuesWithUObject
	class UAISenseConfig_Damage* AISense_Damage;
	// ReSharper disable once CppUE4ProbableMemoryIssuesWithUObject
	class UAISenseConfig_Sight* AISense_Sight;
	// ReSharper disable once CppUE4ProbableMemoryIssuesWithUObject
	class UAISenseConfig_Hearing* AISense_Hearing;
	// ReSharper disable once CppUE4ProbableMemoryIssuesWithUObject
	class UAISenseConfig_Prediction* AISense_Prediction;

// Functions
public:
	AShooterAIController();

	void TryToUseWeapon();
	
	virtual void SetAIState_Implementation(EAIState NewAIState) override;
	
	/** Get current weapon state and react accordingly */
	virtual void SetWeaponState_Implementation(FAmmoComponentInfo AmmoComponentInfo, EWeaponState NewWeaponState) override;
	
protected:
	virtual void BeginPlay() override;

private:
	virtual void OnPossess(APawn* InPawn) override;
	
	UFUNCTION()
	void PerceptionUpdated(const TArray<AActor*>& UpdatedActors);
	
	void HandleSight(AActor* UpdatedActor, FAIStimulus Stimulus);
	void HandleDamage(AActor* UpdatedActor, FAIStimulus Stimulus);
	void HandleHearing(FAIStimulus Stimulus);
	void HandlePrediction(FAIStimulus Stimulus) const;
	void HandleTeam(const AActor* UpdatedActor);
	
	/** Start or resume gunfight */
	void Fight();
	
	void SwitchWeapon();
	void TryToReload(bool bNoAmmoLeftToReload);
	
	/** Return nearest actor as actor object reference and distance to it */
	static float FindNearestOfTwoActor(AActor* Actor1, AActor* Actor2, FVector CurrentLocation, AActor* &CloserActor);

	UFUNCTION()
	void StartPatrolling() const;

	UFUNCTION()
	void AskForHelp() const;
	
	/** Return to normal behavior */
	UFUNCTION()
	void BackToRoutine();
	
	UFUNCTION()
	void Surrender();

// Variables
public:
	UPROPERTY()
	class AAICharacter* ControlledPawn;
	
private:
	uint8 bIsDisarm : 1, bHasPath : 1, bAICharacterInterface : 1, bDoOnceFight : 1, bDoOnceHelp : 1;
	
	UPROPERTY(EditDefaultsOnly, Category = "Defaults", meta = (AllowPrivateAccess = true))
	UBehaviorTree* BehaviorTree;
	
	UPROPERTY()
	class UBehaviorTreeComponent* BehaviorTreeComp;
	
	UPROPERTY()
	UBlackboardComponent* BlackboardComp;
	
	UPROPERTY()
	class APatrolPathActor* PatrolPath;
	
	EWeaponState WeaponState;
	
	/** Attacker is the current enemy that AI is fighting with */
	UPROPERTY()
	AActor* Attacker;
	
	EAIState AIState;
	
	FTimerHandle BackToRoutineTimer, AskForHelpTimer;
};