// Copyright 2022 Danial Kamali. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "LevelManager.generated.h"

class UBoxComponent;

/**
 * Used to load and unload a data layer
 */
UCLASS()
class ALevelManager : public AActor
{
	GENERATED_BODY()

	/** Root component */
	UPROPERTY()
	USceneComponent* SceneComp;

	/** Overlapping with this box will load the data layer */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = true))
	UBoxComponent* Loader;

	/** Overlapping with this box will unload the data layer */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = true))
	UBoxComponent* Unloader;

// Functions
public:
	ALevelManager();

protected:
	virtual void BeginPlay() override;

private:
	UFUNCTION()
	void BeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void DataLayerStateChanged(const UDataLayerInstance* DataLayer, EDataLayerRuntimeState State);

// Variables
private:
	/** The data layer asset that should be controlled by this level manager */
	UPROPERTY(EditInstanceOnly, Category = "Defaults")
	TSoftObjectPtr<UDataLayerAsset> DataLayerAsset;

	/** The door actor that is related to the data layer controlled by this level manager */
	UPROPERTY(EditInstanceOnly, Category = "Defaults")
	class AInteractableDoor* Door;
};