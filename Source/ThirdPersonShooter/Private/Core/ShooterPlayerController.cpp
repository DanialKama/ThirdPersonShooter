// All Rights Reserved.

#include "Core/ShooterPlayerController.h"
#include "Components/SlateWrapperTypes.h"
#include "UI/ShooterHUD.h"

void AShooterPlayerController::BeginPlay()
{
	Super::BeginPlay();

	ShooterHUD = Cast<AShooterHUD>(GetHUD());
}

void AShooterPlayerController::SetWeaponState_Implementation(FAmmoComponentInfo AmmoComponentInfo, EWeaponState WeaponState)
{
	switch (WeaponState)
	{
	case 0:
		// Idle
		ShooterHUD->UpdateWeaponAmmo(AmmoComponentInfo.CurrentAmmo, AmmoComponentInfo.MagazineSize, AmmoComponentInfo.CurrentMagazineAmmo);
		if (AmmoComponentInfo.bNoAmmoLeftToReload)
		{
			ShooterHUD->ToggleCommandMessage(FText::FromName("No Ammo"), ESlateVisibility::Visible, false);
		}
		else if (AmmoComponentInfo.CurrentMagazineAmmo == 0)
		{
			ShooterHUD->ToggleCommandMessage(FText::FromName("Press R to Reload"), ESlateVisibility::Visible, false);
		}
		break;
	case 1:
		// Firing
		ShooterHUD->UpdateInMagAmmo(AmmoComponentInfo.CurrentMagazineAmmo);
		break;
	case 2: case 3:
		// Better To Reload, Need To Reload
		ShooterHUD->UpdateWeaponAmmo(AmmoComponentInfo.CurrentAmmo, AmmoComponentInfo.MagazineSize, AmmoComponentInfo.CurrentMagazineAmmo);
		if (AmmoComponentInfo.bNoAmmoLeftToReload)
		{
			ShooterHUD->ToggleCommandMessage(FText::FromName("Press R to Reload"), ESlateVisibility::Hidden, false);
		}
		else
		{
			ShooterHUD->ToggleCommandMessage(FText::FromName("Press R to Reload"), ESlateVisibility::Visible, false);
		}
		break;
	case 4:
		// Reloading
		ShooterHUD->ToggleCommandMessage(FText::FromName("Press R to Reload"), ESlateVisibility::Hidden, false);
		ShooterHUD->UpdateWeaponAmmo(AmmoComponentInfo.CurrentAmmo, AmmoComponentInfo.MagazineSize, AmmoComponentInfo.CurrentMagazineAmmo);
		break;
	case 5:
		// Cancel Reload
		ShooterHUD->ToggleCommandMessage(FText::FromName("Reload Cancelled"), ESlateVisibility::Visible, true);
		break;
	case 6:
		// Reloaded
		ShooterHUD->ToggleCommandMessage(FText::FromName("Press R to Reload"), ESlateVisibility::Hidden, false);
		ShooterHUD->UpdateWeaponAmmo(AmmoComponentInfo.CurrentAmmo, AmmoComponentInfo.MagazineSize, AmmoComponentInfo.CurrentMagazineAmmo);
		break;
	case 7:
		// Ammo Added
		ShooterHUD->ToggleCommandMessage(FText::FromName("Ammo Added"), ESlateVisibility::Visible, true);
		ShooterHUD->UpdateWeaponAmmo(AmmoComponentInfo.CurrentAmmo, AmmoComponentInfo.MagazineSize, AmmoComponentInfo.CurrentMagazineAmmo);
		break;
	case 8:
		// Empty
		ShooterHUD->ToggleCommandMessage(FText::FromName("No Ammo"), ESlateVisibility::Visible, true);
		break;
	case 9:
		// Overheat
		ShooterHUD->ToggleCommandMessage(FText::FromName("Overheated"), ESlateVisibility::Visible, true);
		break;
	}
}

void AShooterPlayerController::PlayCameraShake_Implementation(TSubclassOf<UCameraShakeBase> CameraShake)
{
	ClientStartCameraShake(CameraShake);
}