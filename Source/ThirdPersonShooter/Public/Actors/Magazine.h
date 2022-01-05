// All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Magazine.generated.h"

class USoundCue;

UCLASS()
class THIRDPERSONSHOOTER_API AMagazine : public AActor
{
	GENERATED_BODY()
	
public:	
	AMagazine();
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UStaticMeshComponent* StaticMesh;

	/** Set it to true if the old magazine is empty */
	uint8 bMagazineIsEmpty : 1;

protected:
	virtual void BeginPlay() override;

private:
	UFUNCTION()
	void OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);
	
	/** Play the sound of hitting the ground for once */
	uint8 bDoOnce : 1;
	
	UPROPERTY(EditDefaultsOnly, Category = "Defaults", meta = (TooltTip = "If the remaining bullets in the magazine is 0 then this static mesh will be displayed", AllowPrivateAccess = true))
	UStaticMesh* EmptyMagazine;
	
	UPROPERTY(EditDefaultsOnly, Category = "Defaults", meta = (TooltTip = "If the remaining bullets in the magazine is more than 0 then this static mesh will be displayed", AllowPrivateAccess = true))
	UStaticMesh* FullMagazine;
	
	UPROPERTY(EditDefaultsOnly, Category = "Defaults", meta = (AllowPrivateAccess = true))
	USoundCue* HitSound;
};