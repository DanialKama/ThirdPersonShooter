// Copyright 2022-2023 Danial Kamali. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "BaseComponent.h"
#include "Structs/AmmoComponentInfoStruct.h"
#include "AmmoComponent.generated.h"

UCLASS()
class UAmmoComponent : public UBaseComponent
{
	GENERATED_BODY()

// Functions
public:	
	UAmmoComponent();

	virtual void Initialize() override;
	
	FAmmoComponentInfo GetAmmoComponentInfo() const;
	void SetAmmoInfo(const int32 InMaxAmmo, const int32 InDefaultAmmo, const int32 InMagazineSize, const int32 InCurrentMagazineAmmo);
	bool BetterToReload() const;
	bool CanReload() const;
	bool NoAmmoLeftToReload() const;
	void ReduceAmmo();
	void Reload();
	void AddAmmo(int32 AmmoAmount);

private:
	bool IsCompletelyEmpty() const;

// Variables
public:
	UPROPERTY(EditDefaultsOnly, Category = "Default", meta = (ClampMin = "0", UIMin = "0"))
	int32 MaxAmmo;
	
	UPROPERTY(EditAnywhere, Category = "Default", meta = (ClampMin = "0", UIMin = "0"))
	int32 DefaultAmmo;

	UPROPERTY(EditDefaultsOnly, Category = "Default", meta = (ClampMin = "0", UIMin = "0"))
	int32 MagazineSize;
	
	UPROPERTY(EditAnywhere, Category = "Default", meta = (ToolTip = "If value set to something greater than zero then in initial value dose not change"))
	int32 CurrentMagazineAmmo;

	UPROPERTY(EditDefaultsOnly, Category = "Default", meta = (ClampMin = "0", ClampMax = "999", UIMin = "0", UIMax = "999"))
	int32 ReloadAmount;

	UPROPERTY(BlueprintReadOnly, Category = "Default")
	int32 CurrentAmmo;
	
private:
	uint8 bCommonInterface : 1;
};