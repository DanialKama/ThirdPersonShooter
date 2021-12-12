// All Rights Reserved.

#include "Components/HealthComponent.h"
#include "Interfaces/CharacterInterface.h"
#include "Interfaces/CommonInterface.h"
#include "Kismet/GameplayStatics.h"

// Sets default values for this component's properties
UHealthComponent::UHealthComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	// Initialize variables
	bCanRecoverHealth = true;
}

void UHealthComponent::SetupComponent()
{
	Super::SetupComponent();
	
	if(Owner)
	{
		Owner->OnTakeAnyDamage.AddDynamic(this, &UHealthComponent::TakeAnyDamage);
		Owner->OnTakePointDamage.AddDynamic(this, &UHealthComponent::TakePointDamage);
		Owner->OnTakeRadialDamage.AddDynamic(this, &UHealthComponent::TakeRadialDamage);

		// Detected if the interfaces is present on owner
		if(Owner->GetClass()->ImplementsInterface(UCommonInterface::StaticClass()))
		{
			bCommonInterface = true;
		}
		if(Owner->GetClass()->ImplementsInterface(UCharacterInterface::StaticClass()))
		{
			bCharacterInterface = true;
		}
	}

	CurrentHealth = FMath::Clamp(DefaultHealth, 0.0f, MaxHealth);

	if(bCharacterInterface)
	{
		ICharacterInterface::Execute_SetHealthLevel(Owner, CurrentHealth / MaxHealth);
	}
}

void UHealthComponent::TakeAnyDamage(AActor* DamagedActor, float Damage, const class UDamageType* DamageType, class AController* InstigatedBy, AActor* DamageCauser)
{
	UpdateHealthOnDamage(Damage, TEXT("None"), Owner->GetActorLocation());
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
	if(Damage <= 0)
	{
		return;
	}

	CurrentHealth = FMath::Clamp(CurrentHealth - Damage, 0.0f, MaxHealth);

	// Update current health on character
	if(bCharacterInterface)
	{
		ICharacterInterface::Execute_SetHealthLevel(Owner, CurrentHealth / MaxHealth);
	}

	if(CurrentHealth <= 0.0f)
	{
		HitBoneName = BoneName;
		ShotOrigin = ShotFromDirection;

		if(bCommonInterface)
		{
			ICommonInterface::Execute_SetHealthState(Owner, EHealthState::Death);
		}
	}
	else if(CurrentHealth < LowHealth && bCommonInterface)
	{
		ICommonInterface::Execute_SetHealthState(Owner, EHealthState::RecoveryStarted);
	}
	
	FTimerHandle StartHealthRecoveryTimer;
	GetWorld()->GetTimerManager().SetTimer(StartHealthRecoveryTimer, this, &UHealthComponent::StartHealthRecovery, StartHealthRecoveryDelay);
}

// Only start when stamina is full
void UHealthComponent::StartHealthRecovery()
{
	if(bCanRecoverHealth)
	{
		if(bCommonInterface)
		{
			ICommonInterface::Execute_SetHealthState(Owner, EHealthState::RecoveryStarted);
		}

		GetWorld()->GetTimerManager().SetTimer(HealthRecoveryTimer, this, &UHealthComponent::RecoverHealth, true);
	}
}

// Stop when stamina is not full
void UHealthComponent::StopHealthRecovery()
{
	if(bCommonInterface)
	{
		ICommonInterface::Execute_SetHealthState(Owner, EHealthState::RecoveryStopped);
	}

	GetWorld()->GetTimerManager().ClearTimer(HealthRecoveryTimer);
}

void UHealthComponent::RecoverHealth()
{
	CurrentHealth = FMath::Clamp(HealingAmount + CurrentHealth, 0.0f, MaxHealth);

	if(bCharacterInterface)
	{
		ICharacterInterface::Execute_SetHealthLevel(Owner, CurrentHealth / MaxHealth);
	}

	if(CurrentHealth >= MaxHealth)
	{
		GetWorld()->GetTimerManager().ClearTimer(HealthRecoveryTimer);

		if(bCommonInterface)
		{
			ICommonInterface::Execute_SetHealthState(Owner, EHealthState::Full);
		}
	}
}

void UHealthComponent::Healing(const float HealthDifference)
{
	CurrentHealth = FMath::Clamp(HealthDifference + CurrentHealth, 0.0f, MaxHealth);

	if(bCharacterInterface)
	{
		ICharacterInterface::Execute_SetHealthLevel(Owner, CurrentHealth / MaxHealth);
	}

	if(CurrentHealth >= MaxHealth && bCommonInterface)
	{
		ICommonInterface::Execute_SetHealthState(Owner, EHealthState::Full);
	}
}