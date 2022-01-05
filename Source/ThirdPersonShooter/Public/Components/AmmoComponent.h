// All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "BaseComponent.h"
#include "Structs/AmmoComponentInfoStruct.h"
#include "AmmoComponent.generated.h"

UCLASS()
class THIRDPERSONSHOOTER_API UAmmoComponent : public UBaseComponent
{
	GENERATED_BODY()

public:	
	UAmmoComponent();
	
	FAmmoComponentInfo GetAmmoComponentInfo() const;
	void SetAmmoInfo(const int32 InMaxAmmo, const int32 InDefaultAmmo, const int32 InMagazineSize, const int32 InCurrentMagazineAmmo);
	bool BetterToReload() const;
	bool CanReload() const;
	bool NoAmmoLeftToReload() const;
	void ReduceAmmo();
	void Reload();
	void AddAmmo(int32 AmmoAmount);
	
	UPROPERTY(EditDefaultsOnly, Category = "Defaults", meta = (ClampMin = "0", UIMin = "0"))
	int32 MaxAmmo = 0;
	
	UPROPERTY(EditAnywhere, Category = "Defaults", meta = (ClampMin = "0", UIMin = "0"))
	int32 DefaultAmmo = 0;

	UPROPERTY(EditDefaultsOnly, Category = "Defaults", meta = (ClampMin = "0", UIMin = "0"))
	int32 MagazineSize = 1;
	
	UPROPERTY(EditAnywhere, Category = "Defaults", meta = (ToolTip = "If value set to something greater than zero then in initial value dose not change"))
	int32 CurrentMagazineAmmo = 0;

	UPROPERTY(EditDefaultsOnly, Category = "Defaults", meta = (ClampMin = "0", ClampMax = "999", UIMin = "0", UIMax = "999"))
	int32 ReloadAmount = 1;

	UPROPERTY(BlueprintReadOnly, Category = "Defaults")
	int32 CurrentAmmo = 0;
	
protected:
	virtual void SetupComponent() override;

private:
	bool IsCompletelyEmpty() const;
	
	uint8 bCommonInterface : 1;
};