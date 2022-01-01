// All Rights Reserved.

#include "Core/AI/ShooterAIController.h"
#include "Actors/PatrolPathActor.h"
#include "Actors/PickupWeapon.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Characters/AICharacter.h"
#include "Components/AmmoComponent.h"
#include "EnvironmentQuery/EnvQueryManager.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Interfaces/AICharacterInterface.h"
#include "Kismet/KismetMathLibrary.h"
#include "Perception/AIPerceptionComponent.h"
#include "Perception/AISenseConfig_Sight.h"
#include "Perception/AISenseConfig_Damage.h"
#include "Perception/AISenseConfig_Hearing.h"
#include "Perception/AISenseConfig_Prediction.h"
#include "Perception/AISense_Prediction.h"

// Sets default values
AShooterAIController::AShooterAIController()
{
	// Create Components
	AIPerception = CreateDefaultSubobject<UAIPerceptionComponent>(TEXT("AI Perception"));
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

	AISense_Damage->SetMaxAge(0.2f);
	
	AISense_Hearing->HearingRange = 5000.0;
	AISense_Hearing->SetMaxAge(0.5);
	AISense_Hearing->DetectionByAffiliation.bDetectEnemies = true;
	AISense_Hearing->DetectionByAffiliation.bDetectFriendlies = true;
	AISense_Hearing->DetectionByAffiliation.bDetectNeutrals = true;

	AISense_Prediction->SetMaxAge(0.2f);

	AIPerception->OnPerceptionUpdated.AddDynamic(this, &AShooterAIController::PerceptionUpdated);
}

void AShooterAIController::BeginPlay()
{
	Super::BeginPlay();

	RunBehaviorTree(BehaviorTree);
	BehaviorTreeComp->StartTree(*BehaviorTree);
	BlackboardComp->InitializeBlackboard(*BehaviorTree->BlackboardAsset);
}

void AShooterAIController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);
	
	ControlledPawn = Cast<AAICharacter>(InPawn);
	if (ControlledPawn)
	{
		if (InPawn->GetClass()->ImplementsInterface(UAICharacterInterface::StaticClass()))
		{
			bAICharacterInterface = true;
			PatrolPath = IAICharacterInterface::Execute_GetPatrolPath(GetPawn());
			// If patrol path is valid then start patrolling
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
	if (!bIsDisarm)
	{
		const uint8 NumOfActors = UpdatedActors.Num();
		for (uint8 i = 0; i < NumOfActors; ++i)
		{
			AActor* UpdatedActor = UpdatedActors[i];
			if (UpdatedActor->GetClass()->ImplementsInterface(UCommonInterface::StaticClass()))
			{
				const FGameplayTag ActorTag = ICommonInterface::Execute_GetTeamTag(UpdatedActor);
				FActorPerceptionBlueprintInfo ActorPerceptionInfo;
				AIPerception->GetActorsPerception(UpdatedActor, ActorPerceptionInfo);
				const uint8 NumOfStimulus = ActorPerceptionInfo.LastSensedStimuli.Num();
				for (uint8 j = 0; j < NumOfStimulus; ++j)
				{
					switch (j)
					{
					case 0:
						// Sight Sense, Check if the actor is not in the same team as the this AI
						if (ActorTag != ControlledPawn->TeamTag)
						{
							HandleSight(UpdatedActor, ActorPerceptionInfo.LastSensedStimuli[j]);
						}
						break;
					case 1:
						// Damage Sense, Only handle damage if it's from a new enemy
						if (ActorPerceptionInfo.LastSensedStimuli[j].WasSuccessfullySensed() && ActorTag != ControlledPawn->TeamTag && GetFocusActor() != UpdatedActor)
						{
							HandleDamage(UpdatedActor, ActorPerceptionInfo.LastSensedStimuli[j]);
						}
						break;
					case 2:
						// Hearing Sense, Handle hearing if not currently in a fight
						if (ActorPerceptionInfo.LastSensedStimuli[j].WasSuccessfullySensed() && !Attacker)
						{
							// If the sound is from a teammate that asking for help, move to its locations
							if (ActorPerceptionInfo.LastSensedStimuli[j].Tag == "Help" && ControlledPawn->TeamTag == ActorTag)
							{
								HandleTeam(UpdatedActor);
							}
							else if (ControlledPawn->TeamTag != ActorTag)
							{
								HandleHearing(UpdatedActor, ActorPerceptionInfo.LastSensedStimuli[j]);
							}
						}
						break;
					case 3:
						// Prediction Sense
						if (ActorTag != ControlledPawn->TeamTag)
						{
							HandlePrediction(ActorPerceptionInfo.LastSensedStimuli[j]);
						}
						break;
					default:
						UE_LOG(LogTemp, Warning, TEXT("Unknown Sense!"));
					}
				}
			}
		}
	}
}

void AShooterAIController::HandleSight(AActor* UpdatedActor, FAIStimulus Stimulus)
{
	if (Stimulus.WasSuccessfullySensed())
	{
		// Notify the rest of the team (asking for help)
		UAISense_Hearing::ReportNoiseEvent(GetWorld(), ControlledPawn->GetActorLocation(), 1.0f, ControlledPawn, 0.0f, FName("Help")); // TODO - Improve it

		// Start fighting if not focused or the updated actor is the focused actor (attacker)
		if (GetFocusActor() == nullptr || GetFocusActor() == UpdatedActor)
		{
			SetFocus(UpdatedActor);
			BlackboardComp->SetValueAsBool(FName("TakeCover"), false);
			BlackboardComp->SetValueAsBool(FName("Search"), false);
			BlackboardComp->SetValueAsBool(FName("SearchForSound"), false);
			BlackboardComp->SetValueAsBool(FName("SearchForEnemy"), false);
			BlackboardComp->SetValueAsObject(FName("TargetActor"), UpdatedActor);
			Attacker = UpdatedActor;
			AIState = EAIState::Fight;
			Fight();
		}
	}
	else if (GetFocusActor() == UpdatedActor)
	{
		ClearFocus(EAIFocusPriority::Gameplay);
		BlackboardComp->SetValueAsObject(FName("TargetActor"), nullptr);
		BlackboardComp->SetValueAsBool(FName("TakeCover"), false);
		BlackboardComp->SetValueAsBool(FName("Search"), true);
		BlackboardComp->SetValueAsBool(FName("SearchForSound"), false);
		BlackboardComp->SetValueAsBool(FName("SearchForEnemy"), true);
		Attacker = nullptr;
		if (WeaponState != EWeaponState::Reloading)
		{
			ControlledPawn->UseWeapon(false, false);
		}
		
		AIState = EAIState::Search;
		UAISense_Prediction::RequestControllerPredictionEvent(this, UpdatedActor, 1.0f);
	}
}

void AShooterAIController::HandleDamage(AActor* UpdatedActor, FAIStimulus Stimulus)
{
	// Start searching and if the Updated actor is a new enemy, check which one is closer then if the new one is closer set it as attacker
	if (GetFocusActor() != UpdatedActor)
	{
		AIState = EAIState::Search;
		BlackboardComp->SetValueAsBool(FName("SearchForSound"), false);
		BlackboardComp->SetValueAsBool(FName("SearchForEnemy"), true);
		BlackboardComp->SetValueAsVector(FName("TargetLocation"), Stimulus.StimulusLocation);
		
		AActor* TargetActor;
		FindNearestOfTwoActor(GetFocusActor(), UpdatedActor, ControlledPawn->GetActorLocation(), TargetActor);
		if (GetFocusActor() != TargetActor)
		{
			BlackboardComp->SetValueAsObject(FName("TargetActor"), TargetActor);
			SetFocus(UpdatedActor);
			Attacker = TargetActor;
		}
	}
}

void AShooterAIController::HandleHearing(AActor* UpdatedActor, FAIStimulus Stimulus)
{
	// Run EQS to check if updated actor is reachable
	SetFocus(UpdatedActor);
	FEnvQueryRequest HidingSpotQueryRequest = FEnvQueryRequest(CanReachTarget, this);
	HidingSpotQueryRequest.Execute(EEnvQueryRunMode::SingleResult, this, &AShooterAIController::HandleQueryResult);
		
	if (bHasPath)
	{
		UE_LOG(LogTemp, Warning, TEXT(__FUNCTION__));
		AIState = EAIState::Search;
		BlackboardComp->SetValueAsVector(FName("TargetLocation"), Stimulus.StimulusLocation);
		BlackboardComp->SetValueAsBool(FName("Urgent"), false);
		BlackboardComp->SetValueAsBool(FName("SearchForSound"), false);

		if (BlackboardComp->GetValueAsBool(FName("Search")))
		{
			const FAmmoComponentInfo AmmoComponentInfo;
			switch (AIState)
			{
			case 0: case 1: case 2:
				// Idle, Fight, Search
				switch (WeaponState)
				{
				case 0: case 1: case 2: case 5: case 6: case 7:
					// Idle, Firing, Better To Reload, Cancel Reload, Reloaded
					ControlledPawn->UseWeapon(true, false);
					BlackboardComp->SetValueAsBool(FName("Search"), true);
					BlackboardComp->SetValueAsBool(FName("SearchForSound"), true);
					break;
				case 3:
					// Need To Reload
					SetWeaponState_Implementation(AmmoComponentInfo, EWeaponState::NeedToReload);
					BlackboardComp->SetValueAsBool(FName("Search"), true);
					BlackboardComp->SetValueAsBool(FName("SearchForSound"), true);
					break;
				case 4:
					// Reloading
					BlackboardComp->SetValueAsBool(FName("Search"), true);
					BlackboardComp->SetValueAsBool(FName("SearchForSound"), true);
					break;
				case 8:
					// Empty
					SwitchWeapon();
					BlackboardComp->SetValueAsBool(FName("Search"), true);
					BlackboardComp->SetValueAsBool(FName("SearchForSound"), true);
					break;
				case 9:
					// Overheat
					BlackboardComp->SetValueAsBool(FName("Search"), true);
					BlackboardComp->SetValueAsBool(FName("SearchForSound"), true);
					break;
				}
				break;
			case 3: case 4:
				// Low Health, Use Med
				BlackboardComp->SetValueAsBool(FName("Search"), true);
				BlackboardComp->SetValueAsBool(FName("SearchForSound"), true);
				break;
			}
		}
		else
		{
			SetFocus(UpdatedActor);
			BlackboardComp->SetValueAsBool(FName("Search"), true);
			BlackboardComp->SetValueAsBool(FName("SearchForSound"), true);
		}
	}
	// If enemy is unreachable then take cover
	else if (!BlackboardComp->GetValueAsBool(FName("TakeCover")))
	{
		AIState = EAIState::Idle;
		BlackboardComp->SetValueAsBool(FName("TakeCover"), true);
		BlackboardComp->SetValueAsFloat(FName("WaitTime"), 5.0f);
		GetWorld()->GetTimerManager().SetTimer(BackToRoutineTimer, this, &AShooterAIController::BackToRoutine, 5.0f);
	}
}

void AShooterAIController::HandlePrediction(FAIStimulus Stimulus) const
{
	if (Stimulus.WasSuccessfullySensed())
	{
		BlackboardComp->SetValueAsVector(FName("TargetLocation"), Stimulus.StimulusLocation);
	}
}

void AShooterAIController::HandleTeam(AActor* UpdatedActor)
{
	AAIController* AIC = Cast<AAIController>(UpdatedActor->GetInstigatorController());
	if (AIC)
	{
		BlackboardComp->SetValueAsBool(FName("TakeCover"), false);
		BlackboardComp->SetValueAsVector(FName("TargetLocation"), UpdatedActor->GetActorLocation());
		BlackboardComp->SetValueAsBool(FName("Urgent"), true);
		BlackboardComp->SetValueAsBool(FName("SearchForSound"), false);
		BlackboardComp->SetValueAsBool(FName("SearchForEnemy"), true);
		BlackboardComp->SetValueAsBool(FName("Search"), true);
		SetFocus(AIC->GetFocusActor());
	}
}

void AShooterAIController::BackToRoutine()
{
	// Only return to default behavior if AI is currently idle
	if (AIState == EAIState::Idle)
	{
		// Stop taking cover
		ClearFocus(EAIFocusPriority::Gameplay);
		ControlledPawn->UseWeapon(false, false);
		BlackboardComp->SetValueAsBool(FName("TakeCover"), false);
		BlackboardComp->SetValueAsFloat(FName("WaitTime"), 0.0f);
	
		if (bAICharacterInterface)
		{
			PatrolPath = IAICharacterInterface::Execute_GetPatrolPath(GetPawn());
		}

		// If patrol path is valid then start patrolling
		if (PatrolPath)
		{
			BlackboardComp->SetValueAsBool(FName("PathLooping"), PatrolPath->bIsLooping);
			BlackboardComp->SetValueAsBool(FName("Direction"), true);
			BlackboardComp->SetValueAsFloat(FName("WaitTime"), PatrolPath->WaitTime);
		}
		else
		{
			BlackboardComp->SetValueAsObject(FName("TargetActor"), nullptr);
			BlackboardComp->SetValueAsBool(FName("Search"), false);
		}
	}
}

void AShooterAIController::HandleQueryResult(TSharedPtr<FEnvQueryResult> Result)
{
	if (Result->IsSuccessful())
	{
		bHasPath = true;
	}
	else
	{
		bHasPath = false;
	}
}

void AShooterAIController::Fight()
{
	switch (AIState)
	{
	case 0: case 1: case 2:
		// Idle, Fight, Search
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
			if (ControlledPawn->CurrentWeapon->AmmoComponent->CurrentMagazineAmmo > 0)
			{
				ControlledPawn->UseWeapon(true, true);
			}
			else
			{
				SwitchWeapon();
			}
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
		if (!Attacker && !BlackboardComp->GetValueAsObject(FName("TargetActor")))
		{
			TryToReload(AmmoComponentInfo.bNoAmmoLeftToReload);
		}
		break;
	case 3:
		// Need To 
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
	case 5: case 6: case 7:
		// Cancel Reload, Reloaded, Ammo Added
		Fight();
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
	FTimerDelegate TimerDelegate;
	TimerDelegate.BindUObject(this, &AShooterAIController::Surrender);
	// If there is no weapon to switch then surrender
	switch (ControlledPawn->CurrentHoldingWeapon)
	{
	case 0:
		// No Weapon
		if (ControlledPawn->PrimaryWeapon && ControlledPawn->PrimaryWeapon->AmmoComponent->CurrentAmmo > 0)
		{
			ControlledPawn->SwitchToPrimary();
		}
		else if (ControlledPawn->SecondaryWeapon && ControlledPawn->SecondaryWeapon->AmmoComponent->CurrentAmmo > 0)
		{
			ControlledPawn->SwitchToSecondary();
		}
		else if (ControlledPawn->SidearmWeapon && ControlledPawn->SidearmWeapon->AmmoComponent->CurrentAmmo > 0)
		{
			ControlledPawn->SwitchToSidearm();
		}
		else
		{
			ControlledPawn->UseWeapon(false, false);
			GetWorld()->GetTimerManager().SetTimerForNextTick(TimerDelegate);
		}
		break;
	case 1:
		// Primary Weapon
		if (ControlledPawn->SecondaryWeapon && ControlledPawn->SecondaryWeapon->AmmoComponent->CurrentAmmo > 0)
		{
			ControlledPawn->SwitchToSecondary();
		}
		else if (ControlledPawn->SidearmWeapon && ControlledPawn->SidearmWeapon->AmmoComponent->CurrentAmmo > 0)
		{
			ControlledPawn->SwitchToSidearm();
		}
		else
		{
			ControlledPawn->UseWeapon(false, false);
			GetWorld()->GetTimerManager().SetTimerForNextTick(TimerDelegate);
		}
		break;
	case 2:
		// Secondary Weapon
		if (ControlledPawn->PrimaryWeapon && ControlledPawn->PrimaryWeapon->AmmoComponent->CurrentAmmo > 0)
		{
			ControlledPawn->SwitchToPrimary();
		}
		else if (ControlledPawn->SidearmWeapon && ControlledPawn->SidearmWeapon->AmmoComponent->CurrentAmmo > 0)
		{
			ControlledPawn->SwitchToSidearm();
		}
		else
		{
			ControlledPawn->UseWeapon(false, false);
			GetWorld()->GetTimerManager().SetTimerForNextTick(TimerDelegate);
		}
		break;
	case 3:
		// Sidearm Weapon
		if (ControlledPawn->PrimaryWeapon && ControlledPawn->PrimaryWeapon->AmmoComponent->CurrentAmmo > 0)
		{
			ControlledPawn->SwitchToPrimary();
		}
		else if (ControlledPawn->SecondaryWeapon && ControlledPawn->SecondaryWeapon->AmmoComponent->CurrentAmmo > 0)
		{
			ControlledPawn->SwitchToSecondary();
		}
		else
		{
			ControlledPawn->UseWeapon(false, false);
			GetWorld()->GetTimerManager().SetTimerForNextTick(TimerDelegate);
		}
		break;
	}
}

bool AShooterAIController::CheckWeapon(bool SwitchToAvailable, EWeaponToDo WeaponToSwitch)
{
	if (WeaponToSwitch != ControlledPawn->CurrentHoldingWeapon)
	{
		switch (WeaponToSwitch)
		{
		case 0:
			// No Weapon
			ControlledPawn->HolsterWeapon();
			return true;
		case 1:
			// Primary Weapon
			if (ControlledPawn->PrimaryWeapon)
			{
				ControlledPawn->SwitchToPrimary();
				return true;
			}
			if (SwitchToAvailable)
			{
				if (ControlledPawn->SecondaryWeapon)
				{
					ControlledPawn->SwitchToSecondary();
					return true;
				}
				if (ControlledPawn->SidearmWeapon)
				{
					ControlledPawn->SwitchToSidearm();
					return true;
				}
			}
			return false;
		case 2:
			// Secondary Weapon
			if (ControlledPawn->SecondaryWeapon)
			{
				ControlledPawn->SwitchToSecondary();
				return true;
			}
			if (SwitchToAvailable)
			{
				if (ControlledPawn->PrimaryWeapon)
				{
					ControlledPawn->SwitchToPrimary();
					return true;
				}
				if (ControlledPawn->SidearmWeapon)
				{
					ControlledPawn->SwitchToSidearm();
					return true;
				}
			}
			return false;
		case 3:
			// Sidearm Weapon
			if (ControlledPawn->SidearmWeapon)
			{
				ControlledPawn->SwitchToSidearm();
				return true;
			}
			if (SwitchToAvailable)
			{
				if (ControlledPawn->PrimaryWeapon)
				{
					ControlledPawn->SwitchToPrimary();
					return true;
				}
				if (ControlledPawn->SecondaryWeapon)
				{
					ControlledPawn->SwitchToSecondary();
					return true;
				}
			}
			return false;
		}
		return false;
	}
	return true;
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

void AShooterAIController::SetAIState_Implementation(EAIState NewAIState)
{
	AIState = NewAIState;
	switch (AIState)
	{
	case 0: case 1: case 2:
		// Idle, Fight, Search
		CheckWeapon(true, EWeaponToDo::PrimaryWeapon);
		break;
	case 3:
		// Low Health
		ClearFocus(EAIFocusPriority::Gameplay);
		ControlledPawn->UseWeapon(false, false);
		break;
	case 4:
		// Use Med
		break;
	}
}

void AShooterAIController::StartPatrol()
{
	BlackboardComp->SetValueAsBool(FName("Patrol"), true);
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

void AShooterAIController::Surrender()
{
	ControlledPawn->DropCurrentObject();
	bIsDisarm = true;
	BlackboardComp->SetValueAsBool(FName("IsDisarm"), true);
	BlackboardComp->SetValueAsFloat(FName("WaitTime"), 60.0f); // TODO - improve it with head following the nearest enemy
	ClearFocus(EAIFocusPriority::Gameplay);
	ControlledPawn->GetCharacterMovement()->DisableMovement();
	ControlledPawn->PlayAnimMontage(ControlledPawn->SurrenderMontage);
}