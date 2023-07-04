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
	bDrawDebug = false;
#endif
	
	SceneComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Scene Component"));
	RootComponent = SceneComponent;
	SceneComponent->Mobility = EComponentMobility::Static;
}

void ASpawnerAI::BeginPlay()
{
	Super::BeginPlay();
	
	// Start spawning if there is any actor to spawn
	if (SpawnList.IsEmpty() == false)
	{
		StartSpawn();
	}

#if WITH_EDITORONLY_DATA
	if (bDrawDebug)
	{
		DrawDebugCircle(GetWorld(), SceneComponent->GetComponentLocation(), Range, 36, FLinearColor::Green.ToFColor(true), false, 10.0f, SDPG_World, 5.0f, FVector(0,1,0), FVector(1,0,0), false);
	}
#endif
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

		if (AAICharacter* NewCharacter = Cast<AAICharacter>(GetWorld()->SpawnActor(LoadedAsset, &SpawnTransform)))
		{
			NewCharacter->Spawner = this;
		
			SpawnList.RemoveAt(ReadyToSpawn[i]);
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("Failed to spawn the AI character"));
		}
	}
	
	if (SpawnList.IsEmpty())
	{
		GetWorld()->GetTimerManager().ClearTimer(SpawnTimer);
	}
}
