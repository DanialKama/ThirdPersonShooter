// Copyright 2022 Danial Kamali. All Rights Reserved.

#include "Actors/LevelManager.h"
#include "Characters/PlayerCharacter.h"
#include "Components/BoxComponent.h"
#include "InteractableDoor.h"
#include "WorldPartition/DataLayer/DataLayerAsset.h"
#include "WorldPartition/DataLayer/DataLayerSubsystem.h"

ALevelManager::ALevelManager()
{
	PrimaryActorTick.bCanEverTick = false;
	PrimaryActorTick.bStartWithTickEnabled = false;
	SetHidden(true);
	bIsSpatiallyLoaded = false;

	SceneComp = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	RootComponent = SceneComp;
	SceneComp->Mobility = EComponentMobility::Static;
	
	Loader = CreateDefaultSubobject<UBoxComponent>(TEXT("Loader"));
	Loader->SetupAttachment(SceneComp);
	Loader->Mobility = EComponentMobility::Static;
	Loader->SetLineThickness(2.0f);
	Loader->ShapeColor = FColor::Green;
	Loader->CanCharacterStepUpOn = ECB_No;
	Loader->SetCollisionProfileName(FName("OverlapOnlyPawn"));
	Loader->SetCanEverAffectNavigation(false);
	Loader->OnComponentBeginOverlap.AddDynamic(this, &ALevelManager::BeginOverlap);

	Unloader = CreateDefaultSubobject<UBoxComponent>(TEXT("Unloader"));
	Unloader->SetupAttachment(SceneComp);
	Unloader->Mobility = EComponentMobility::Static;
	Unloader->SetLineThickness(2.0f);
	Unloader->ShapeColor = FColor::Green;
	Unloader->CanCharacterStepUpOn = ECB_No;
	Unloader->SetCollisionProfileName(FName("OverlapOnlyPawn"));
	Unloader->SetCanEverAffectNavigation(false);
	Unloader->OnComponentBeginOverlap.AddDynamic(this, &ALevelManager::BeginOverlap);
}

void ALevelManager::BeginPlay()
{
	Super::BeginPlay();

	UDataLayerSubsystem* DataLayerSubsystem = GetWorld()->GetSubsystem<UDataLayerSubsystem>();
	DataLayerSubsystem->OnDataLayerRuntimeStateChanged.AddDynamic(this, &ALevelManager::DataLayerStateChanged);
}

void ALevelManager::BeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (Cast<APlayerCharacter>(OtherActor))
	{
		UDataLayerSubsystem* DataLayerSubsystem = GetWorld()->GetSubsystem<UDataLayerSubsystem>();

		// Load the data layer
		if (Loader == OverlappedComponent)
		{
			DataLayerSubsystem->SetDataLayerInstanceRuntimeState(DataLayerAsset.LoadSynchronous(), EDataLayerRuntimeState::Activated);
		}
		// Unload the data layer
		else
		{
			DataLayerSubsystem->SetDataLayerInstanceRuntimeState(DataLayerAsset.LoadSynchronous(), EDataLayerRuntimeState::Unloaded);
		}
	}
}

void ALevelManager::DataLayerStateChanged(const UDataLayerInstance* DataLayer, EDataLayerRuntimeState State)
{
	// Updating the door based on the data layer state
	if (DataLayerAsset == DataLayer)
	{
		Door->bIsEnable = State == EDataLayerRuntimeState::Activated;
	}
}