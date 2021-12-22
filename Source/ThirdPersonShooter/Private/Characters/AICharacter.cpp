// All Rights Reserved.

#include "Characters/AICharacter.h"
#include "AIController.h"
#include "Actors/PickupWeapon.h"
#include "Actors/RespawnActor.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Blueprint/AIBlueprintHelperLibrary.h"
#include "Components/HealthComponent.h"
#include "Components/WidgetComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Interfaces/AIControllerInterface.h"
#include "Interfaces/WidgetInterface.h"

// Sets default values
AAICharacter::AAICharacter()
{
	// Create components
	Widget = CreateDefaultSubobject<UWidgetComponent>(TEXT("Health Bar"));

	// Setup components attachment
	Widget->SetupAttachment(GetRootComponent());

	// Initialize components
	Widget->SetWidgetSpace(EWidgetSpace::Screen);
	Widget->SetGenerateOverlapEvents(false);
	Widget->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	Widget->SetCollisionResponseToAllChannels(ECR_Ignore);
	Widget->SetVisibility(false);

	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;
}

void AAICharacter::BeginPlay()
{
	Super::BeginPlay();

	const float Health = HealthComponent->DefaultHealth / HealthComponent->MaxHealth;
	AIController = Cast<AAIController>(GetController());
	if (AIController->GetClass()->ImplementsInterface(UAIControllerInterface::StaticClass()))
	{
		bAIControllerInterface = true;
		if (PatrolPath)
		{
			IAIControllerInterface::Execute_StartPatrol(AIController);
		}
	}
	if (Widget->GetWidget()->GetClass()->ImplementsInterface(UWidgetInterface::StaticClass()))
	{
		IWidgetInterface::Execute_UpdateActorHealth(Widget->GetWidget(), Health);
		bWidgetInterface = true;
	}
	
	ImplementedBlackboard = UAIBlueprintHelperLibrary::GetBlackboard(this);
	ImplementedBlackboard->SetValueAsFloat(FName("Health"), Health);
	
	SetPrimaryWeapon();
	FTimerDelegate TimerDelegate;
	TimerDelegate.BindUObject(this, &AAICharacter::SetSidearmWeapon);
	GetWorld()->GetTimerManager().SetTimerForNextTick(TimerDelegate);

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
	Super::SwitchToPrimary();
}

void AAICharacter::SwitchToSecondary()
{
	GetCharacterMovement()->DisableMovement();
	Super::SwitchToSecondary();
}

void AAICharacter::SwitchToSidearm()
{
	GetCharacterMovement()->DisableMovement();
	Super::SwitchToSidearm();
}

void AAICharacter::HolsterWeapon()
{
	GetCharacterMovement()->DisableMovement();
	Super::HolsterWeapon();
}

void AAICharacter::SwitchIsEnded()
{
	Super::SwitchIsEnded();
	FTimerDelegate TimerDelegate;
	TimerDelegate.BindUObject(this, &AAICharacter::TryToResetMovement);
}

void AAICharacter::TryToResetMovement() const
{
	GetCharacterMovement()->SetMovementMode(MOVE_Walking);
}

float AAICharacter::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);
	
	if (bIsAlive &&  Execute_GetTeamTag(DamageCauser->GetOwner()) != TeamTag)
	{
		if (DamageCauser->GetOwner()->GetClass()->ImplementsInterface(UCommonInterface::StaticClass()) && ICommonInterface::Execute_IsPlayer(DamageCauser->GetOwner()))
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

bool AAICharacter::SwitchToWeapon(bool SwitchToAvailable, EWeaponToDo WeaponToSwitch)
{
	if (WeaponToSwitch != CurrentHoldingWeapon)
	{
		switch (WeaponToSwitch)
		{
		case 0:
			// No Weapon
			HolsterWeapon();
			return true;
		case 1:
			// Primary Weapon
			if (PrimaryWeapon)
			{
				SwitchToPrimary();
				return true;
			}
			if (SwitchToAvailable)
			{
				if (SecondaryWeapon)
				{
					SwitchToSecondary();
					return true;
				}
				if (SidearmWeapon)
				{
					SwitchToSidearm();
					return true;
				}
			}
			return false;
		case 2:
			// Secondary Weapon
			if (SecondaryWeapon)
			{
				SwitchToSecondary();
				return true;
			}
			if (SwitchToAvailable)
			{
				if (PrimaryWeapon)
				{
					SwitchToPrimary();
					return true;
				}
				if (SidearmWeapon)
				{
					SwitchToSidearm();
					return true;
				}
			}
			return false;
		case 3:
			// Sidearm Weapon
			if (SidearmWeapon)
			{
				SwitchToSidearm();
				return true;
			}
			if (SwitchToAvailable)
			{
				if (PrimaryWeapon)
				{
					SwitchToPrimary();
					return true;
				}
				if (SecondaryWeapon)
				{
					SwitchToSecondary();
					return true;
				}
			}
			return false;
		}
		return false;
	}
	return true;
}

void AAICharacter::SetHealthLevel_Implementation(float Health)
{
	if (bWidgetInterface)
	{
		IWidgetInterface::Execute_UpdateActorHealth(Widget->GetWidget(), Health);
	}
	if (ImplementedBlackboard)
	{
		ImplementedBlackboard->SetValueAsFloat(FName("Health"), Health);
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
			// If health is low, report health state to the controller to start taking cover and using meds
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
			RespawnHandler->StartRespawn(RespawnInfo);
		}
		break;
	}
}

AAICharacter* AAICharacter::GetAICharacterReference_Implementation()
{
	return this;
}

APatrolPathActor* AAICharacter::GetPatrolPath_Implementation()
{
	return PatrolPath;	
}