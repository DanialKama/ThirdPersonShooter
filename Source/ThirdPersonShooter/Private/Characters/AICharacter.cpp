// Copyright 2022-2023 Danial Kamali. All Rights Reserved.

#include "AICharacter.h"

#include "Actors/Interactable/PickupWeapon.h"
#include "Actors/NonInteractive/SpawnerAI.h"
#include "AI/ShooterAIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Components/HealthComponent.h"
#include "Components/WidgetComponent.h"
#include "Core/Interfaces/WidgetInterface.h"
#include "GameFramework/CharacterMovementComponent.h"

AAICharacter::AAICharacter()
{
	AutoPossessAI = EAutoPossessAI::Spawned;
	SpawnCollisionHandlingMethod = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;
	
	Widget = CreateDefaultSubobject<UWidgetComponent>(TEXT("Health Bar"));
	Widget->SetupAttachment(GetRootComponent());
	Widget->SetWidgetSpace(EWidgetSpace::Screen);
	Widget->SetGenerateOverlapEvents(false);
	Widget->CanCharacterStepUpOn = ECB_No;
	Widget->SetCollisionProfileName(UCollisionProfile::NoCollision_ProfileName);
	Widget->SetVisibility(false);

	GetCharacterMovement()->RotationRate = FRotator(0.0f, 180.0f, 0.0f);

	// Initialize variables
	bAIControllerInterface = false;
	bWidgetInterface = false;
}

void AAICharacter::BeginPlay()
{
	Super::BeginPlay();

	SetPrimaryWeapon();
	SetSidearmWeapon();
}

void AAICharacter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	if (NewController)
	{
		UE_LOG(LogTemp, Warning, TEXT("DEF AIC IS VALID!!!"));
	}
	
	AIController = Cast<AShooterAIController>(NewController);
	if (AIController && AIController->GetClass()->ImplementsInterface(UAIControllerInterface::StaticClass()))
	{
		bAIControllerInterface = true;
		
		AIBlackboard = AIController->GetBlackboardComponent();
		const float Health = GetHealthComponent()->DefaultHealth / GetHealthComponent()->MaxHealth;
		AIBlackboard->SetValueAsFloat("Health", Health);
		
		Widget->InitWidget();
		if (Widget->GetWidget() && Widget->GetWidget()->GetClass()->ImplementsInterface(UWidgetInterface::StaticClass()))
		{
			IWidgetInterface::Execute_UpdateActorHealth(Widget->GetWidget(), Health);
			bWidgetInterface = true;
		}
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("CUSTOM AIC IS NOT VALID!!!"));
	}
}

void AAICharacter::SetPrimaryWeapon()
{
	if (PrimaryWeapons.Num() > 0)
	{
		const TSubclassOf<APickupWeapon> WeaponToSpawn = PrimaryWeapons[FMath::RandRange(0, PrimaryWeapons.Num() - 1)];
		const FVector Location = GetMesh()->GetSocketLocation("RightHandHoldSocket");
		const FRotator Rotation = GetMesh()->GetSocketRotation("RightHandHoldSocket");
		
		FActorSpawnParameters SpawnParameters;
		SpawnParameters.Owner = this;
		SpawnParameters.Instigator = GetInstigator();
		SpawnParameters.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
		
		if (APickupWeapon* NewWeapon = GetWorld()->SpawnActor<APickupWeapon>(WeaponToSpawn, Location, Rotation, SpawnParameters))
		{
			SetPickup_Implementation(EItemType::Weapon, NewWeapon);
			Interact_Implementation();

			AIController->MaxFiringDistance = NewWeapon->WeaponInfo.EffectiveRange;
		}
	}
}

void AAICharacter::SetSidearmWeapon()
{
	if (SidearmWeapons.Num() > 0)
	{
		const TSubclassOf<APickupWeapon> WeaponToSpawn = SidearmWeapons[FMath::RandRange(0, SidearmWeapons.Num() - 1)];
		const FVector Location = GetMesh()->GetSocketLocation("Weapon3Socket");
		const FRotator Rotation = GetMesh()->GetSocketRotation("Weapon3Socket");
		
		FActorSpawnParameters SpawnParameters;
		SpawnParameters.Owner = this;
		SpawnParameters.Instigator = GetInstigator();
		SpawnParameters.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
		
		if (APickupWeapon* NewWeapon = GetWorld()->SpawnActor<APickupWeapon>(WeaponToSpawn, Location, Rotation, SpawnParameters))
		{
			SetPickup_Implementation(EItemType::Weapon, NewWeapon);
			Interact_Implementation();

			/*if (AIController->MaxFiringDistance == -1.0f)
			{
				AIController->MaxFiringDistance = NewWeapon->WeaponInfo.EffectiveRange;
			}*/
		}
	}
}

void AAICharacter::ReloadWeapon()
{
	// Stop AI movement while reloading to not interrupt reloading montage
	GetCharacterMovement()->DisableMovement();
	if (bAIControllerInterface)
	{
		IAIControllerInterface::Execute_SetAIState(AIController, EAIState::Reload);
	}
	
	Super::ReloadWeapon();
}

void AAICharacter::ResetReload()
{
	Super::ResetReload();
	
	FTimerDelegate TimerDelegate;
	TimerDelegate.BindUObject(this, &AAICharacter::TryToResetMovement);
	GetWorld()->GetTimerManager().SetTimerForNextTick(TimerDelegate);
}

void AAICharacter::SwitchToPrimary()
{
	// Stop AI movement while switching to not interrupt montages
	GetCharacterMovement()->DisableMovement();
	if (bAIControllerInterface)
	{
		IAIControllerInterface::Execute_SetAIState(AIController, EAIState::Switch);
	}
	
	Super::SwitchToPrimary();
}

void AAICharacter::SwitchToSecondary()
{
	GetCharacterMovement()->DisableMovement();
	if (bAIControllerInterface)
	{
		IAIControllerInterface::Execute_SetAIState(AIController, EAIState::Switch);
	}
	
	Super::SwitchToSecondary();
}

void AAICharacter::SwitchToSidearm()
{
	GetCharacterMovement()->DisableMovement();
	if (bAIControllerInterface)
	{
		IAIControllerInterface::Execute_SetAIState(AIController, EAIState::Switch);
	}
	
	Super::SwitchToSidearm();
}

void AAICharacter::HolsterWeapon()
{
	GetCharacterMovement()->DisableMovement();
	if (bAIControllerInterface)
	{
		IAIControllerInterface::Execute_SetAIState(AIController, EAIState::Switch);
	}
	
	Super::HolsterWeapon();
}

void AAICharacter::SwitchIsEnded()
{
	Super::SwitchIsEnded();

	FTimerDelegate TimerDelegate;
	TimerDelegate.BindUObject(this, &AAICharacter::TryToResetMovement);
	GetWorld()->GetTimerManager().SetTimerForNextTick(TimerDelegate);
}

void AAICharacter::TryToResetMovement() const
{
	GetCharacterMovement()->SetMovementMode(MOVE_Walking);
	if (bAIControllerInterface)
	{
		IAIControllerInterface::Execute_SetAIState(AIController, EAIState::Idle);
	}
}

float AAICharacter::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);
	
	if (bIsAlive && DamageCauser->GetOwner()->GetClass()->ImplementsInterface(UCommonInterface::StaticClass()))
	{
		if (Execute_GetTeamTag(DamageCauser->GetOwner()) != TeamTag && Execute_IsPlayer(DamageCauser->GetOwner()))
		{
			Widget->SetVisibility(true);
			GetWorld()->GetTimerManager().ClearTimer(WidgetTimerHandle);
			GetWorld()->GetTimerManager().SetTimer(WidgetTimerHandle, this, &AAICharacter::HideWidget, 2.0f);
		}
	}
	
	return DamageAmount;
}

void AAICharacter::HideWidget() const
{
	Widget->SetVisibility(false);	
}

void AAICharacter::UseWeapon(const bool bAim, const bool bFire)
{
	if (bAim)
	{
		if (CurrentWeapon && SetAimState(true))
		{
			if (bFire)
			{
				FTimerHandle TimerHandle;
				GetWorld()->GetTimerManager().SetTimer(TimerHandle, this, &ABaseCharacter::StartFireWeapon, 0.75f);
			}
			else
			{
				StopFireWeapon();
			}
		}
	}
	else
	{
		StopFireWeapon();
		SetAimState(false);
	}
}

void AAICharacter::SetHealthLevel_Implementation(float Health)
{
	// AIController->GetBlackboardComponent()->SetValueAsFloat("Health", Health);
	if (bWidgetInterface)
	{
		IWidgetInterface::Execute_UpdateActorHealth(Widget->GetWidget(), Health);
	}
}

void AAICharacter::SetHealthState_Implementation(EHealthState HealthState)
{
	switch (HealthState)
	{
		case EHealthState::Full: case EHealthState::RecoveryStarted: case EHealthState::RecoveryStopped:
			Super::SetHealthState_Implementation(HealthState);
			break;
		case EHealthState::Low:
			if (bAIControllerInterface)
			{
				// If health is low, report it to the controller to start taking cover and using meds
				IAIControllerInterface::Execute_SetAIState(AIController, EAIState::LowHealth);
			}
			break;
		case EHealthState::Death:
			Super::SetHealthState_Implementation(HealthState);
			
			Widget->SetVisibility(false);
			
			if (Spawner)
			{
				Spawner->EnterSpawnQueue(FSpawnData(RespawnDelay, StaticClass()));
			}
			break;
	}
}

void AAICharacter::HealingMontageHandler(UAnimMontage* AnimMontage, bool bInterrupted) const
{
	Super::HealingMontageHandler(AnimMontage, bInterrupted);

	if (AIController)
	{
		AIController->SetAIState_Implementation(EAIState::LowHealth);
		GetCharacterMovement()->SetMovementMode(MOVE_Walking);
	}
}
