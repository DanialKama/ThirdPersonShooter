// All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "RespawnActor.generated.h"

class AAICharacter;

struct FRespawnInfo
{
	float SpawnTime;
	TSubclassOf<AAICharacter*> CharacterToSpawn;
};

UCLASS()
class THIRDPERSONSHOOTER_API ARespawnActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ARespawnActor();
	
	// Functions
	void StartRespawn(FRespawnInfo RespawnInfo);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
};