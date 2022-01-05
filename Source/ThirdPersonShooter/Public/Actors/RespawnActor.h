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
	
	UPROPERTY(EditAnywhere, Category = "Defaults")
	float SpawnTime;
	
	UPROPERTY(EditAnywhere, Category = "Defaults")
	TSubclassOf<AAICharacter> CharacterToSpawn;
};

UCLASS()
class THIRDPERSONSHOOTER_API ARespawnActor : public AActor
{
	GENERATED_BODY()
	
public:	
	ARespawnActor();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UBillboardComponent* Billboard;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	USphereComponent* RespawnRadius;
	
	/** Enter the respawn queue and wait till respawning */
	void EnterRespawnQueue(FRespawnInfo RespawnInfo);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Defaults")
	TArray<FRespawnInfo> RespawnList;

protected:
	virtual void BeginPlay() override;

private:
	void StartRespawn();
	
	UFUNCTION()
	void RespawnHandler();

	UPROPERTY()
	UNavigationSystemV1* NavigationSystem;

	FTimerHandle RespawnTimer;
};