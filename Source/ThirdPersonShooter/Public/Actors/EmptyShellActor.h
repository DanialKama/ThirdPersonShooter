

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "EmptyShellActor.generated.h"

UCLASS()
class THIRDPERSONSHOOTER_API AEmptyShellActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AEmptyShellActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
