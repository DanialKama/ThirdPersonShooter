// All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "RespawnActor.generated.h"

class AAICharacter;
class USphereComponent;
class UNavigationSystemV1;

USTRUCT(BlueprintType)
struct FRespawnInfo
{
	GENERATED_BODY()
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Defaults")
	float SpawnTime;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Defaults")
	TSubclassOf<AAICharacter> CharacterToSpawn;
};

UCLASS()
class THIRDPERSONSHOOTER_API ARespawnActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ARespawnActor();

	// Components
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UBillboardComponent* Billboard;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	USphereComponent* RespawnRadius;
	
	// Functions
	/** Enter the respawn queue and wait till respawning */
	void EnterRespawnQueue(FRespawnInfo RespawnInfo);

	// Variables
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Defaults")
	TArray<FRespawnInfo> RespawnList;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

private:
	// Functions
	void StartRespawn();
	UFUNCTION()
	void RespawnHandler();

	// Variables
	FTimerHandle RespawnTimer;
	UPROPERTY()
	UNavigationSystemV1* NavigationSystem;
};