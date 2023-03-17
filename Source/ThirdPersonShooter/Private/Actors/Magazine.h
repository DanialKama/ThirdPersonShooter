// Copyright 2022-2023 Danial Kamali. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Magazine.generated.h"

UCLASS(meta = (DisplayName = "Magazine Actor"))
class AMagazine : public AActor
{
	GENERATED_BODY()

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = true))
	TObjectPtr<UStaticMeshComponent> StaticMesh;

// Functions
public:	
	AMagazine();
	
	FORCEINLINE UStaticMeshComponent* GetMesh() const { return StaticMesh; }

protected:
	virtual void BeginPlay() override;

private:
	UFUNCTION()
	void OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

// Variables
public:
	uint8 bIsEmpty : 1;
	
private:
	/** If the remaining bullets in the magazine are 0, then this static mesh will be used. */
	UPROPERTY(EditDefaultsOnly, Category = "Defaults", meta = (AllowPrivateAccess = true))
	TObjectPtr<UStaticMesh> EmptyMagazine;
	
	/** If the remaining bullets in the magazine are more than 0, then this static mesh will be used. */
	UPROPERTY(EditDefaultsOnly, Category = "Defaults", meta = (AllowPrivateAccess = true))
	TObjectPtr<UStaticMesh> FullMagazine;
	
	UPROPERTY(EditDefaultsOnly, Category = "Defaults", meta = (AllowPrivateAccess = true))
	TObjectPtr<USoundBase> HitSound;

	uint8 bDoOnceHit : 1;
};
