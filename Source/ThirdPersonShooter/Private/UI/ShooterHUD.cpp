// All Rights Reserved.

#include "UI/ShooterHUD.h"

AShooterHUD* AShooterHUD::GetHUDReference_Implementation()
{
	return this;
}

void AShooterHUD::SetHealth_Implementation(float CurrentHealth)
{
}

void AShooterHUD::SetStamina_Implementation(float CurrentStamina)
{
}

void AShooterHUD::SetUIVisibility_Implementation(ESlateVisibility NewVisibility)
{
}

void AShooterHUD::SetCrosshairVisibility_Implementation(ESlateVisibility NewVisibility)
{
}

void AShooterHUD::ToggleCommandMessage_Implementation(const FText &CommandText, ESlateVisibility NewVisibility, bool AutoHide)
{
}

void AShooterHUD::AddRecoil_Implementation(float Recoil, float ControlTime)
{
}
void AShooterHUD::SetAmmoInfoVisibility_Implementation(ESlateVisibility NewVisibility)
{
}