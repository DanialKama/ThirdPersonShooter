// Copyright 2022-2023 Danial Kamali. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Field/FieldSystemActor.h"
#include "ProjectileFieldSystemActor.generated.h"

// TODO: Rename to AProjectileFieldSystem
UCLASS(meta = (DisplayName = "Projectile Field System"))
class AProjectileFieldSystemActor : public AFieldSystemActor
{
	GENERATED_BODY()

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = true))
	TObjectPtr<class USphereComponent> Sphere;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = true))
	TObjectPtr<class URadialFalloff> RadialFalloff;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = true))
	TObjectPtr<class UOperatorField> OperatorField;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = true))
	TObjectPtr<class URadialVector> RadialVector;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = true))
	TObjectPtr<class URandomVector> RandomVector;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = true))
	TObjectPtr<class UUniformVector> UniformVector;

// Functions
public:
	AProjectileFieldSystemActor();

// Variables
public:
	UPROPERTY(BlueprintReadOnly, Category = "Default")
	float StrainMagnitude = 50000.0f;
	
	UPROPERTY(BlueprintReadOnly, Category = "Default")
	float ForceMagnitude = 5000.0f;
	
	UPROPERTY(BlueprintReadOnly, Category = "Default")
	float TorqueMagnitude = 1000000.0f;
};
