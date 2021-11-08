


#include "Actors/EmptyShellActor.h"

// Sets default values
AEmptyShellActor::AEmptyShellActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AEmptyShellActor::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AEmptyShellActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

