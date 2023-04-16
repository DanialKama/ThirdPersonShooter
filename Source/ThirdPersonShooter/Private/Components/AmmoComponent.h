// Copyright 2022-2023 Danial Kamali. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "BaseComponent.h"
#include "Core/Structures/AmmoComponentInfo.h"
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
	int32 MaxAmmo = 0;
	
	UPROPERTY(EditAnywhere, Category = "Default", meta = (ClampMin = "0", UIMin = "0"))
	int32 DefaultAmmo = 0;

	UPROPERTY(EditDefaultsOnly, Category = "Default", meta = (ClampMin = "0", UIMin = "0"))
	int32 MagazineSize = 1;

	/** If value set to something greater than zero then in initial value dose not change */
	UPROPERTY(EditAnywhere, Category = "Default")
	int32 CurrentMagazineAmmo = 0;

	UPROPERTY(EditDefaultsOnly, Category = "Default", meta = (ClampMin = "0", ClampMax = "999", UIMin = "0", UIMax = "999"))
	int32 ReloadAmount = 1;

	UPROPERTY(BlueprintReadOnly, Category = "Default")
	int32 CurrentAmmo = 0;
	
private:
	uint8 bCommonInterface : 1;
};
