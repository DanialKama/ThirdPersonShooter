// Copyright 2022-2023 Danial Kamali. All Rights Reserved.

#include "ShooterAIController.h"

#include "Actors/Interactable/PickupWeapon.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Characters/AICharacter.h"
#include "Components/AmmoComponent.h"
#include "NavigationPath.h"
#include "NavigationSystem.h"
#include "Components/HealthComponent.h"
#include "Perception/AIPerceptionComponent.h"
#include "Perception/AISense_Prediction.h"
#include "Perception/AISenseConfig_Damage.h"
#include "Perception/AISenseConfig_Hearing.h"
#include "Perception/AISenseConfig_Prediction.h"
#include "Perception/AISenseConfig_Sight.h"

// TODO: Simplify the controller and move logic to the behaviour tree

AShooterAIController::AShooterAIController()
{
	AISense_Sight = CreateDefaultSubobject<UAISenseConfig_Sight>(TEXT("Sight Sense"));
	AISense_Sight->PeripheralVisionAngleDegrees = 45.0f;
	AISense_Sight->PointOfViewBackwardOffset = 250.0f;
	AISense_Sight->DetectionByAffiliation.bDetectEnemies = true;
	AISense_Sight->DetectionByAffiliation.bDetectFriendlies = true;
	AISense_Sight->DetectionByAffiliation.bDetectNeutrals = true;
	
	AISense_Damage = CreateDefaultSubobject<UAISenseConfig_Damage>(TEXT("Damage Sense"));
	AISense_Damage->SetMaxAge(0.2f);
	
	AISense_Hearing = CreateDefaultSubobject<UAISenseConfig_Hearing>(TEXT("Hearing Sense"));
	AISense_Hearing->HearingRange = 5000.0;
	AISense_Hearing->SetMaxAge(0.2f);
	AISense_Hearing->DetectionByAffiliation.bDetectEnemies = true;
	AISense_Hearing->DetectionByAffiliation.bDetectFriendlies = true;
	AISense_Hearing->DetectionByAffiliation.bDetectNeutrals = true;
	
	AISense_Prediction = CreateDefaultSubobject<UAISenseConfig_Prediction>(TEXT("Prediction Sense"));
	AISense_Prediction->SetMaxAge(0.2f);

	AIPerception = CreateDefaultSubobject<UAIPerceptionComponent>(TEXT("AI Perception"));
	
	// Initialize variables
	bDoOnceFight = true;
	bDoOnceHelp = true;
	bIsDisarm = false;
}

void AShooterAIController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	// TODO: Use GetPawn()
	ControlledPawn = Cast<AAICharacter>(InPawn);

	RunBehaviorTree(BehaviorTree);

	AIPerception->OnPerceptionUpdated.AddDynamic(this, &AShooterAIController::PerceptionUpdated);
}

bool AShooterAIController::InitializeBlackboard(UBlackboardComponent& BlackboardComp, UBlackboardData& BlackboardAsset)
{
	if (Super::InitializeBlackboard(BlackboardComp, BlackboardAsset))
	{
		AIPerception->ConfigureSense(*AISense_Sight);
		AIPerception->ConfigureSense(*AISense_Damage);
		AIPerception->ConfigureSense(*AISense_Hearing);
		AIPerception->ConfigureSense(*AISense_Prediction);
		AIPerception->SetDominantSense(AISense_Damage->GetSenseImplementation());

		Blackboard->SetValueAsFloat(FName("Health"), ControlledPawn->HealthComponent->DefaultHealth / ControlledPawn->HealthComponent->MaxHealth);

		return true;
	}

	return false;
}

void AShooterAIController::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	
	if (Attacker &&
		(WeaponState == EWeaponState::Idle || WeaponState == EWeaponState::Reloaded) &&
		MaxFiringDistance > FVector::Distance(ControlledPawn->GetActorLocation(), Attacker->GetActorLocation()))
	{
		Fight();
	}
}

void AShooterAIController::PerceptionUpdated(const TArray<AActor*>& UpdatedActors)
{
	if (bIsDisarm)
	{
		return;
	}
	
	const uint8 NumOfActors = UpdatedActors.Num();
	for (uint8 i = 0; i < NumOfActors; ++i)
	{
		AActor* UpdatedActor = UpdatedActors[i];
		if (UpdatedActor->GetClass()->ImplementsInterface(UCommonInterface::StaticClass()) == false)
		{
			continue;
		}
		
		const FGameplayTag ActorTag = ICommonInterface::Execute_GetTeamTag(UpdatedActor);
		FActorPerceptionBlueprintInfo ActorPerceptionInfo;
		AIPerception->GetActorsPerception(UpdatedActor, ActorPerceptionInfo);
		const uint8 NumOfStimulus = ActorPerceptionInfo.LastSensedStimuli.Num();
		for (uint8 j = 0; j < NumOfStimulus; ++j)
		{
			switch (j)
			{
				case 0:
					// Sight Sense, Check if the actor is not in the same team as this AI
					if (ActorTag != ControlledPawn->TeamTag)
					{
						HandleSight(UpdatedActor, ActorPerceptionInfo.LastSensedStimuli[j]);
					}
					else if (ActorPerceptionInfo.LastSensedStimuli[j].WasSuccessfullySensed() && Attacker == nullptr && GetFocusActor() == nullptr && Blackboard->GetValueAsBool(FName("SearchForSound")))
					{
						ControlledPawn->UseWeapon(false, false);
					}
					break;
				case 1:
					// Damage Sense, Only handle damage if it's from a new enemy
					if (ActorPerceptionInfo.LastSensedStimuli[j].WasSuccessfullySensed() && ControlledPawn->TeamTag != ActorTag && GetFocusActor() != UpdatedActor)
					{
						HandleDamage(UpdatedActor, ActorPerceptionInfo.LastSensedStimuli[j]);
					}
					break;
				case 2:
					// Hearing Sense, Handle hearing if not currently in a fight
					if (ActorPerceptionInfo.LastSensedStimuli[j].WasSuccessfullySensed() && Attacker == nullptr)
					{
						// If the sound is from a teammate that asking for help, move to its locations
						if (ActorPerceptionInfo.LastSensedStimuli[j].Tag == "Help" && ControlledPawn->TeamTag == ActorTag)
						{
							HandleTeam(UpdatedActor);
						}
						else if (ControlledPawn->TeamTag != ActorTag || GetFocusActor() == nullptr)
						{
							HandleHearing(ActorPerceptionInfo.LastSensedStimuli[j]);
						}
					}
					break;
				case 3:
					// Prediction Sense
					if (ActorPerceptionInfo.LastSensedStimuli[j].WasSuccessfullySensed() && ActorTag != ControlledPawn->TeamTag)
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

void AShooterAIController::HandleSight(AActor* UpdatedActor, const FAIStimulus& Stimulus)
{
	if (Stimulus.WasSuccessfullySensed())
	{
		// Notify the rest of the team (asking for help)
		if (bDoOnceHelp)
		{
			bDoOnceHelp = false;
			GetWorld()->GetTimerManager().ClearTimer(AskForHelpTimer);
			GetWorld()->GetTimerManager().SetTimer(AskForHelpTimer, this, &AShooterAIController::AskForHelp, 0.25f);
		}

		// Start fighting if not focused or the updated actor is the focused actor (attacker)
		if (GetFocusActor() == nullptr || GetFocusActor() == UpdatedActor)
		{
			SetFocus(UpdatedActor);
			Attacker = UpdatedActor;
			AIState = EAIState::Fight;
			
			if (bDoOnceFight)
			{
				bDoOnceFight = false;
				Blackboard->SetValueAsBool(FName("TakeCover"), false);
				Blackboard->SetValueAsBool(FName("Search"), false);
				Blackboard->SetValueAsBool(FName("Urgent"), false);
				Blackboard->SetValueAsBool(FName("SearchForSound"), false);
				Blackboard->SetValueAsBool(FName("SearchForEnemy"), false);
				Blackboard->SetValueAsObject(FName("TargetActor"), UpdatedActor);
			}
		}
	}
	else if (GetFocusActor() == UpdatedActor)
	{
		bDoOnceHelp = true;
		AIState = EAIState::Search;
		ClearFocus(EAIFocusPriority::Gameplay);
		Attacker = nullptr;
		
		if (bDoOnceFight == false)
		{
			bDoOnceFight = true;
			Blackboard->SetValueAsObject(FName("TargetActor"), nullptr);
			Blackboard->SetValueAsBool(FName("Search"), true);
			Blackboard->SetValueAsBool(FName("SearchForSound"), false);
			Blackboard->SetValueAsBool(FName("SearchForEnemy"), true);
		}
	
		if (WeaponState != EWeaponState::Reloading)
		{
			ControlledPawn->UseWeapon(false, false);
		}

		// Only predict if AI is not taking cover
		if (Blackboard->GetValueAsBool(FName("TakeCover")) == false)
		{
			UAISense_Prediction::RequestControllerPredictionEvent(this, UpdatedActor, 1.0f);
		}
	}
}

void AShooterAIController::HandleDamage(AActor* UpdatedActor, const FAIStimulus& Stimulus)
{
	// Start searching
	AIState = EAIState::Search;
	Blackboard->SetValueAsBool(FName("SearchForSound"), false);
	Blackboard->SetValueAsBool(FName("SearchForEnemy"), false);
	Blackboard->SetValueAsBool(FName("Urgent"), true);
	Blackboard->SetValueAsBool(FName("Search"), true);
	Blackboard->SetValueAsVector(FName("TargetLocation"), Stimulus.StimulusLocation);
	
	// If the Updated actor is a new enemy, check which one is closer then if the new one is closer set it as attacker
	AActor* TargetActor;
	FindNearestOfTwoActor(GetFocusActor(), UpdatedActor, ControlledPawn->GetActorLocation(), TargetActor);
	GetFocusActor() != TargetActor ? SetFocus(UpdatedActor) : SetFocus(TargetActor);
	Blackboard->SetValueAsObject(FName("TargetActor"), TargetActor);
	Attacker = TargetActor;
}

void AShooterAIController::HandleHearing(const FAIStimulus& Stimulus)
{
	// Check if the updated actor is reachable
	const UNavigationPath* NavPath = UNavigationSystemV1::FindPathToLocationSynchronously(GetWorld(), ControlledPawn->GetActorLocation(), Stimulus.StimulusLocation);
	
	if (NavPath && NavPath->IsValid() && NavPath->IsPartial() == false)
	{
		AIState = EAIState::Search;
		Blackboard->SetValueAsVector(FName("TargetLocation"), Stimulus.StimulusLocation);
		Blackboard->SetValueAsBool(FName("Urgent"), false);
		Blackboard->SetValueAsBool(FName("SearchForSound"), true);
		Blackboard->SetValueAsBool(FName("Search"), true);
		
		switch (AIState)
		{
			case EAIState::Idle: case EAIState::Fight: case EAIState::Search:
				switch (WeaponState)
				{
					case EWeaponState::Idle: case EWeaponState::Firing: case EWeaponState::BetterToReload: case EWeaponState::CancelReload: case EWeaponState::Reloaded: case EWeaponState::AmmoAdded:
						ControlledPawn->UseWeapon(true, false);
						break;
					case EWeaponState::NeedToReload:
						SetWeaponState_Implementation(FAmmoComponentInfo(), EWeaponState::NeedToReload);
						break;
					case EWeaponState::Empty:
						SwitchWeapon();
						break;
					default:
						break;
				}
				break;
			default:
				break;
		}
	}
	// take cover if enemy is unreachable
	else if (Blackboard->GetValueAsBool(FName("TakeCover")) == false)
	{
		AIState = EAIState::Idle;
		Blackboard->SetValueAsBool(FName("Search"), false);
		Blackboard->SetValueAsBool(FName("SearchForEnemy"), false);
		Blackboard->SetValueAsBool(FName("SearchForSound"), false);
		Blackboard->SetValueAsBool(FName("TakeCover"), true);
		Blackboard->SetValueAsFloat(FName("WaitTime"), 5.0f);
		GetWorld()->GetTimerManager().SetTimer(BackToRoutineTimer, this, &AShooterAIController::BackToRoutine, 5.0f);
	}
}

void AShooterAIController::HandlePrediction(const FAIStimulus& Stimulus) const
{
	Blackboard->SetValueAsVector(FName("TargetLocation"), Stimulus.StimulusLocation);
}

void AShooterAIController::HandleTeam(const AActor* UpdatedActor)
{
	const AAIController* AIC = Cast<AAIController>(UpdatedActor->GetInstigatorController());
	if (AIC && AIC->GetFocusActor())
	{
		Blackboard->SetValueAsBool(FName("TakeCover"), false);
		Blackboard->SetValueAsVector(FName("TargetLocation"), AIC->GetFocusActor()->GetActorLocation());
		Blackboard->SetValueAsBool(FName("Urgent"), true);
		Blackboard->SetValueAsBool(FName("SearchForSound"), false);
		Blackboard->SetValueAsBool(FName("SearchForEnemy"), true);
		Blackboard->SetValueAsBool(FName("Search"), true);
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
		Blackboard->SetValueAsBool(FName("TakeCover"), false);
		Blackboard->SetValueAsFloat(FName("WaitTime"), 0.0f);
		
		Blackboard->SetValueAsObject(FName("TargetActor"), nullptr);
		Blackboard->SetValueAsBool(FName("Search"), false);
	}
}

// TODO - Explain it
void AShooterAIController::Fight()
{
	switch (AIState)
	{
		case EAIState::Idle: case EAIState::Fight: case EAIState::Search:
			if (Attacker)
			{
				TryToUseWeapon();
			}
			break;
		default:
			break;
	}
}

void AShooterAIController::TryToUseWeapon()
{
	switch (WeaponState)
	{
		case EWeaponState::Idle: case EWeaponState::BetterToReload: case EWeaponState::CancelReload: case EWeaponState::Reloaded: case EWeaponState::AmmoAdded:
			ControlledPawn->UseWeapon(true, true);
			break;
		case EWeaponState::NeedToReload:
		{
			ControlledPawn->UseWeapon(false, false);
			const FAmmoComponentInfo AmmoComponentInfo;
			SetWeaponState_Implementation(AmmoComponentInfo, EWeaponState::NeedToReload);
		}
			break;
		case EWeaponState::Reloading:
			ControlledPawn->UseWeapon(false, false);
			break;
		case EWeaponState::Empty:
			SwitchWeapon();
			break;
		default:
			break;
	}
}

void AShooterAIController::SetWeaponState_Implementation(FAmmoComponentInfo AmmoComponentInfo, EWeaponState NewWeaponState)
{
	WeaponState = NewWeaponState;
	switch (NewWeaponState)
	{
		case EWeaponState::Idle:
			if (AmmoComponentInfo.bNoAmmoLeftToReload && ControlledPawn->CurrentWeapon->AmmoComponent->CurrentMagazineAmmo == 0)
			{
				SwitchWeapon();
			}
			break;
		case EWeaponState::BetterToReload:
			// If there is no threat then reload the weapon
			if (!Attacker && !Blackboard->GetValueAsObject(FName("TargetActor")) && AIState != EAIState::Reload && AIState != EAIState::Switch)
			{
				TryToReload(AmmoComponentInfo.bNoAmmoLeftToReload);
			}
			break;
		case EWeaponState::NeedToReload:
			if (AIState != EAIState::Reload && AIState != EAIState::Switch)
			{
				TryToReload(AmmoComponentInfo.bNoAmmoLeftToReload);
			}
			break;
		case EWeaponState::Reloading:
			// If AI is in combat and there is an attacker and half of the mag is already full then stop reloading and continue fighting
			if (Attacker && ControlledPawn->CurrentWeapon->AmmoComponent->CurrentMagazineAmmo > ControlledPawn->CurrentWeapon->AmmoComponent->MagazineSize / 2)
			{
				Fight();
			}
			break;
		case EWeaponState::CancelReload: case EWeaponState::Reloaded: case EWeaponState::AmmoAdded:
			// Resume fighting after reload finished
			Fight();
			break;
		case EWeaponState::Empty:
			if (AIState != EAIState::Switch)
			{
				SwitchWeapon();
			}
			break;
		default:
			break;
	}
}

void AShooterAIController::SwitchWeapon()
{
	FTimerDelegate TimerDelegate;
	TimerDelegate.BindUObject(this, &AShooterAIController::Surrender);

	// If there is no weapon to switch then surrender
	switch (ControlledPawn->CurrentWeapon ? ControlledPawn->CurrentWeapon->CurrentSocket : EWeaponToDo::NoWeapon)
	{
		case EWeaponToDo::NoWeapon:
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
		case EWeaponToDo::PrimaryWeapon:
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
		case EWeaponToDo::SecondaryWeapon:
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
		case EWeaponToDo::SidearmWeapon:
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

	if (ControlledPawn->CurrentWeapon)
	{
		MaxFiringDistance = ControlledPawn->CurrentWeapon->WeaponInfo.EffectiveRange;
	}
	else
	{
		MaxFiringDistance = -1.0f;
	}
}

void AShooterAIController::TryToReload(bool bNoAmmoLeftToReload)
{
	ControlledPawn->UseWeapon(false, false);
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
	if (NewAIState == EAIState::LowHealth)
	{
		ClearFocus(EAIFocusPriority::Gameplay);
		ControlledPawn->UseWeapon(false, false);
	}
}

float AShooterAIController::FindNearestOfTwoActor(AActor* Actor1, AActor* Actor2, const FVector& CurrentLocation, AActor*& CloserActor)
{
	float DistanceToActor;
	if (Actor1)
	{
		DistanceToActor = FVector::Distance(Actor1->GetActorLocation(), CurrentLocation);
		if (Actor2)
		{
			const float NewDistance = FVector::Distance(Actor2->GetActorLocation(), CurrentLocation);
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
		DistanceToActor = FVector::Distance(Actor2->GetActorLocation(), CurrentLocation);
		CloserActor = Actor2;
		return DistanceToActor;
	}
	
	CloserActor = nullptr;
	return 0.0f;
}

void AShooterAIController::AskForHelp() const
{
	UAISense_Hearing::ReportNoiseEvent(GetWorld(), ControlledPawn->GetActorLocation(), 1.0f, ControlledPawn, 0.0f, FName("Help"));
}

// TODO: Improve
void AShooterAIController::Surrender()
{
	/*ControlledPawn->DropCurrentObject();
	bIsDisarm = true;
	Blackboard->SetValueAsBool(FName("IsDisarm"), true);
	Blackboard->SetValueAsFloat(FName("WaitTime"), 60.0f);
	ClearFocus(EAIFocusPriority::Gameplay);
	ControlledPawn->GetCharacterMovement()->DisableMovement();
	ControlledPawn->PlayAnimMontage(ControlledPawn->SurrenderMontage);*/
}
