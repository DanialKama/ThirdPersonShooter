// All Rights Reserved.

#include "Characters/AICharacter.h"
#include "Actors/PickupWeapon.h"
#include "Actors/RespawnActor.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Components/HealthComponent.h"
#include "Components/WidgetComponent.h"
#include "Core/AI/ShooterAIController.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Interfaces/WidgetInterface.h"

AAICharacter::AAICharacter()
{
	// Create components
	Widget = CreateDefaultSubobject<UWidgetComponent>(TEXT("Health Bar"));

	// Attach components
	Widget->SetupAttachment(GetRootComponent());

	// Initialize components
	Widget->SetComponentTickEnabled(false);
	Widget->SetWidgetSpace(EWidgetSpace::Screen);
	Widget->SetGenerateOverlapEvents(false);
	Widget->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	Widget->SetCollisionResponseToAllChannels(ECR_Ignore);
	Widget->SetVisibility(false);

	GetCharacterMovement()->RotationRate = FRotator(0.0f, 180.0f, 0.0f);
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;
}

void AAICharacter::BeginPlay()
{
	Super::BeginPlay();

	SetPrimaryWeapon();
	FTimerDelegate TimerDelegate;
	TimerDelegate.BindUObject(this, &AAICharacter::SetSidearmWeapon);
	GetWorld()->GetTimerManager().SetTimerForNextTick(TimerDelegate);
}

void AAICharacter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);
	
	AIController = Cast<AShooterAIController>(NewController);
	if (AIController->GetClass()->ImplementsInterface(UAIControllerInterface::StaticClass()))
	{
		bAIControllerInterface = true;

		AIBlackboard = AIController->GetBlackboardComponent();
		const float Health = HealthComponent->DefaultHealth / HealthComponent->MaxHealth;
		AIBlackboard->SetValueAsFloat(FName("Health"), Health);

		Widget->InitWidget();
		if (Widget->GetWidget() && Widget->GetWidget()->GetClass()->ImplementsInterface(UWidgetInterface::StaticClass()))
		{
			IWidgetInterface::Execute_UpdateActorHealth(Widget->GetWidget(), Health);
			bWidgetInterface = true;
		}
	}
}

void AAICharacter::SetPrimaryWeapon()
{
	if (PrimaryWeapons.Num() > 0)
	{
		const TSubclassOf<APickupWeapon> WeaponToSpawn = PrimaryWeapons[FMath::RandRange(0, PrimaryWeapons.Num() - 1)];
		const FVector Location = GetMesh()->GetSocketLocation(FName("RightHandHoldSocket"));
		const FRotator Rotation = GetMesh()->GetSocketRotation(FName("RightHandHoldSocket"));
		FActorSpawnParameters SpawnParameters;
		SpawnParameters.Owner = this;
		SpawnParameters.Instigator = GetInstigator();
		SpawnParameters.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
		APickupWeapon* NewWeapon = GetWorld()->SpawnActor<APickupWeapon>(WeaponToSpawn, Location, Rotation, SpawnParameters);
		SetPickup_Implementation(EItemType::Weapon, NewWeapon);
		Interact_Implementation();
	}
}

void AAICharacter::SetSidearmWeapon()
{
	if (SidearmWeapons.Num() > 0)
	{
		const TSubclassOf<APickupWeapon> WeaponToSpawn = SidearmWeapons[FMath::RandRange(0, SidearmWeapons.Num() - 1)];
		const FVector Location = GetMesh()->GetSocketLocation(FName("Weapon3Socket"));
		const FRotator Rotation = GetMesh()->GetSocketRotation(FName("Weapon3Socket"));
		FActorSpawnParameters SpawnParameters;
		SpawnParameters.Owner = this;
		SpawnParameters.Instigator = GetInstigator();
		SpawnParameters.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
		APickupWeapon* NewWeapon = GetWorld()->SpawnActor<APickupWeapon>(WeaponToSpawn, Location, Rotation, SpawnParameters);
		SetPickup_Implementation(EItemType::Weapon, NewWeapon);
		Interact_Implementation();
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

void AAICharacter::UseWeapon(bool bAim, bool bFire)
{
	if (bAim)
	{
		if (CurrentHoldingWeapon != EWeaponToDo::NoWeapon)
		{
			if (SetAimState(true))
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
	}
	else
	{
		StopFireWeapon();
		SetAimState(false);
	}
}

void AAICharacter::SetHealthLevel_Implementation(float Health)
{
	AIBlackboard->SetValueAsFloat(FName("Health"), Health);
	if (bWidgetInterface)
	{
		IWidgetInterface::Execute_UpdateActorHealth(Widget->GetWidget(), Health);
	}
}

void AAICharacter::SetHealthState_Implementation(EHealthState HealthState)
{
	switch (HealthState)
	{
	case 0:
		// Full
		Super::SetHealthState_Implementation(HealthState);
		break;
	case 1:
		// Low
		if (bAIControllerInterface)
		{
			// If health is low, report it to the controller to start taking cover and using meds
			IAIControllerInterface::Execute_SetAIState(AIController, EAIState::LowHealth);
		}
		break;
	case 2:
		// Recovery Started
		Super::SetHealthState_Implementation(HealthState);
		break;
	case 3:
		// Recovery Ended
		Super::SetHealthState_Implementation(HealthState);
		break;
	case 4:
		// Death
		Super::SetHealthState_Implementation(HealthState);
		Widget->SetVisibility(false);
		if (RespawnHandler)
		{
			FRespawnInfo RespawnInfo;
			RespawnInfo.SpawnTime = RespawnTime;
			RespawnInfo.CharacterToSpawn = this->GetClass();
			RespawnHandler->EnterRespawnQueue(RespawnInfo);
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

APatrolPathActor* AAICharacter::GetPatrolPath_Implementation()
{
	return PatrolPath;	
}