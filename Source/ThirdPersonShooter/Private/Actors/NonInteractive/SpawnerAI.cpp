// Copyright 2022-2023 Danial Kamali. All Rights Reserved.

#include "SpawnerAI.h"

ASpawnerAI::ASpawnerAI()
{
	PrimaryActorTick.bCanEverTick = false;
	PrimaryActorTick.bStartWithTickEnabled = false;
	SetHidden(true);
	SetCanBeDamaged(false);
	UpdateOverlapsMethodDuringLevelStreaming = EActorUpdateOverlapsMethod::NeverUpdate;

#if WITH_EDITORONLY_DATA
	bEnableAutoLODGeneration = false;
#endif
	
	SceneComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Scene Component"));
	RootComponent = SceneComponent;
	SceneComponent->Mobility = EComponentMobility::Static;
}

void ASpawnerAI::BeginPlay()
{
	Super::BeginPlay();
	
	// Start spawning if there is any actor to spawn
	if (RespawnList.Num() > 0)
	{
		StartSpawn();
	}
}

void ASpawnerAI::EnterSpawnQueue(const FSpawnData& SpawnData)
{
	RespawnList.Add(SpawnData);
	StartSpawn();
}

void ASpawnerAI::StartSpawn()
{
	// Start the spawn timer if it is not started yet.
	if (!GetWorld()->GetTimerManager().IsTimerActive(SpawnTimer))
	{
		GetWorld()->GetTimerManager().SetTimer(SpawnTimer, this, &ASpawnerAI::UpdateSpawnQueue, 1.0f, true);
	}
}

void ASpawnerAI::UpdateSpawnQueue()
{
	TArray<uint8> IndicesToSpawn;
	const uint8 ListLength = RespawnList.Num();
	for (uint8 i = 0; i < ListLength; ++i)
	{
		const int32 NewTime = --RespawnList[i].SpawnDelay;
		// Check if it is time to respawn
		if (NewTime > 0)
		{
			RespawnList[i].SpawnDelay = NewTime;
		}
		else
		{
			IndicesToSpawn.Add(i);
		}
	}
	
	const uint8 SpawnLength = IndicesToSpawn.Num();
	for (uint8 j = 0; j < SpawnLength; ++j)
	{
		FTransform Transform;
		FNavLocation NavLocation;
		/*const bool bResult = NavigationSystem->GetRandomReachablePointInRadius(GetActorLocation(), RespawnRadius->GetScaledSphereRadius(), NavLocation);
		bResult ? Transform.SetLocation(NavLocation.Location) : Transform.SetLocation(GetActorLocation());
		Transform.SetRotation(FRotator(0.0f, FMath::RandRange(0.0f, 360.0f), 0.0f).Quaternion());
		AAICharacter* NewActor = GetWorld()->SpawnActorDeferred<AAICharacter>(RespawnList[IndicesToSpawn[j]].CharacterToSpawn, Transform, nullptr, nullptr, ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn);
		NewActor->RespawnHandler = this;
		UGameplayStatics::FinishSpawningActor(NewActor, Transform);*/
		
		RespawnList.RemoveAt(IndicesToSpawn[j]);
		for (uint8 k = j; k < SpawnLength; ++k)
		{
			IndicesToSpawn[k] = --IndicesToSpawn[k];
		}
	}
	
	if (RespawnList.IsEmpty())
	{
		GetWorld()->GetTimerManager().ClearTimer(SpawnTimer);
	}
}
