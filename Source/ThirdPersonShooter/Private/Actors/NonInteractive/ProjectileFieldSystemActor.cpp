// Copyright 2022-2023 Danial Kamali. All Rights Reserved.

#include "ProjectileFieldSystemActor.h"

#include "Components/SphereComponent.h"
#include "Field/FieldSystemObjects.h"

AProjectileFieldSystemActor::AProjectileFieldSystemActor()
{
	PrimaryActorTick.bCanEverTick = false;
	PrimaryActorTick.bStartWithTickEnabled = false;
	
	SetCanBeDamaged(false);

	// TODO: Destroy it after applying the effect
	InitialLifeSpan = 0.2f;
	
	Sphere = CreateDefaultSubobject<USphereComponent>(TEXT("Sphere Component"));
	Sphere->SetupAttachment(GetRootComponent());
	Sphere->SetSphereRadius(50.0f);
	Sphere->SetGenerateOverlapEvents(false);
	Sphere->CanCharacterStepUpOn = ECB_No;
	Sphere->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	
	RadialFalloff = CreateDefaultSubobject<URadialFalloff>(TEXT("Radial Falloff"));
	
	OperatorField = CreateDefaultSubobject<UOperatorField>(TEXT("Operator Field"));
	
	RadialVector = CreateDefaultSubobject<URadialVector>(TEXT("Radial Vector"));
	
	RandomVector = CreateDefaultSubobject<URandomVector>(TEXT("Random Vector"));
	
	UniformVector = CreateDefaultSubobject<UUniformVector>(TEXT("Uniform Vector"));
}
