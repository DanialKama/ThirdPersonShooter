// All Rights Reserved.

#include "Core/ShooterAIController.h"
#include "Actors/PatrolPathActor.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Blueprint/AIBlueprintHelperLibrary.h"
#include "Characters/AICharacter.h"
#include "Interfaces/AICharacterInterface.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Perception/AIPerceptionComponent.h"
#include "Perception/AISenseConfig_Sight.h"
#include "Perception/AISenseConfig_Damage.h"
#include "Perception/AISenseConfig_Hearing.h"
#include "Perception/AISenseConfig_Prediction.h"

// Sets default values
AShooterAIController::AShooterAIController()
{
	// Create Components
	AIPerception = CreateDefaultSubobject<UAIPerceptionComponent>(TEXT("AI Perception"));
	AISense_Sight = CreateDefaultSubobject<UAISenseConfig_Sight>(TEXT("Sight Sence"));
	AISense_Damage = CreateDefaultSubobject<UAISenseConfig_Damage>(TEXT("Damage Sense"));
	AISense_Hearing = CreateDefaultSubobject<UAISenseConfig_Hearing>(TEXT("Hearing Sense"));
	AISense_Prediction = CreateDefaultSubobject<UAISenseConfig_Prediction>(TEXT("Prediction Sense"));
	
	// Initialize components
	AIPerception->ConfigureSense(*AISense_Sight);
	AIPerception->ConfigureSense(*AISense_Damage);
	AIPerception->ConfigureSense(*AISense_Hearing);
	AIPerception->ConfigureSense(*AISense_Prediction);
	AIPerception->SetDominantSense(AISense_Sight->GetSenseImplementation());

	AISense_Sight->PeripheralVisionAngleDegrees = 45.0f;
	AISense_Sight->PointOfViewBackwardOffset = 250.0f;
	AISense_Sight->DetectionByAffiliation.bDetectEnemies = true;
	AISense_Sight->DetectionByAffiliation.bDetectFriendlies = true;
	AISense_Sight->DetectionByAffiliation.bDetectNeutrals = true;

	AISense_Hearing->DetectionByAffiliation.bDetectEnemies = true;
	AISense_Hearing->DetectionByAffiliation.bDetectFriendlies = true;
	AISense_Hearing->DetectionByAffiliation.bDetectNeutrals = true;
	
	AIPerception->OnTargetPerceptionUpdated.AddDynamic(this, &AShooterAIController::PerceptionUpdated);
}

void AShooterAIController::BeginPlay()
{
	if (RunBehaviorTree(BehaviorTree))
	{
		ImplementedBlackboard = UAIBlueprintHelperLibrary::GetBlackboard(this);
		
		if (GetPawn()->GetClass()->ImplementsInterface(UAICharacterInterface::StaticClass()))
		{
			ControlledPawn = IAICharacterInterface::Execute_GetAICharacterReference(GetPawn());
			PatrolPath = IAICharacterInterface::Execute_GetPatrolPath(GetPawn());
			// If AI patrol path is true then start patrolling
			if (PatrolPath)
			{
				ImplementedBlackboard->SetValueAsBool(FName("PathLooping"), PatrolPath->bIsLooping);
				ImplementedBlackboard->SetValueAsBool(FName("Direction"), true);
				ImplementedBlackboard->SetValueAsFloat(FName("WaitTime"), PatrolPath->WaitTime);
			}
		}
	}
}

void AShooterAIController::PerceptionUpdated(AActor* Actor, FAIStimulus Stimulus)	// TODO Change OnTargetPerceptionUpdated with PerceptionUpdated
{
	if (Actor->GetClass()->ImplementsInterface(UCommonInterface::StaticClass()))
	{
		const FGameplayTag ActorTag = ICommonInterface::Execute_GetTeamTag(Actor);
		// Check if the actor is not in the same team as the AI
		if (ActorTag != ControlledPawn->TeamTag)
		{
			const FString String = UAIPerceptionSystem::GetSenseClassForStimulus(GetWorld(), Stimulus)->GetName();
			UE_LOG(LogTemp, Warning, TEXT("%S"), *String);
			const FName Name = FName(*String);
			switch (Name.GetNumber())
			{
			case 0:
				break;
			case 1:
				break;
			case 2:
				break;
			case 3:
				break;
				default:
					UE_LOG(LogTemp, Warning, TEXT("Default"));
			}
		}
	}
}

void AShooterAIController::SetWeaponState_Implementation(FAmmoComponentInfo AmmoComponentInfo, EWeaponState WeaponState)
{
	
}

void AShooterAIController::SetAIState_Implementation(EAIState AIState)
{
	
}

void AShooterAIController::StartPatrol_Implementation()
{
	
}

AShooterAIController* AShooterAIController::GetAIControllerReference_Implementation()
{
	return this;
}