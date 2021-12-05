// Copyright Epic Games, Inc. All Rights Reserved.

#include "Components/HealthComponent.h"
#include "Interfaces/CharacterInterface.h"

// Sets default values for this component's properties
UHealthComponent::UHealthComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	// Variable defaults
	bCanRecoverHealth = true;
}

void UHealthComponent::SetupComponent()
{
	Super::SetupComponent();
	
	Owner->OnTakeAnyDamage.AddDynamic(this, &UHealthComponent::TakeAnyDamage);
	Owner->OnTakePointDamage.AddDynamic(this, &UHealthComponent::TakePointDamage);
	Owner->OnTakeRadialDamage.AddDynamic(this, &UHealthComponent::TakeRadialDamage);

	// CurrentHealth = Clamp; TODO

	if(Owner)
	{
		// C++ only
		/*ICharacterInterface* Interface = Cast<ICharacterInterface>(Owner);
		if(Interface)
		{
			Interface->Execute_SetHealthLevel(Owner, CurrentHealth / MaxHealth);
		}*/
		// C++ and blueprint
		if (Owner->GetClass()->ImplementsInterface(UCharacterInterface::StaticClass()))
		{
			ICharacterInterface::Execute_SetHealthLevel(Owner, CurrentHealth / MaxHealth);
		}
	}
}

void UHealthComponent::TakeAnyDamage(AActor* DamagedActor, float Damage, const class UDamageType* DamageType, class AController* InstigatedBy, AActor* DamageCauser)
{
	// CurrentHealth = CurrentHealth - Damage Clamp TODO

	// Update current health on character
	if(Owner)
	{
		// C++ only
		/*ICharacterInterface* Interface = Cast<ICharacterInterface>(Owner);
		if(Interface)
		{
			Interface->Execute_SetHealthLevel(Owner, CurrentHealth / MaxHealth);
		}*/
		// C++ and blueprint
		if (Owner->GetClass()->ImplementsInterface(UCharacterInterface::StaticClass()))
		{
			ICharacterInterface::Execute_SetHealthLevel(Owner, CurrentHealth / MaxHealth);
		}
	}
	
	if(CurrentHealth <= 0.0f)
	{
		HitBoneName = TEXT("None");
		ShotOrigin = Owner->GetActorLocation();
	}
}

void UHealthComponent::TakePointDamage(AActor* DamagedActor, float Damage, AController* InstigatedBy, FVector HitLocation, UPrimitiveComponent* FHitComponent, FName BoneName, FVector ShotFromDirection, const UDamageType* DamageType, AActor* DamageCauser)
{
	
}

void UHealthComponent::TakeRadialDamage(AActor* DamagedActor, float Damage, const UDamageType* DamageType, FVector Origin, FHitResult HitInfo, AController* InstigatedBy, AActor* DamageCauser)
{
	
}