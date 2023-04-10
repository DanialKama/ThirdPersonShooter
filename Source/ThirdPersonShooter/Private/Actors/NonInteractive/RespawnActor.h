// Copyright 2022-2023 Danial Kamali. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "RespawnActor.generated.h"

class AAICharacter;

// TODO: Delete this
USTRUCT(BlueprintType)
struct FRespawnInfo
{
	GENERATED_BODY()
	
	UPROPERTY(EditAnywhere, Category = "Default")
	float SpawnTime;
	
	UPROPERTY(EditAnywhere, Category = "Default")
	TSubclassOf<AAICharacter> CharacterToSpawn;

	// Default constructor
	FRespawnInfo()
	{
		SpawnTime = 5.0f;
	}
};

// TODO: Rename to AAISpawner
UCLASS(meta = (DisplayName = "AI Spawner"))
class ARespawnActor : public AActor
{
	GENERATED_BODY()

	// TODO: Use Scene component
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = true))
	TObjectPtr<UBillboardComponent> Billboard;

	// TODO: Use random location in box
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = true))
	TObjectPtr<class USphereComponent> RespawnRadius;

// Functions
public:	
	ARespawnActor();

	// TODO: Rename Respawn to Spawn
	
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
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Default")
	TArray<FRespawnInfo> RespawnList;

private:
	UPROPERTY(EditDefaultsOnly, Category = "Default", meta = (AllowPrivateAccess = true))
	float SpawnDelay = 5.0f;

	/** AI character to spawn */
	UPROPERTY(EditDefaultsOnly, Category = "Default", meta = (AllowPrivateAccess = true))
	TSubclassOf<AAICharacter> CharacterToSpawn;
	
	UPROPERTY()
	class UNavigationSystemV1* NavigationSystem;

	FTimerHandle SpawnTimer;
};
