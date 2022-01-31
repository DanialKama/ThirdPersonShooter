// Copyright 2022 Danial Kamali. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Field/FieldSystemActor.h"
#include "ProjectileFieldSystemActor.generated.h"

UCLASS()
class THIRDPERSONSHOOTER_API AProjectileFieldSystemActor : public AFieldSystemActor
{
	GENERATED_BODY()

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = true))
	class USphereComponent* Sphere;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = true))
	URadialFalloff* RadialFalloff;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = true))
	UOperatorField* OperatorField;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = true))
	URadialVector* RadialVector;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = true))
	URandomVector* RandomVector;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = true))
	UUniformVector* UniformVector;

// Functions
public:
	AProjectileFieldSystemActor();

// Variables
public:
	UPROPERTY(BlueprintReadOnly, Category = "Defaults")
	float StrainMagnitude;
	
	UPROPERTY(BlueprintReadOnly, Category = "Defaults")
	float ForceMagnitude;
	
	UPROPERTY(BlueprintReadOnly, Category = "Defaults")
	float TorqueMagnitude;
};