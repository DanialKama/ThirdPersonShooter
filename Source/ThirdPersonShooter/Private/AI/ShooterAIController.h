// Copyright 2022-2023 Danial Kamali. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "Core/Enumerations/PickupEnums.h"
#include "Core/Enumerations/CharacterEnums.h"
#include "Core/Interfaces/AIControllerInterface.h"
#include "Perception/AIPerceptionTypes.h"
#include "ShooterAIController.generated.h"

// TODO: Rename to AAIC_Shooter
UCLASS(meta = (DisplayName = "Shooter AI Controller"))
class AShooterAIController : public AAIController, public IAIControllerInterface
{
	GENERATED_BODY()

	// TODO: Use empty UPROPERTY()
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = true))
	TObjectPtr<UAIPerceptionComponent> AIPerception;

	UPROPERTY()
	TObjectPtr<class UAISenseConfig_Damage> AISense_Damage;
	
	UPROPERTY()
	TObjectPtr<class UAISenseConfig_Sight> AISense_Sight;
	
	UPROPERTY()
	TObjectPtr<class UAISenseConfig_Hearing> AISense_Hearing;

	UPROPERTY()
	TObjectPtr<class UAISenseConfig_Prediction> AISense_Prediction;

// Functions
public:
	AShooterAIController();
	
	void TryToUseWeapon();
	
	virtual void SetAIState_Implementation(EAIState NewAIState) override;
	
	/** Get current weapon state and react accordingly */
	virtual void SetWeaponState_Implementation(FAmmoComponentInfo AmmoComponentInfo, EWeaponState NewWeaponState) override;
	
protected:
	virtual void OnPossess(APawn* InPawn) override;
	virtual bool InitializeBlackboard(UBlackboardComponent& BlackboardComp, UBlackboardData& BlackboardAsset) override;
	virtual void Tick(float DeltaSeconds) override;

private:
	UFUNCTION()
	void PerceptionUpdated(const TArray<AActor*>& UpdatedActors);
	
	void HandleSight(AActor* UpdatedActor, const FAIStimulus& Stimulus);
	void HandleDamage(AActor* UpdatedActor, const FAIStimulus& Stimulus);
	void HandleHearing(const FAIStimulus& Stimulus);
	void HandlePrediction(const FAIStimulus& Stimulus) const;
	void HandleTeam(const AActor* UpdatedActor);
	
	/** Start or resume gunfight */
	void Fight();
	
	void SwitchWeapon();
	void TryToReload(bool bNoAmmoLeftToReload);
	
	/** Return nearest actor as actor object reference and distance to it */
	static float FindNearestOfTwoActor(AActor* Actor1, AActor* Actor2, const FVector& CurrentLocation, AActor* &CloserActor);

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
	
	float MaxFiringDistance = -1.0f;
	
private:
	uint8 bIsDisarm : 1, bDoOnceFight : 1, bDoOnceHelp : 1;
	
	UPROPERTY(EditDefaultsOnly, Category = "Default", meta = (AllowPrivateAccess = true))
	UBehaviorTree* BehaviorTree;

	UPROPERTY(EditDefaultsOnly, Category = "Default", meta = (AllowPrivateAccess = true))
	float MaxMeleeDistance = 200.0f;
	
	EWeaponState WeaponState = EWeaponState::Idle;
	
	/** Attacker is the current enemy that AI is fighting with */
	UPROPERTY()
	AActor* Attacker;
	
	EAIState AIState = EAIState::Idle;
	
	FTimerHandle BackToRoutineTimer, AskForHelpTimer;
};
