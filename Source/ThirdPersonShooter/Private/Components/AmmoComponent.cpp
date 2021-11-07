
#include "Components/AmmoComponent.h"
#include "Interfaces/CommonInterface.h"
#include "Math/UnrealMathUtility.h"

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

	CommonInterface = Cast<ICommonInterface>(Owner);

	if(CurrentMagazineAmmo <= 0)
	{
		if(CommonInterface)
		{
			CommonInterface->SetCanFire(false);
		}
		// Optional, replace this code with interface call
		// On Setup component will fill the mag if Current Magazine Ammo is zero
		/*if(CurrentAmmo >= MagazineSize)
		{
			CurrentAmmo = CurrentAmmo - MagazineSize;
			CurrentMagazineAmmo = MagazineSize;
		}
		else
		{
			CurrentMagazineAmmo = CurrentAmmo;
			CurrentAmmo = 0;
		}*/
	}
}

void UAmmoComponent::ReduceAmmo()
{
	// By making Interface invalid player can fire without restrictions
	// Component will not be able to communicate with the weapon and tell it to stop fire when there is no ammo left
	CurrentMagazineAmmo = --CurrentMagazineAmmo;
	
	if(CommonInterface)
	{
		if(CurrentMagazineAmmo <= 0)
		{
			if(!NoAmmoLeftToReload())
			{
				CommonInterface->SetWeaponState(EWeaponState::NeedToReload);
			}
		}
		else if(BetterToReload())
		{
			if(!NoAmmoLeftToReload())
			{
				CommonInterface->SetWeaponState(EWeaponState::BetterToReload);
			}
		}
		CommonInterface->SetWeaponState(EWeaponState::Firing);
		if(IsCompletelyEmpty())
		{
			CommonInterface->SetWeaponState(EWeaponState::Empty);
		}
	}
}

void UAmmoComponent::Reload()
{
	if(CommonInterface)
	{
		if(CurrentAmmo > 0)
		{
			const int32 UsedAmmoFromMag = MagazineSize - CurrentMagazineAmmo;
		
			if(CurrentAmmo >= UsedAmmoFromMag)
			{
				const int32 CurrentReloadAmount= FMath::Clamp(ReloadAmount, 0, UsedAmmoFromMag);
				CurrentAmmo = CurrentAmmo - CurrentReloadAmount;
				CurrentMagazineAmmo = CurrentReloadAmount + CurrentMagazineAmmo;
			}
			else
			{
				CurrentMagazineAmmo =+ CurrentAmmo;
				CurrentAmmo = 0;
			}
			CommonInterface->SetWeaponState(EWeaponState::Reloaded);
		}
		else
		{
			CommonInterface->SetWeaponState(EWeaponState::Empty);
		}
	}
}

void UAmmoComponent::AddAmmo(int32 AmmoAmount)
{
	CurrentAmmo = FMath::Clamp(AmmoAmount + CurrentAmmo, 0, MaxAmmo);
	if(CommonInterface)
	{
		CommonInterface->SetWeaponState(EWeaponState::AmmoAdded);
	}
}

void UAmmoComponent::SetAmmoInfo(const int32 InMaxAmmo, const int32 InDefaultAmmo, const int32 InMagazineSize, const int32 InCurrentMagazineAmmo)
{
	MaxAmmo = InMaxAmmo;
	DefaultAmmo = InDefaultAmmo;
	MagazineSize = InMagazineSize;
	CurrentMagazineAmmo = InCurrentMagazineAmmo;
	SetupComponent();
	
}

void UAmmoComponent::GetAmmoInfo(int32& OutCurrentAmmo, int32& OutMagazineSize, int32& OutCurrentMagazineAmmo) const
{
	OutCurrentAmmo = CurrentAmmo;
	OutMagazineSize = MagazineSize;
	OutCurrentMagazineAmmo = CurrentMagazineAmmo;
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