// Copyright 2022-2023 Danial Kamali. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "RespawnActor.generated.h"

class AAICharacter;

USTRUCT(BlueprintType)
struct FRespawnInfo
{
	GENERATED_BODY()
	
	UPROPERTY(EditAnywhere, Category = "Defaults")
	float SpawnTime;
	
	UPROPERTY(EditAnywhere, Category = "Defaults")
	TSubclassOf<AAICharacter> CharacterToSpawn;

	// Default constructor
	FRespawnInfo()
	{
		SpawnTime = 5.0f;
	}
};

UCLASS()
class ARespawnActor : public AActor
{
	GENERATED_BODY()
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = true))
	UBillboardComponent* Billboard;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = true))
	class USphereComponent* RespawnRadius;

// Functions
public:	
	ARespawnActor();
	
	/** Enter the respawn queue and wait till respawning */
	void EnterRespawnQueue(FRespawnInfo RespawnInfo);

protected:
	virtual void BeginPlay() override;

private:
	void StartRespawn();
	
	UFUNCTION()
	void RespawnHandler();

// Variables
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Defaults")
	TArray<FRespawnInfo> RespawnList;

private:
	UPROPERTY()
	class UNavigationSystemV1* NavigationSystem;

	FTimerHandle RespawnTimer;
};