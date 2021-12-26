// All Rights Reserved.

#include "Core/AI/ShooterAIController.h"
#include "Actors/PatrolPathActor.h"
#include "Actors/PickupWeapon.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Blueprint/AIBlueprintHelperLibrary.h"
#include "Characters/AICharacter.h"
#include "Components/AmmoComponent.h"
#include "Core/AI/CustomAIPerceptionComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Interfaces/AICharacterInterface.h"
#include "Kismet/KismetMathLibrary.h"
#include "Perception/AISenseConfig_Sight.h"
#include "Perception/AISenseConfig_Damage.h"
#include "Perception/AISenseConfig_Hearing.h"
#include "Perception/AISenseConfig_Prediction.h"
// #include "Perception/AISense_Prediction.h"

// Sets default values
AShooterAIController::AShooterAIController()
{
	// Create Components
	AIPerception = CreateDefaultSubobject<UCustomAIPerceptionComponent>(TEXT("AI Perception"));
	AISense_Sight = CreateDefaultSubobject<UAISenseConfig_Sight>(TEXT("Sight Sence"));
	AISense_Damage = CreateDefaultSubobject<UAISenseConfig_Damage>(TEXT("Damage Sense"));
	AISense_Hearing = CreateDefaultSubobject<UAISenseConfig_Hearing>(TEXT("Hearing Sense"));
	AISense_Prediction = CreateDefaultSubobject<UAISenseConfig_Prediction>(TEXT("Prediction Sense"));
	BehaviorTreeComp = CreateDefaultSubobject<UBehaviorTreeComponent>(TEXT("Behavior Component"));
	BlackboardComp = CreateDefaultSubobject<UBlackboardComponent>(TEXT("Blackboard Component"));
	
	// Initialize components
	AIPerception->ConfigureSense(*AISense_Sight);
	AIPerception->ConfigureSense(*AISense_Damage);
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
}

void AShooterAIController::BeginPlay()
{
	Super::BeginPlay();

	RunBehaviorTree(BehaviorTree);
	BehaviorTreeComp->StartTree(*BehaviorTree);
	BlackboardComp->InitializeBlackboard(*BehaviorTree->BlackboardAsset);
	AIPerception->SetStimulusAge(AISense_Damage->GetSenseID(), 0.1f);
	AIPerception->SetStimulusAge(AISense_Hearing->GetSenseID(), 0.1f);
}

void AShooterAIController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);
	
	ControlledPawn = Cast<AAICharacter>(InPawn);
	if (ControlledPawn)
	{
		if (InPawn->GetClass()->ImplementsInterface(UAICharacterInterface::StaticClass()))
		{
			PatrolPath = IAICharacterInterface::Execute_GetPatrolPath(GetPawn());
			// If AI patrol path is true then start patrolling
			if (PatrolPath)
			{
				BlackboardComp->SetValueAsBool(FName("PathLooping"), PatrolPath->bIsLooping);
				BlackboardComp->SetValueAsBool(FName("Direction"), true);
				BlackboardComp->SetValueAsFloat(FName("WaitTime"), PatrolPath->WaitTime);
			}
		}
	}
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
						// Sight Sense
						SightHandler(UpdatedActor, ActorPerceptionInfo.LastSensedStimuli[j]);
						break;
					case 1:
						// Damage Sense
						DamageHandler(UpdatedActor, ActorPerceptionInfo.LastSensedStimuli[j]);
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
				}
			}
		}
	}
}

void AShooterAIController::SightHandler(AActor* UpdatedActor, FAIStimulus Stimulus)
{
	if (Stimulus.WasSuccessfullySensed())
	{
		// If not focused or the updated actor is focused actor (attacker) start shooting at it
		if (GetFocusActor() == nullptr || GetFocusActor() == UpdatedActor)
		{
			SetFocus(UpdatedActor, EAIFocusPriority::Gameplay);
			Attacker = UpdatedActor;
			Fight();
		}
	}
	else if (GetFocusActor() == UpdatedActor)
	{
		ClearFocus(EAIFocusPriority::Move);
		BlackboardComp->SetValueAsObject(FName("TargetActor"), nullptr);
		BlackboardComp->SetValueAsBool(FName("Search"), true);
		BlackboardComp->SetValueAsBool(FName("SearchForPlayer"), true);
		Attacker = nullptr;
		if (WeaponState != EWeaponState::Reloading)
		{
			ControlledPawn->UseWeapon(false, false);
		}
		// UAISense_Prediction::RequestControllerPredictionEvent(this, UpdatedActor, 1.0f);
	}
}

void AShooterAIController::DamageHandler(AActor* UpdatedActor, FAIStimulus Stimulus)
{
	// Start searching and if the Updated actor is a new enemy, check which one is closer then if the new one is closer set it as attacker
	if (Stimulus.WasSuccessfullySensed() && GetFocusActor() != UpdatedActor)
	{
		BlackboardComp->SetValueAsBool(FName("SearchForEnemy"), true);
		BlackboardComp->SetValueAsVector(FName("TargetLocation"), Stimulus.StimulusLocation);
		
		AActor* TargetActor;
		FindNearestOfTwoActor(GetFocusActor(), UpdatedActor, ControlledPawn->GetActorLocation(), TargetActor);
		if (GetFocusActor() != TargetActor)
		{
			BlackboardComp->SetValueAsObject(FName("TargetActor"), TargetActor);
			SetFocus(UpdatedActor, EAIFocusPriority::Gameplay);
			Attacker = TargetActor;
		}
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
	switch (AIState)
	{
	case 0: case 1: case 2:
		// Idle, Chase, Search
		if (Attacker)
		{
			TryToUseWeapon();
		}
		break;
	case 3:
		// Low Health
		break;
	case 4:
		// Use Med
		break;
	}
}

void AShooterAIController::TryToUseWeapon()
{
	const FAmmoComponentInfo AmmoComponentInfo;
	switch (WeaponState)
	{
	case 0: case 1: case 2: case 5: case 6: case 7:
		// Idle, Firing, Better To Reload, Cancel Reload, Reloaded, Ammo Added
		ControlledPawn->UseWeapon(true, true);
		break;
	case 3:
		// Need To Reload
		SetWeaponState_Implementation(AmmoComponentInfo, EWeaponState::NeedToReload);
		break;
	case 4:
		// Reloading
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
		if (!BlackboardComp->GetValueAsObject(FName("TargetActor")))
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
		// If AI is in combat and there is an attacker and half of the mag is already full then stop reloading and continue fighting
		if (Attacker && ControlledPawn->CurrentWeapon->AmmoComponent->CurrentMagazineAmmo > ControlledPawn->CurrentWeapon->AmmoComponent->MagazineSize / 2)
		{
			Fight();
		}
		break;
	case 5: case 6:
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

void AShooterAIController::SwitchWeapon() const
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

void AShooterAIController::TryToReload(bool bNoAmmoLeftToReload) const
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

void AShooterAIController::SetAIState_Implementation(EAIState NewAIState)
{
	AIState = NewAIState;
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

void AShooterAIController::StartPatrol()
{
	// BlackboardComp->SetValueAsBool(FName("Patrol"), true);
}

AShooterAIController* AShooterAIController::GetAIControllerReference_Implementation()
{
	return this;
}

float AShooterAIController::FindNearestOfTwoActor(AActor* Actor1, AActor* Actor2, FVector CurrentLocation, AActor*& CloserActor)
{
	float DistanceToActor;
	if (Actor1)
	{
		DistanceToActor = UKismetMathLibrary::Vector_Distance(Actor1->GetActorLocation(), CurrentLocation);
		if (Actor2)
		{
			const float NewDistance = UKismetMathLibrary::Vector_Distance(Actor2->GetActorLocation(), CurrentLocation);
			if (NewDistance < DistanceToActor)
			{
				DistanceToActor = NewDistance;
				CloserActor = Actor2;
				return DistanceToActor;
			}
		}
		
		CloserActor = Actor1;
		return DistanceToActor;
	}
	
	if (Actor2)
	{
		DistanceToActor = UKismetMathLibrary::Vector_Distance(Actor2->GetActorLocation(), CurrentLocation);
		CloserActor = Actor2;
		return DistanceToActor;
	}
	
	CloserActor = nullptr;
	return 0.0f;
}