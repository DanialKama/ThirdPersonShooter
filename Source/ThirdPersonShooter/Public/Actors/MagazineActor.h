

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MagazineActor.generated.h"

UCLASS()
class THIRDPERSONSHOOTER_API AMagazineActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AMagazineActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
