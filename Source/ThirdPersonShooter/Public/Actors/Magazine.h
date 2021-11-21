// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Sound/SoundCue.h"
#include "Magazine.generated.h"

UCLASS()
class THIRDPERSONSHOOTER_API AMagazine : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AMagazine();
	
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Meta = (ExposeOnSpawn = true))
	uint8 bMagazineIsEmpty : 1;

protected:
    // Components
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UStaticMeshComponent* StaticMesh;
	
private:
	// Functions
	UFUNCTION()
	void OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

	// Variables
	uint8 bDoOnce : 1;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Defaults", meta = (TooltTip = "If remaining bullets in mag is 0 then this static mesh will be displayed", AllowPrivateAccess = true))
	UStaticMesh* EmptyMagazine;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Defaults", meta = (TooltTip = "If remaining bullets in mag is more than 0 then this static mesh will be displayed", AllowPrivateAccess = true))
	UStaticMesh* FullMagazine;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Defaults", meta = (AllowPrivateAccess = true))
	USoundCue* HitSound;
};
