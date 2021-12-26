// All Rights Reserved.

#include "Components/AmmoComponent.h"
#include "Interfaces/CommonInterface.h"
#include "Structs/AmmoComponentInfoStruct.h"
#include "Math/UnrealMathUtility.h"
#include "Enums/PickupEnums.h"

// Sets default values for this component's properties
UAmmoComponent::UAmmoComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;
}

void UAmmoComponent::SetupComponent()
{
	Super::SetupComponent();
	
	CurrentAmmo = DefaultAmmo;

	// Detected if the interfaces is present on owner
	if(Owner)
	{
		if(Owner->GetClass()->ImplementsInterface(UCommonInterface::StaticClass()))
		{
			bCommonInterface = true;
		}
	}

	if(CurrentMagazineAmmo <= 0)
	{
		if(bCommonInterface)
		{
			ICommonInterface::Execute_SetCanFire(Owner, false);
		}
	}
}

void UAmmoComponent::ReduceAmmo()
{
	CurrentMagazineAmmo = --CurrentMagazineAmmo;
	
	if(bCommonInterface)
	{
		if(CurrentMagazineAmmo <= 0)
		{
			if(!NoAmmoLeftToReload())
			{
				ICommonInterface::Execute_SetWeaponState(Owner, EWeaponState::NeedToReload);
			}
		}
		else if(BetterToReload())
		{
			if(!NoAmmoLeftToReload())
			{
				ICommonInterface::Execute_SetWeaponState(Owner, EWeaponState::BetterToReload);
			}
		}
		
		ICommonInterface::Execute_SetWeaponState(Owner, EWeaponState::Firing);
		
		if(IsCompletelyEmpty())
		{
			ICommonInterface::Execute_SetWeaponState(Owner, EWeaponState::Empty);
		}
	}
}

void UAmmoComponent::Reload()
{
	if(bCommonInterface)
	{
		if(CurrentAmmo > 0)
		{
			const int32 UsedAmmoFromMag = MagazineSize - CurrentMagazineAmmo;
		
			if(CurrentAmmo >= UsedAmmoFromMag)
			{
				const int32 CurrentReloadAmount= FMath::Clamp(ReloadAmount, 0, UsedAmmoFromMag);
				CurrentAmmo = CurrentAmmo - CurrentReloadAmount;
				CurrentMagazineAmmo = CurrentReloadAmount + CurrentMagazineAmmo;

				if (CurrentMagazineAmmo == MagazineSize)
				{
					ICommonInterface::Execute_SetWeaponState(Owner, EWeaponState::Reloaded);
				}
				else
				{
					ICommonInterface::Execute_SetWeaponState(Owner, EWeaponState::Reloading);
				}
			}
			else
			{
				CurrentMagazineAmmo =+ CurrentAmmo;
				CurrentAmmo = 0;
				ICommonInterface::Execute_SetWeaponState(Owner, EWeaponState::Reloaded);
			}
		}
		else
		{
			ICommonInterface::Execute_SetWeaponState(Owner, EWeaponState::Empty);
		}
	}
}

void UAmmoComponent::AddAmmo(int32 AmmoAmount)
{
	CurrentAmmo = FMath::Clamp(AmmoAmount + CurrentAmmo, 0, MaxAmmo);
	if(bCommonInterface)
	{
		ICommonInterface::Execute_SetWeaponState(Owner, EWeaponState::AmmoAdded);
	}
}

FAmmoComponentInfo UAmmoComponent::GetAmmoComponentInfo() const
{
	return FAmmoComponentInfo(NoAmmoLeftToReload(), CurrentAmmo, MagazineSize, CurrentMagazineAmmo);
}

void UAmmoComponent::SetAmmoInfo(const int32 InMaxAmmo, const int32 InDefaultAmmo, const int32 InMagazineSize, const int32 InCurrentMagazineAmmo)
{
	MaxAmmo = InMaxAmmo;
	DefaultAmmo = InDefaultAmmo;
	MagazineSize = InMagazineSize;
	CurrentMagazineAmmo = InCurrentMagazineAmmo;
	SetupComponent();
	
}

bool UAmmoComponent::BetterToReload() const
{
	if(CurrentMagazineAmmo <= MagazineSize / 3)
	{
		return true;
	}
	return false;
}

bool UAmmoComponent::CanReload() const
{
	if(CurrentAmmo > 0 && CurrentMagazineAmmo < MagazineSize)
	{
		return true;
	}
	return false;
}

bool UAmmoComponent::NoAmmoLeftToReload() const
{
	if(CurrentAmmo <= 0)
	{
		return true;
	}
	return false;
}

bool UAmmoComponent::IsCompletelyEmpty() const
{
	if(CurrentAmmo <= 0 && CurrentMagazineAmmo <= 0)
	{
		return true;
	}
	return false;
}