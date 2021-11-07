
#pragma once

#include "CoreMinimal.h"
#include "BaseComponent.h"
#include "AmmoComponent.generated.h"

class ICommonInterface;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class THIRDPERSONSHOOTER_API UAmmoComponent : public UBaseComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UAmmoComponent();

	// Functions
	UFUNCTION(BlueprintCallable, Category = "Public")
	void SetAmmoInfo(const int32 InMaxAmmo, const int32 InDefaultAmmo, const int32 InMagazineSize, const int32 InCurrentMagazineAmmo);

	UFUNCTION(BlueprintCallable, Category = "Public")
	void GetAmmoInfo(int32& OutCurrentAmmo, int32& OutMagazineSize, int32& OutCurrentMagazineAmmo) const;

	UFUNCTION(BlueprintCallable, Category = "Public")
	bool BetterToReload() const;

	UFUNCTION(BlueprintCallable, Category = "Public")
	bool CanReload() const;

	UFUNCTION(BlueprintCallable, Category = "Public")
	bool NoAmmoLeftToReload() const;

	UFUNCTION(BlueprintCallable, Category = "Public")
	void ReduceAmmo();

	UFUNCTION(BlueprintCallable, Category = "Public")
	void Reload();

	UFUNCTION(BlueprintCallable, Category = "Public")
	void AddAmmo(int32 AmmoAmount);
	
	// Variables
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "Public")
	int32 MaxAmmo = 0;
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Public")
	int32 DefaultAmmo = 0;

	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "Public")
	int32 MagazineSize = 1;
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Public", meta = (ToolTip = "If value set to something greater than zero then in initial value dose not change"))
	int32 CurrentMagazineAmmo = 0;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Public")
	int32 ReloadAmount = 1;
	
protected:
	virtual void SetupComponent() override;

private:
	// Functions
	UFUNCTION(BlueprintCallable, Category = "Private")
	bool IsCompletelyEmpty() const;
	
	// Variables
	ICommonInterface* CommonInterface;
	
	UPROPERTY(EditDefaultsOnly, Category = "Private")
	int32 CurrentAmmo = 0;
};
