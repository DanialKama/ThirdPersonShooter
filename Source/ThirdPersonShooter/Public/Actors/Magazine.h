// All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Magazine.generated.h"

UCLASS()
class THIRDPERSONSHOOTER_API AMagazine : public AActor
{
	GENERATED_BODY()

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = true))
	UStaticMeshComponent* StaticMesh;

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
	/** Set it to true if the old magazine is empty */
	uint8 bMagazineIsEmpty : 1;
	
private:
	UPROPERTY(EditDefaultsOnly, Category = "Defaults", meta = (TooltTip = "If the remaining bullets in the magazine is 0 then this static mesh will be displayed", AllowPrivateAccess = true))
	UStaticMesh* EmptyMagazine;
	
	UPROPERTY(EditDefaultsOnly, Category = "Defaults", meta = (TooltTip = "If the remaining bullets in the magazine is more than 0 then this static mesh will be displayed", AllowPrivateAccess = true))
	UStaticMesh* FullMagazine;
	
	UPROPERTY(EditDefaultsOnly, Category = "Defaults", meta = (AllowPrivateAccess = true))
	class USoundCue* HitSound;

	/** Play the sound of hitting the ground for once */
	uint8 bDoOnce : 1;
};