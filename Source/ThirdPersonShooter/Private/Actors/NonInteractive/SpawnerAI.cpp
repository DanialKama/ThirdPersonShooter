// Copyright 2022-2023 Danial Kamali. All Rights Reserved.

#include "SpawnerAI.h"

#include "Characters/AICharacter.h"

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
	if (SpawnList.Num() > 0)
	{
		StartSpawn();
	}
}

void ASpawnerAI::EnterSpawnQueue(const FSpawnData& SpawnData)
{
	SpawnList.Add(SpawnData);
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
	// Index of the ready-to-spawn characters
	TArray<int32> ReadyToSpawn;

	int32 i;
	for (i = 0; i < SpawnList.Num(); ++i)
	{
		// Reduce the spawn delay and spawn the character if needed
		const int32 NewTime = --SpawnList[i].SpawnDelay;
		
		if (NewTime > 0)
		{
			SpawnList[i].SpawnDelay = NewTime;
		}
		else
		{
			ReadyToSpawn.Add(i);
		}
	}

	if (ReadyToSpawn.IsEmpty())
	{
		return;
	}
	
	for (i = 0; i < ReadyToSpawn.Num(); ++i)
	{
		FVector RandomUnit = FMath::VRand();
		RandomUnit.Z = 0.0f;
		RandomUnit *= FMath::FRandRange(0.0f, Range);

		FTransform SpawnTransform;
		SpawnTransform.SetLocation(RandomUnit + SceneComponent->GetComponentLocation());
		SpawnTransform.SetRotation(FRotator(0.0f, FMath::RandRange(0.0f, 360.0f), 0.0f).Quaternion());

		UClass* LoadedAsset = SpawnList[ReadyToSpawn[i]].CharacterToSpawn.LoadSynchronous();

		AAICharacter* NewCharacter = Cast<AAICharacter>(GetWorld()->SpawnActor(LoadedAsset, &SpawnTransform));
		if (NewCharacter)
		{
			NewCharacter->Spawner = this;
		
			SpawnList.RemoveAt(ReadyToSpawn[i]);
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("NEW CHARACTER IS NOT VALID!!!"));
		}
	}
	
	if (SpawnList.IsEmpty())
	{
		GetWorld()->GetTimerManager().ClearTimer(SpawnTimer);
	}
}
