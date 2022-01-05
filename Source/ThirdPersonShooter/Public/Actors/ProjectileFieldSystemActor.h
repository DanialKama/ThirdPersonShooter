// All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Components/SphereComponent.h"
#include "Field/FieldSystemActor.h"
#include "ProjectileFieldSystemActor.generated.h"

UCLASS()
class THIRDPERSONSHOOTER_API AProjectileFieldSystemActor : public AFieldSystemActor
{
	GENERATED_BODY()
	
public:
	AProjectileFieldSystemActor();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	USphereComponent* Sphere;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	URadialFalloff* RadialFalloff;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UOperatorField* OperatorField;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	URadialVector* RadialVector;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	URandomVector* RandomVector;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UUniformVector* UniformVector;

	UPROPERTY(BlueprintReadOnly, Category = "Defaults")
	float StrainMagnitude = 50000.0f;
	
	UPROPERTY(BlueprintReadOnly, Category = "Defaults")
	float ForceMagnitude = 5000.0f;
	
	UPROPERTY(BlueprintReadOnly, Category = "Defaults")
	float TorqueMagnitude = 1000000.0f;
};