// All Rights Reserved.

#include "Core/AI/ShooterAIController.h"
#include "Actors/PatrolPathActor.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Blueprint/AIBlueprintHelperLibrary.h"
#include "Characters/AICharacter.h"
#include "Core/AI/CustomAIPerceptionComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Interfaces/AICharacterInterface.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Perception/AISenseConfig_Sight.h"
#include "Perception/AISenseConfig_Damage.h"
#include "Perception/AISenseConfig_Hearing.h"
#include "Perception/AISenseConfig_Prediction.h"

// Sets default values
AShooterAIController::AShooterAIController()
{
	// Create Components
	AIPerception = CreateDefaultSubobject<UCustomAIPerceptionComponent>(TEXT("AI Perception"));
	AISense_Damage = CreateDefaultSubobject<UAISenseConfig_Damage>(TEXT("Damage Sense"));
	AISense_Sight = CreateDefaultSubobject<UAISenseConfig_Sight>(TEXT("Sight Sence"));
	AISense_Hearing = CreateDefaultSubobject<UAISenseConfig_Hearing>(TEXT("Hearing Sense"));
	AISense_Prediction = CreateDefaultSubobject<UAISenseConfig_Prediction>(TEXT("Prediction Sense"));
	
	// Initialize components
	AIPerception->ConfigureSense(*AISense_Damage);
	AIPerception->ConfigureSense(*AISense_Sight);
	AIPerception->ConfigureSense(*AISense_Hearing);
	AIPerception->ConfigureSense(*AISense_Prediction);
	AIPerception->SetDominantSense(AISense_Damage->GetSenseImplementation());
	
	AISense_Sight->PeripheralVisionAngleDegrees = 45.0f;
	AISense_Sight->PointOfViewBackwardOffset = 250.0f;
	AISense_Sight->DetectionByAffiliation.bDetectEnemies = true;
	AISense_Sight->DetectionByAffiliation.bDetectFriendlies = true;
	AISense_Sight->DetectionByAffiliation.bDetectNeutrals = true;

	AISense_Hearing->DetectionByAffiliation.bDetectEnemies = true;
	AISense_Hearing->DetectionByAffiliation.bDetectFriendlies = true;
	AISense_Hearing->DetectionByAffiliation.bDetectNeutrals = true;
	
	AIPerception->OnPerceptionUpdated.AddDynamic(this, &AShooterAIController::PerceptionUpdated);
	AIPerception->OnTargetPerceptionUpdated.AddDynamic(this, &AShooterAIController::TargetPerceptionUpdated);
}

void AShooterAIController::BeginPlay()
{
	if (RunBehaviorTree(BehaviorTree))
	{
		AIBlackboard = UAIBlueprintHelperLibrary::GetBlackboard(this);
		
		if (GetPawn()->GetClass()->ImplementsInterface(UAICharacterInterface::StaticClass()))
		{
			ControlledPawn = IAICharacterInterface::Execute_GetAICharacterReference(GetPawn());
			PatrolPath = IAICharacterInterface::Execute_GetPatrolPath(GetPawn());
			// If AI patrol path is true then start patrolling
			if (PatrolPath)
			{
				AIBlackboard->SetValueAsBool(FName("PathLooping"), PatrolPath->bIsLooping);
				AIBlackboard->SetValueAsBool(FName("Direction"), true);
				AIBlackboard->SetValueAsFloat(FName("WaitTime"), PatrolPath->WaitTime);
			}
		}
	}
	
	AIPerception->SetStimulusAge(AISense_Damage->GetSenseID(), 0.1f);
	AIPerception->SetStimulusAge(AISense_Hearing->GetSenseID(), 0.1f);
}

void AShooterAIController::PerceptionUpdated(const TArray<AActor*>& UpdatedActors)
{
	const uint8 NumOfActors = UpdatedActors.Num();
	for (uint8 i = 0; i < NumOfActors; ++i)
	{
		AActor* UpdatedActor = UpdatedActors[i];
		if (UpdatedActor->GetClass()->ImplementsInterface(UCommonInterface::StaticClass()))
		{
			const FGameplayTag ActorTag = ICommonInterface::Execute_GetTeamTag(UpdatedActor);
			// Check if the actor is not in the same team as the this AI
			if (ActorTag != ControlledPawn->TeamTag)
			{
				FActorPerceptionBlueprintInfo ActorPerceptionInfo;
				AIPerception->GetActorsPerception(UpdatedActor, ActorPerceptionInfo);
				const uint8 NumOfStimulus = ActorPerceptionInfo.LastSensedStimuli.Num();
				for (uint8 j = 0; j < NumOfStimulus; ++j)
				{
					switch (j)
					{
					case 0:
						// Damage Sense
						DamageHandler(ActorPerceptionInfo.LastSensedStimuli[j]);
						break;
					case 1:
						// Sight Sense
						SightHandler(ActorPerceptionInfo.LastSensedStimuli[j]);
						break;
					case 2:
						// Hearing Sense
						HearingHandler(ActorPerceptionInfo.LastSensedStimuli[j]);
						break;
					case 3:
						// Prediction Sense
						PredictionHandler(ActorPerceptionInfo.LastSensedStimuli[j]);
						break;
					default:
						UE_LOG(LogTemp, Warning, TEXT("Unknown Sense!"));
					}
					UE_LOG(LogTemp, Warning, TEXT("Was Successfully Sensed = %s"), ActorPerceptionInfo.LastSensedStimuli[j].WasSuccessfullySensed() ? TEXT("true") : TEXT("false"));
				}
			}
		}
	}
}

void AShooterAIController::TargetPerceptionUpdated(AActor* Actor, FAIStimulus Stimulus)
{
	if (Stimulus.GetAge() == FLT_MAX)
	{
		UE_LOG(LogTemp, Error, TEXT("EXPIRED!"));
		Stimulus.MarkExpired();
	}
	// const FAISenseID ID = AISense_Sight->GetSenseID();
	// if (ID.IsValid() && Stimulus.IsValid() && Stimulus.Type == ID)
	// {
	// 	UE_LOG(LogTemp, Warning, TEXT("YES"));
	// }
	// MoveToActor(Actor);
	// if (AISense_Damage)
	// {
		// UE_LOG(LogTemp, Warning, TEXT("YES"));
	// }
	if (GetFocusActor() && GetFocusActor() == Actor)
	{
		
	}
}

void AShooterAIController::DamageHandler(FAIStimulus Stimulus)
{
	if (Stimulus.WasSuccessfullySensed())
	{
		UE_LOG(LogTemp, Warning, TEXT("Damage Sense"));
	}
}

void AShooterAIController::SightHandler(FAIStimulus Stimulus)
{
	if (Stimulus.WasSuccessfullySensed())
	{
		UE_LOG(LogTemp, Warning, TEXT("Sight Sense"));
	}
}

void AShooterAIController::HearingHandler(FAIStimulus Stimulus)
{
	if (Stimulus.WasSuccessfullySensed())
	{
		UE_LOG(LogTemp, Warning, TEXT("Hearing Sense"));
	}
}

void AShooterAIController::PredictionHandler(FAIStimulus Stimulus)
{
	if (Stimulus.WasSuccessfullySensed())
	{
		UE_LOG(LogTemp, Warning, TEXT("Prediction Sense"));
	}
}

void AShooterAIController::Fight()
{
	
}

void AShooterAIController::SetWeaponState_Implementation(FAmmoComponentInfo AmmoComponentInfo, EWeaponState NewWeaponState)
{
	WeaponState = NewWeaponState;
	switch (WeaponState)
	{
	case 0:
		// Idle
		if (AmmoComponentInfo.bNoAmmoLeftToReload)
		{
			SwitchWeapon();
		}
		else if (AmmoComponentInfo.CurrentMagazineAmmo < 0)
		{
			ControlledPawn->ReloadWeapon();
		}
		break;
	case 1:
		// Firing
		break;
	case 2:
		// Better To Reload, If there is no threat then reload the weapon
		if (!AIBlackboard->GetValueAsObject(FName("TargetActor")))
		{
			TryToReload(AmmoComponentInfo.bNoAmmoLeftToReload);
		}
		break;
	case 3:
		// Need To Reload
		TryToReload(AmmoComponentInfo.bNoAmmoLeftToReload);
		break;
	case 4:
		// Reloading
		break;
	case 5: case 6:
		// Cancel Reload, Reloaded
		Fight();
		break;
	case 7:
		// Ammo Added
		break;
	case 8:
		// Empty
		SwitchWeapon();
		break;
	case 9:
		// Overheat
		break;
	}
}

void AShooterAIController::SwitchWeapon()
{
	// If there is no weapon to switch then surrender
	switch (ControlledPawn->CurrentHoldingWeapon)
	{
	case 0:
		// No Weapon
		if (ControlledPawn->PrimaryWeapon)
		{
			ControlledPawn->SwitchToPrimary();
		}
		else if (ControlledPawn->SecondaryWeapon)
		{
			ControlledPawn->SwitchToSecondary();
		}
		else if (ControlledPawn->SidearmWeapon)
		{
			ControlledPawn->SwitchToSidearm();
		}
		else
		{
			ControlledPawn->DropItem();
			ControlledPawn->GetCharacterMovement()->DisableMovement();
			ControlledPawn->PlayAnimMontage(ControlledPawn->SurrenderMontage);
		}
		break;
	case 1:
		// Primary Weapon
		if (ControlledPawn->SecondaryWeapon)
		{
			ControlledPawn->SwitchToSecondary();
		}
		else if (ControlledPawn->SidearmWeapon)
		{
			ControlledPawn->SwitchToSidearm();
		}
		else
		{
			ControlledPawn->DropItem();
			ControlledPawn->GetCharacterMovement()->DisableMovement();
			ControlledPawn->PlayAnimMontage(ControlledPawn->SurrenderMontage);
		}
		break;
	case 2:
		// Secondary Weapon
		if (ControlledPawn->PrimaryWeapon)
		{
			ControlledPawn->SwitchToPrimary();
		}
		else if (ControlledPawn->SidearmWeapon)
		{
			ControlledPawn->SwitchToSidearm();
		}
		else
		{
			ControlledPawn->DropItem();
			ControlledPawn->GetCharacterMovement()->DisableMovement();
			ControlledPawn->PlayAnimMontage(ControlledPawn->SurrenderMontage);
		}
		break;
	case 3:
		// Sidearm Weapon
		if (ControlledPawn->PrimaryWeapon)
		{
			ControlledPawn->SwitchToPrimary();
		}
		else if (ControlledPawn->SecondaryWeapon)
		{
			ControlledPawn->SwitchToSecondary();
		}
		else
		{
			ControlledPawn->DropItem();
			ControlledPawn->GetCharacterMovement()->DisableMovement();
			ControlledPawn->PlayAnimMontage(ControlledPawn->SurrenderMontage);
		}
		break;
	}
}

void AShooterAIController::TryToReload(bool bNoAmmoLeftToReload)
{
	if (bNoAmmoLeftToReload)
	{
		SwitchWeapon();
	}
	else
	{
		ControlledPawn->ReloadWeapon();
	}
}

void AShooterAIController::SetAIState_Implementation(EAIState AIState)
{
	switch (AIState)
	{
	case 0: case 1: case 2:
		// Idle, Chase, Search
		ControlledPawn->SwitchToWeapon(true, EWeaponToDo::PrimaryWeapon);
		break;
	case 3:
		// Low Health
		ClearFocus(EAIFocusPriority::Move);
		ControlledPawn->UseWeapon(false, false);
		break;
	case 4:
		// Use Med
		break;
	}
}

void AShooterAIController::StartPatrol_Implementation()
{
	AIBlackboard->SetValueAsBool(FName("Patrol"), true);
}

AShooterAIController* AShooterAIController::GetAIControllerReference_Implementation()
{
	return this;
}