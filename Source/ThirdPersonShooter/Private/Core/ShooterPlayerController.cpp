// Copyright 2022-2023 Danial Kamali. All Rights Reserved.

#include "ShooterPlayerController.h"

#include "Components/SlateWrapperTypes.h"
#include "UI/ShooterHUD.h"

void AShooterPlayerController::BeginPlay()
{
	Super::BeginPlay();

	ShooterHUD = Cast<AShooterHUD>(GetHUD());
}

void AShooterPlayerController::SetWeaponState_Implementation(FAmmoComponentInfo AmmoComponentInfo, EWeaponState WeaponState)
{
	// Use Text macro
	switch (WeaponState)
	{
	case EWeaponState::Idle:
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
	case EWeaponState::Firing:
		ShooterHUD->UpdateInMagAmmo(AmmoComponentInfo.CurrentMagazineAmmo);
		break;
	case EWeaponState::BetterToReload: case EWeaponState::NeedToReload:
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
	case EWeaponState::Reloading:
		ShooterHUD->ToggleCommandMessage(FText::FromName("Press R to Reload"), ESlateVisibility::Hidden, false);
		ShooterHUD->UpdateWeaponAmmo(AmmoComponentInfo.CurrentAmmo, AmmoComponentInfo.MagazineSize, AmmoComponentInfo.CurrentMagazineAmmo);
		break;
	case EWeaponState::CancelReload:
		ShooterHUD->ToggleCommandMessage(FText::FromName("Reload Cancelled"), ESlateVisibility::Visible, true);
		break;
	case EWeaponState::Reloaded:
		ShooterHUD->ToggleCommandMessage(FText::FromName("Press R to Reload"), ESlateVisibility::Hidden, false);
		ShooterHUD->UpdateWeaponAmmo(AmmoComponentInfo.CurrentAmmo, AmmoComponentInfo.MagazineSize, AmmoComponentInfo.CurrentMagazineAmmo);
		break;
	case EWeaponState::AmmoAdded:
		ShooterHUD->ToggleCommandMessage(FText::FromName("Ammo Added"), ESlateVisibility::Visible, true);
		ShooterHUD->UpdateWeaponAmmo(AmmoComponentInfo.CurrentAmmo, AmmoComponentInfo.MagazineSize, AmmoComponentInfo.CurrentMagazineAmmo);
		break;
	case EWeaponState::Empty:
		ShooterHUD->ToggleCommandMessage(FText::FromName("No Ammo"), ESlateVisibility::Visible, true);
		break;
	case EWeaponState::Overheat:
		ShooterHUD->ToggleCommandMessage(FText::FromName("Overheated"), ESlateVisibility::Visible, true);
		break;
	}
}

void AShooterPlayerController::PlayCameraShake_Implementation(TSubclassOf<UCameraShakeBase> CameraShake)
{
	// TODO: Use camera manager
	ClientStartCameraShake(CameraShake);
}
