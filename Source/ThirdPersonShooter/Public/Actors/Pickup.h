// All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Enums/PickupEnums.h"
#include "Interfaces/PickupInterface.h"
#include "Pickup.generated.h"

UCLASS()
class THIRDPERSONSHOOTER_API APickup : public AActor, public IPickupInterface
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	APickup();

	UFUNCTION(BlueprintCallable, Category = "Pickup")
	virtual void SetPickupStatus(EPickupState PickupState);

	// Interfaces
	virtual APickup* GetPickupReference_Implementation() override;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	// Variables
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Defaults")
	EItemType PickupType = EItemType::Weapon;
};