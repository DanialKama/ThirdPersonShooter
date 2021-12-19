// All Rights Reserved.


#include "Actors/RespawnActor.h"

// Sets default values
ARespawnActor::ARespawnActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ARespawnActor::BeginPlay()
{
	Super::BeginPlay();
	
}

void ARespawnActor::StartRespawn(FRespawnInfo RespawnInfo)
{
	
}