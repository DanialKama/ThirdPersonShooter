// Copyright 2022-2023 Danial Kamali. All Rights Reserved.

#include "LayerManager.h"

#include "Characters/PlayerCharacter.h"
#include "Components/BoxComponent.h"
#include "InteractableDoor.h"
#include "WorldPartition/DataLayer/DataLayerSubsystem.h"

ALayerManager::ALayerManager()
{
	PrimaryActorTick.bCanEverTick = false;
	PrimaryActorTick.bStartWithTickEnabled = false;
	SetHidden(true);
	SetCanBeDamaged(false);

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

	Unloader = CreateDefaultSubobject<UBoxComponent>(TEXT("Unloader"));
	Unloader->SetupAttachment(SceneComp);
	Unloader->Mobility = EComponentMobility::Static;
	Unloader->SetLineThickness(2.0f);
	Unloader->ShapeColor = FColor::Blue;
	Unloader->CanCharacterStepUpOn = ECB_No;
	Unloader->SetCollisionProfileName(FName("OverlapOnlyPawn"));
	Unloader->SetCanEverAffectNavigation(false);
}

void ALayerManager::BeginPlay()
{
	Super::BeginPlay();

	Loader->OnComponentBeginOverlap.AddDynamic(this, &ALayerManager::BeginOverlap);
	Unloader->OnComponentBeginOverlap.AddDynamic(this, &ALayerManager::BeginOverlap);

	UDataLayerSubsystem* DataLayerSubsystem = GetWorld()->GetSubsystem<UDataLayerSubsystem>();
	DataLayerSubsystem->OnDataLayerRuntimeStateChanged.AddDynamic(this, &ALayerManager::DataLayerStateChanged);
}

void ALayerManager::BeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (Cast<APlayerCharacter>(OtherActor))
	{
		UDataLayerSubsystem* DataLayerSubsystem = GetWorld()->GetSubsystem<UDataLayerSubsystem>();

		// Load the data layer
		if (Loader == OverlappedComponent)
		{
			DataLayerSubsystem->SetDataLayerInstanceRuntimeState(DataLayerAsset, EDataLayerRuntimeState::Activated);
		}
		// Unload the data layer
		else
		{
			DataLayerSubsystem->SetDataLayerInstanceRuntimeState(DataLayerAsset, EDataLayerRuntimeState::Unloaded);
		}
	}
}

void ALayerManager::DataLayerStateChanged(const UDataLayerInstance* DataLayer, EDataLayerRuntimeState State)
{
	// Updating the door based on the data layer state
	const UDataLayerSubsystem* DataLayerSubsystem = GetWorld()->GetSubsystem<UDataLayerSubsystem>();

	if (DataLayerSubsystem->GetDataLayerInstanceFromAsset(DataLayerAsset) == DataLayer)
	{
		Door->bIsEnable = State == EDataLayerRuntimeState::Activated;
	}
}
