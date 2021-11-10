

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ProjectileActor.generated.h"

UCLASS()
class THIRDPERSONSHOOTER_API AProjectileActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AProjectileActor();
	
	// Variables
	UPROPERTY(EditDefaultsOnly, Category = "Projectile")
	int32 NumberOfPellets = 1;

	UPROPERTY(EditDefaultsOnly, Category = "Projectile")
	float PelletSpread = 0.0f;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
