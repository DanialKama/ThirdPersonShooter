// All Rights Reserved.

#include "Actors/ProjectileFieldSystemActor.h"
#include "Components/SphereComponent.h"

AProjectileFieldSystemActor::AProjectileFieldSystemActor()
{
	PrimaryActorTick.bCanEverTick = false;
	SetCanBeDamaged(false);
	InitialLifeSpan = 0.2f;
	
	FieldSystemComponent->SetEnableGravity(false);
	FieldSystemComponent->bApplyImpulseOnDamage = false;
	
	Sphere = CreateDefaultSubobject<USphereComponent>(TEXT("Sphere Component"));
	Sphere->SetupAttachment(GetRootComponent());
	Sphere->SetComponentTickEnabled(false);
	Sphere->SetSphereRadius(50.0f);
	Sphere->SetComponentTickEnabled(false);
	Sphere->SetEnableGravity(false);
	Sphere->SetGenerateOverlapEvents(false);
	Sphere->CanCharacterStepUpOn = ECB_No;
	Sphere->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	
	RadialFalloff = CreateDefaultSubobject<URadialFalloff>(TEXT("Radial Falloff"));
	
	OperatorField = CreateDefaultSubobject<UOperatorField>(TEXT("Operator Field"));
	
	RadialVector = CreateDefaultSubobject<URadialVector>(TEXT("Radial Vector"));
	
	RandomVector = CreateDefaultSubobject<URandomVector>(TEXT("Random Vector"));
	
	UniformVector = CreateDefaultSubobject<UUniformVector>(TEXT("Uniform Vector"));

	// Initialize variables
	StrainMagnitude = 50000.0f;
	ForceMagnitude = 5000.0f;
	TorqueMagnitude = 1000000.0f;
}