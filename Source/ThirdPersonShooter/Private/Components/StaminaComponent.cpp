// Copyright 2022-2023 Danial Kamali. All Rights Reserved.

#include "StaminaComponent.h"

#include "Characters/BaseCharacter.h"
#include "Core/Interfaces/CharacterInterface.h"

UStaminaComponent::UStaminaComponent()
{
	PrimaryComponentTick.bStartWithTickEnabled = false;

	// Initialize variables
	bCharacterInterface = false;
	bDoOnce = true;
}

void UStaminaComponent::Activate(bool bReset)
{
	Super::Activate(bReset);

	CurrentStamina = FMath::Clamp(DefaultStamina, 0.0f, MaxStamina);

	// Detected if the interfaces is present on owner
	if (GetOwner()->GetClass()->ImplementsInterface(UCharacterInterface::StaticClass()))
	{
		bCharacterInterface = true;
		ICharacterInterface::Execute_SetStaminaLevel(GetOwner(), CurrentStamina / MaxStamina, CurrentStamina == MaxStamina);
	}
}

void UStaminaComponent::Deactivate()
{
	Super::Deactivate();

	FTimerManager& TimerManager = GetWorld()->GetTimerManager();
	TimerManager.ClearTimer(RestoreTimer);
	TimerManager.ClearTimer(RunningTimer);
	TimerManager.ClearTimer(SprintingTimer);
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
			ABaseCharacter* OwnerCharacter = GetOwner<ABaseCharacter>();
			OwnerCharacter->SetMovementState(OwnerCharacter->WalkSpeed, OwnerCharacter->WalkJumpZVelocity);
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
			ABaseCharacter* OwnerCharacter = GetOwner<ABaseCharacter>();
			OwnerCharacter->SetMovementState(OwnerCharacter->WalkSpeed, OwnerCharacter->WalkJumpZVelocity);
		}
	}
}
