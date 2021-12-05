
#pragma once

#include "CoreMinimal.h"
#include "BaseComponent.h"
#include "Structs/AmmoComponentInfoStruct.h"
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
	UFUNCTION(BlueprintCallable, Category = "AmmoComponent")
	FAmmoComponentInfo GetAmmoComponentInfo() const;
	
	UFUNCTION(BlueprintCallable, Category = "AmmoComponent")
	void SetAmmoInfo(const int32 InMaxAmmo, const int32 InDefaultAmmo, const int32 InMagazineSize, const int32 InCurrentMagazineAmmo);

	UFUNCTION(BlueprintCallable, Category = "AmmoComponent")
	void GetAmmoInfo(int32& OutCurrentAmmo, int32& OutMagazineSize, int32& OutCurrentMagazineAmmo) const;

	UFUNCTION(BlueprintCallable, Category = "AmmoComponent")
	bool BetterToReload() const;

	UFUNCTION(BlueprintCallable, Category = "AmmoComponent")
	bool CanReload() const;

	UFUNCTION(BlueprintCallable, Category = "AmmoComponent")
	bool NoAmmoLeftToReload() const;

	UFUNCTION(BlueprintCallable, Category = "AmmoComponent")
	void ReduceAmmo();

	UFUNCTION(BlueprintCallable, Category = "AmmoComponent")
	void Reload();

	UFUNCTION(BlueprintCallable, Category = "AmmoComponent")
	void AddAmmo(int32 AmmoAmount);
	
	// Variables
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "Defaults")
	int32 MaxAmmo = 0;
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Defaults")
	int32 DefaultAmmo = 0;

	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "Defaults")
	int32 MagazineSize = 1;
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Defaults", meta = (ToolTip = "If value set to something greater than zero then in initial value dose not change"))
	int32 CurrentMagazineAmmo = 0;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Defaults")
	int32 ReloadAmount = 1;
	
protected:
	virtual void SetupComponent() override;

private:
	// Functions
	UFUNCTION(BlueprintCallable, Category = "AmmoComponent")
	bool IsCompletelyEmpty() const;
	
	// Variables
	ICommonInterface* CommonInterface;
	int32 CurrentAmmo = 0;
};
