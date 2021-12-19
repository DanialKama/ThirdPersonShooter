// All Rights Reserved.

#include "Characters/AICharacter.h"
#include "AIController.h"
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
	
	Blackboard = UAIBlueprintHelperLibrary::GetBlackboard(this);
	Blackboard->SetValueAsFloat(FName("Health"), Health);
	
	SetPrimaryWeapon();
	FTimerDelegate TimerDelegate;
	TimerDelegate.BindUObject(this, &AAICharacter::SetSidearmWeapon);
	GetWorld()->GetTimerManager().SetTimerForNextTick(TimerDelegate);

}

void AAICharacter::SetPrimaryWeapon()
{
}

void AAICharacter::SetSidearmWeapon()
{
	
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
	
}

float AAICharacter::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	if (bIsAlive && Execute_IsPlayer(DamageCauser->GetOwner()) && Execute_GetTeamTag(DamageCauser->GetOwner()) != TeamTag)
	{
		Widget->SetVisibility(true);
		FTimerHandle TimerHandle;
		GetWorld()->GetTimerManager().SetTimer(TimerHandle, this, &AAICharacter::HideWidget, 2.0f);
		return DamageAmount;
	}
	return DamageAmount;
}

void AAICharacter::HideWidget() const
{
	Widget->SetVisibility(false);	
}

void AAICharacter::UseWeapon_Implementation(bool bAim, bool bFire)
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
	if (bWidgetInterface)
	{
		IWidgetInterface::Execute_UpdateActorHealth(Widget->GetWidget(), Health);
	}
	if (Blackboard)
	{
		Blackboard->SetValueAsFloat(FName("Health"), Health);
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