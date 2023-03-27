// Copyright 2022-2023 Danial Kamali. All Rights Reserved.

#include "HealthComponent.h"
#include "Core/Interfaces/CharacterInterface.h"
#include "Core/Interfaces/CommonInterface.h"

UHealthComponent::UHealthComponent()
{
	PrimaryComponentTick.bCanEverTick = false;

	// Initialize variables
	DefaultHealth = MaxHealth = 100.0f;
	LowHealth = 25.0f;
	HealingAmount = 10.0f;
	HealthRecoveryRate = 0.25f;
	StartHealthRecoveryDelay = 3.0f;
	CurrentHealth = 0.0f;
	HitBoneName = FName("None");
	ShotOrigin = FVector::ZeroVector;
	bCanRecoverHealth = true;
	bCommonInterface = false;
	bCharacterInterface = false;
}

void UHealthComponent::Initialize()
{
	Super::Initialize();
	
	if (GetOwner())
	{
		GetOwner()->OnTakeAnyDamage.AddDynamic(this, &UHealthComponent::TakeAnyDamage);
		GetOwner()->OnTakePointDamage.AddDynamic(this, &UHealthComponent::TakePointDamage);
		GetOwner()->OnTakeRadialDamage.AddDynamic(this, &UHealthComponent::TakeRadialDamage);

		// Detected if the interfaces is present on owner
		if (GetOwner()->GetClass()->ImplementsInterface(UCommonInterface::StaticClass()))
		{
			bCommonInterface = true;
		}
		if (GetOwner()->GetClass()->ImplementsInterface(UCharacterInterface::StaticClass()))
		{
			bCharacterInterface = true;
		}
	}

	CurrentHealth = FMath::Clamp(DefaultHealth, 0.0f, MaxHealth);

	if (bCharacterInterface)
	{
		ICharacterInterface::Execute_SetHealthLevel(GetOwner(), CurrentHealth / MaxHealth);
	}
}

void UHealthComponent::TakeAnyDamage(AActor* DamagedActor, float Damage, const UDamageType* DamageType, AController* InstigatedBy, AActor* DamageCauser)
{
	UpdateHealthOnDamage(Damage, TEXT("None"), GetOwner()->GetActorLocation());
}

void UHealthComponent::TakePointDamage(AActor* DamagedActor, float Damage, AController* InstigatedBy, FVector HitLocation, UPrimitiveComponent* FHitComponent, FName BoneName, FVector ShotFromDirection, const UDamageType* DamageType, AActor* DamageCauser)
{
	UpdateHealthOnDamage(Damage, BoneName, ShotFromDirection);
}

void UHealthComponent::TakeRadialDamage(AActor* DamagedActor, float Damage, const UDamageType* DamageType, FVector Origin, FHitResult HitInfo, AController* InstigatedBy, AActor* DamageCauser)
{
	UpdateHealthOnDamage(Damage, HitInfo.BoneName, Origin);
}

void UHealthComponent::UpdateHealthOnDamage(float Damage, FName BoneName, FVector ShotFromDirection)
{
	if (Damage <= 0)
	{
		return;
	}

	CurrentHealth = FMath::Clamp(CurrentHealth - Damage, 0.0f, MaxHealth);

	// Update current health on character
	if (bCharacterInterface)
	{
		ICharacterInterface::Execute_SetHealthLevel(GetOwner(), CurrentHealth / MaxHealth);
	}

	if (CurrentHealth <= 0.0f)
	{
		HitBoneName = BoneName;
		ShotOrigin = ShotFromDirection;

		if (bCommonInterface)
		{
			ICommonInterface::Execute_SetHealthState(GetOwner(), EHealthState::Death);
		}
	}
	else if (CurrentHealth < LowHealth && bCommonInterface)
	{
		ICommonInterface::Execute_SetHealthState(GetOwner(), EHealthState::RecoveryStarted);
	}
	
	FTimerHandle StartHealthRecoveryTimer;
	GetWorld()->GetTimerManager().SetTimer(StartHealthRecoveryTimer, this, &UHealthComponent::StartHealthRecovery, StartHealthRecoveryDelay);
}

void UHealthComponent::StartHealthRecovery()
{
	if (bCanRecoverHealth)
	{
		GetWorld()->GetTimerManager().SetTimer(HealthRecoveryTimer, this, &UHealthComponent::RecoverHealth, HealthRecoveryRate, true);
		if (bCommonInterface)
		{
			ICommonInterface::Execute_SetHealthState(GetOwner(), EHealthState::RecoveryStarted);
		}
	}
}

void UHealthComponent::StopHealthRecovery()
{
	GetWorld()->GetTimerManager().ClearTimer(HealthRecoveryTimer);
	if (bCommonInterface)
	{
		ICommonInterface::Execute_SetHealthState(GetOwner(), EHealthState::RecoveryStopped);
	}
}

void UHealthComponent::RecoverHealth()
{
	CurrentHealth = FMath::Clamp(HealingAmount + CurrentHealth, 0.0f, MaxHealth);

	if (bCharacterInterface)
	{
		ICharacterInterface::Execute_SetHealthLevel(GetOwner(), CurrentHealth / MaxHealth);
	}

	if (CurrentHealth >= MaxHealth)
	{
		GetWorld()->GetTimerManager().ClearTimer(HealthRecoveryTimer);
		if (bCommonInterface)
		{
			ICommonInterface::Execute_SetHealthState(GetOwner(), EHealthState::Full);
		}
	}
}

void UHealthComponent::Healing(float HealthDifference)
{
	CurrentHealth = FMath::Clamp(HealthDifference + CurrentHealth, 0.0f, MaxHealth);

	if (bCharacterInterface)
	{
		ICharacterInterface::Execute_SetHealthLevel(GetOwner(), CurrentHealth / MaxHealth);
	}

	if (CurrentHealth >= MaxHealth && bCommonInterface)
	{
		ICommonInterface::Execute_SetHealthState(GetOwner(), EHealthState::Full);
	}
}