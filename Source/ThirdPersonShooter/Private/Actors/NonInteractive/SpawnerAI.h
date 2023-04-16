// Copyright 2022-2023 Danial Kamali. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SpawnerAI.generated.h"

class AAICharacter;

USTRUCT(BlueprintType, meta = (DisplayName = "Spawn Data"))
struct FSpawnData
{
	GENERATED_BODY()
	
	UPROPERTY(EditAnywhere, meta = (ClampMin = 0, UIMin = 0))
	int32 SpawnDelay;
	
	UPROPERTY(EditAnywhere)
	TSoftClassPtr<AAICharacter> CharacterToSpawn;

	// Default constructor
	FSpawnData()
		: SpawnDelay(5)
	{}
};

UCLASS(meta = (DisplayName = "AI Spawner"))
class ASpawnerAI : public AActor
{
	GENERATED_BODY()

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = true))
	TObjectPtr<USceneComponent> SceneComponent;

// Functions
public:
	/** Sets default values for this actor's properties */
	ASpawnerAI();

	void EnterSpawnQueue(const FSpawnData& SpawnData);

protected:
	/** Called when the game starts or when spawned */
	virtual void BeginPlay() override;

private:
	void StartSpawn();
	
	void UpdateSpawnQueue();

// Variables
public:
	UPROPERTY(EditAnywhere, Category = "Default")
	TArray<FSpawnData> RespawnList;

private:
	FTimerHandle SpawnTimer;
};
