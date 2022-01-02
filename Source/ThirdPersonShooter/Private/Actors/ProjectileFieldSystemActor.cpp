// All Rights Reserved.

#include "Actors/ProjectileFieldSystemActor.h"

AProjectileFieldSystemActor::AProjectileFieldSystemActor()
{
	PrimaryActorTick.bCanEverTick = false;
	SetCanBeDamaged(false);
	InitialLifeSpan = 0.2f;
	
	// Create components
	Sphere = CreateDefaultSubobject<USphereComponent>(TEXT("Sphere Component"));
	RadialFalloff = CreateDefaultSubobject<URadialFalloff>(TEXT("Radial Falloff"));
	OperatorField = CreateDefaultSubobject<UOperatorField>(TEXT("Operator Field"));
	RadialVector = CreateDefaultSubobject<URadialVector>(TEXT("Radial Vector"));
	RandomVector = CreateDefaultSubobject<URandomVector>(TEXT("Random Vector"));
	UniformVector = CreateDefaultSubobject<UUniformVector>(TEXT("Uniform Vector"));
	
	// Setup components attachment
	Sphere->SetupAttachment(GetRootComponent());
	
	// Initialize
	FieldSystemComponent->SetEnableGravity(false);
	FieldSystemComponent->bApplyImpulseOnDamage = false;
	
	Sphere->SetSphereRadius(50.0f);
	Sphere->SetComponentTickEnabled(false);
	Sphere->SetEnableGravity(false);
	Sphere->SetGenerateOverlapEvents(false);
	Sphere->CanCharacterStepUpOn = ECB_No;
	Sphere->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}