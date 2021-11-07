// Danial Kama

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

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Interfaces
	virtual void GetPickupReference_Implementation(APickup* &Reference) override;
	virtual void SetPickupStatus_Implementation(EPickupState PickupState) override;
	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Components
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
	USceneComponent* Scene;
	
	// Variables
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Variables|Dynamics")
	EItemType PickupType = EItemType::Weapon;

	UPROPERTY(BlueprintReadOnly, Category = "Protected|References")
	APawn* PickupOwner;
};
