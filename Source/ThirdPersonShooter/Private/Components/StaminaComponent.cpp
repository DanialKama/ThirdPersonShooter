// Copyright 2022 Danial Kamali. All Rights Reserved.

#include "Components/StaminaComponent.h"
#include "Interfaces/CharacterInterface.h"

UStaminaComponent::UStaminaComponent()
{
	PrimaryComponentTick.bCanEverTick = false;

	// Initialize variables
	DefaultStamina = MaxStamina = 100.0f;
	RestoreStaminaAmount = 25.0f;
	StaminaRestoreRate = 1.0f;
	StartStaminaRecoveryDelay = 5.0f;
	RunStaminaDrainAmount = 1.0f;
	SprintStaminaDrainAmount = 3.0f;
	CurrentStamina = 0.0f;
	bCharacterInterface = false;
	bDoOnce = true;
}

void UStaminaComponent::Initialize()
{
	Super::Initialize();

	CurrentStamina = FMath::Clamp(DefaultStamina, 0.0f, MaxStamina);

	if (GetOwner())
	{
		// Detected if the interfaces is present on owner
		if (GetOwner()->GetClass()->ImplementsInterface(UCharacterInterface::StaticClass()))
		{
			bCharacterInterface = true;

			ICharacterInterface::Execute_SetStaminaLevel(GetOwner(), CurrentStamina / MaxStamina, CurrentStamina == MaxStamina);
		}
	}
}

void UStaminaComponent::StopStaminaDrain()
{
	GetWorld()->GetTimerManager().ClearTimer(RunningTimer);
	GetWorld()->GetTimerManager().ClearTimer(SprintingTimer);
	
	if (bDoOnce)
	{
		GetWorld()->GetTimerManager().SetTimer(RestoreTimer, this, &UStaminaComponent::RestoreStamina, StaminaRestoreRate, true, StartStaminaRecoveryDelay);
		bDoOnce = false;
	}
}

void UStaminaComponent::StartRunning()
{
	if (GetOwner()->GetVelocity().Size() > 0)
	{
		GetWorld()->GetTimerManager().ClearTimer(RestoreTimer);
		GetWorld()->GetTimerManager().ClearTimer(SprintingTimer);
		bDoOnce = true;
		
		GetWorld()->GetTimerManager().SetTimer(RunningTimer, this, &UStaminaComponent::RunningDrainStamina, 0.1f, true);
		
		if (bCharacterInterface)
		{
			ICharacterInterface::Execute_SetStaminaLevel(GetOwner(), CurrentStamina / MaxStamina, false);
		}
	}
}

void UStaminaComponent::StartSprinting()
{
	if (GetOwner()->GetVelocity().Size() > 0)
	{
		GetWorld()->GetTimerManager().ClearTimer(RestoreTimer);
		GetWorld()->GetTimerManager().ClearTimer(RunningTimer);
		bDoOnce = true;
		
		GetWorld()->GetTimerManager().SetTimer(SprintingTimer, this, &UStaminaComponent::SprintingDrainStamina, 0.1f, true);
		if (bCharacterInterface)
		{
			ICharacterInterface::Execute_SetStaminaLevel(GetOwner(), CurrentStamina / MaxStamina, false);
		}
	}
}

void UStaminaComponent::RestoreStamina()
{
	CurrentStamina = FMath::Clamp(CurrentStamina + RestoreStaminaAmount, 0.0f, MaxStamina);

	const bool bIsStaminaFull = CurrentStamina == MaxStamina;

	if (bCharacterInterface)
	{
		ICharacterInterface::Execute_SetStaminaLevel(GetOwner(), CurrentStamina / MaxStamina, bIsStaminaFull);
	}

	// If stamina is fully restored then stop the timer
	if (bIsStaminaFull)
	{
		GetWorld()->GetTimerManager().ClearTimer(RestoreTimer);
	}	
}

void UStaminaComponent::RunningDrainStamina()
{
	CurrentStamina = FMath::Clamp(CurrentStamina - RunStaminaDrainAmount, 0.0f, MaxStamina);

	if (bCharacterInterface)
	{
		ICharacterInterface::Execute_SetStaminaLevel(GetOwner(), CurrentStamina / MaxStamina, false);

		// If stamina is zero set owner movement to walk
		if (CurrentStamina <= 0.0f)
		{
			ICharacterInterface::Execute_SetMovementState(GetOwner(), EMovementState::Walk, false, false);
		}
	}
}

void UStaminaComponent::SprintingDrainStamina()
{
	CurrentStamina = FMath::Clamp(CurrentStamina - SprintStaminaDrainAmount, 0.0f, MaxStamina);

	if (bCharacterInterface)
	{
		ICharacterInterface::Execute_SetStaminaLevel(GetOwner(), CurrentStamina / MaxStamina, false);
		
		// If stamina is zero set owner movement to walk
		if (CurrentStamina <= 0.0f)
		{
			ICharacterInterface::Execute_SetMovementState(GetOwner(), EMovementState::Walk, false, false);
		}
	}
}