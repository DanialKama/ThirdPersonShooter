// All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Enums/PickupEnums.h"
#include "Pickup.generated.h"

UCLASS()
class THIRDPERSONSHOOTER_API APickup : public AActor
{
	GENERATED_BODY()
	
public:	
	APickup();

	virtual void SetPickupStatus(EPickupState PickupState);

protected:
	UPROPERTY(EditDefaultsOnly, Category = "Defaults")
	EItemType PickupType = EItemType::Weapon;
};