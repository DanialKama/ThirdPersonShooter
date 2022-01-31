// Copyright 2022 Danial Kamali. All Rights Reserved.

#include "Actors/RespawnActor.h"
#include "NavigationSystem.h"
#include "Characters/AICharacter.h"
#include "Components/BillboardComponent.h"
#include "Components/SphereComponent.h"
#include "Kismet/GameplayStatics.h"

ARespawnActor::ARespawnActor()
{
	PrimaryActorTick.bCanEverTick = false;

	Billboard = CreateDefaultSubobject<UBillboardComponent>(TEXT("Billboard"));
	SetRootComponent(Billboard);
	Billboard->SetComponentTickEnabled(false);
	Billboard->bIsScreenSizeScaled = true;
	
	RespawnRadius = CreateDefaultSubobject<USphereComponent>(TEXT("Respawn Radius"));
	RespawnRadius->SetupAttachment(Billboard);
	RespawnRadius->SetComponentTickEnabled(false);
	RespawnRadius->SetSphereRadius(250.0f);
	RespawnRadius->SetGenerateOverlapEvents(false);
	RespawnRadius->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	RespawnRadius->SetCollisionResponseToAllChannels(ECR_Ignore);
	RespawnRadius->CanCharacterStepUpOn = ECB_No;
}

void ARespawnActor::BeginPlay()
{
	Super::BeginPlay();

	// Initialize navigation system
	NavigationSystem = UNavigationSystemV1::GetCurrent(GetWorld());
	
	// Start respawning if there is any actor to respawn
	if (RespawnList.Num() > 0)
	{
		StartRespawn();
	}
}

void ARespawnActor::EnterRespawnQueue(FRespawnInfo RespawnInfo)
{
	RespawnList.Add(RespawnInfo);
	StartRespawn();
}

void ARespawnActor::StartRespawn()
{
	// If respawn timer is not started yet then start it
	if (RespawnTimer.IsValid() == false)
	{
		GetWorld()->GetTimerManager().SetTimer(RespawnTimer, this, &ARespawnActor::RespawnHandler, 0.5f, true);
	}
}

void ARespawnActor::RespawnHandler()
{
	TArray<uint8> IndicesToSpawn;
	const uint8 ListLength = RespawnList.Num();
	for (uint8 i = 0; i < ListLength; ++i)
	{
		const float NewTime = RespawnList[i].SpawnTime - 0.5f;
		// Check if it is time to respawn
		if (NewTime > 0.0f)
		{
			RespawnList[i].SpawnTime = NewTime;
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
		const bool bResult = NavigationSystem->GetRandomReachablePointInRadius(GetActorLocation(), RespawnRadius->GetScaledSphereRadius(), NavLocation);
		bResult ? Transform.SetLocation(NavLocation.Location) : Transform.SetLocation(GetActorLocation());
		Transform.SetRotation(FRotator(0.0f, FMath::RandRange(0.0f, 360.0f), 0.0f).Quaternion());
		AAICharacter* NewActor = GetWorld()->SpawnActorDeferred<AAICharacter>(RespawnList[IndicesToSpawn[j]].CharacterToSpawn, Transform, nullptr, nullptr, ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn);
		NewActor->RespawnHandler = this;
		UGameplayStatics::FinishSpawningActor(NewActor, Transform);
		
		RespawnList.RemoveAt(IndicesToSpawn[j]);
		for (uint8 k = j; k < SpawnLength; ++k)
		{
			IndicesToSpawn[k] = --IndicesToSpawn[k];
		}
	}
	
	if (RespawnList.IsEmpty())
	{
		GetWorld()->GetTimerManager().ClearTimer(RespawnTimer);
	}
}