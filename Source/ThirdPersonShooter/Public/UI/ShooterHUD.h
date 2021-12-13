// All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "Interfaces/HUDInterface.h"
#include "ShooterHUD.generated.h"

UCLASS()
class THIRDPERSONSHOOTER_API AShooterHUD : public AHUD, public IHUDInterface
{
	GENERATED_BODY()
	
public:
	// Functions
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "ShooterHUD")
	void SetHealth(float CurrentHealth);
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "ShooterHUD")
	void SetStamina(float CurrentStamina);
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "ShooterHUD")
	void SetUIVisibility(ESlateVisibility NewVisibility);
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "ShooterHUD")
	void SetCrosshairVisibility(ESlateVisibility NewVisibility);
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "ShooterHUD")
	void ToggleCommandMessage(const FText &CommandText, ESlateVisibility NewVisibility, bool AutoHide);
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "ShooterHUD")
	void AddRecoil(float Recoil, float ControlTime);
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "ShooterHUD")
	void SetAmmoInfoVisibility(ESlateVisibility NewVisibility);
	// Interfaces
	virtual AShooterHUD* GetHUDReference_Implementation() override;
};
