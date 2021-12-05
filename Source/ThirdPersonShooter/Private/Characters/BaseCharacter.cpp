// Copyright Epic Games, Inc. All Rights Reserved.

#include "Characters/BaseCharacter.h"
#include "Components/CapsuleComponent.h"
#include "PhysicsEngine/PhysicsConstraintComponent.h"

// Sets default values
ABaseCharacter::ABaseCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Create components
	// CapsuleComponent->InitCapsuleSize(35.0f, 90.0f);
	FallCapsule = CreateDefaultSubobject<UCapsuleComponent>(TEXT("Fall Capsule"));
	Root1 = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Root 1"));
	Hinge1 = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Hinge 1"));
	PhysicsConstraint1 = CreateDefaultSubobject<UPhysicsConstraintComponent>(TEXT("Physics Constraint 1"));
	Root2 = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Root 2"));
	Hinge2 = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Hinge 2"));
	PhysicsConstraint2 = CreateDefaultSubobject<UPhysicsConstraintComponent>(TEXT("Physics Constraint 2"));
	Root3 = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Root 3"));
	Hinge3 = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Hinge 3"));
	PhysicsConstraint3 = CreateDefaultSubobject<UPhysicsConstraintComponent>(TEXT("Physics Constraint 3"));
	
	// Setup components attachment	TODO
	// FallCapsule->SetupAttachment(Mesh);
	// Root1->SetupAttachment(Mesh, TEXT("Weapon1Socket"));
	// Hinge1->SetupAttachment(Root1);
	// PhysicsConstraint1->SetupAttachment(Hinge1);
	// Root2->SetupAttachment(Mesh, TEXT("Weapon2Socket"));
	// Hinge2->SetupAttachment(Root2);
	// PhysicsConstraint2->SetupAttachment(Hinge2);
	// Root3->SetupAttachment(Mesh, TEXT("Weapon3Socket"));
	// Hinge3->SetupAttachment(Root3);
	// PhysicsConstraint3->SetupAttachment(Hinge3);

	// Set component defaults
	// FTransform NewTransform = FTransform().SetLocation(FVector(0.0f, 0.0f, 90.0f));
	// FallCapsule->SetRelativeTransform(FTransform(FVector(0.0f, 0.0f, 90.0f), FRotator(0.0f, 0.0f, 90.0f)));
	// FallCapsule->InitCapsuleSize(55.0f, 110.0f);
	// FallCapsule->SetEnableGravity(false);
	// FallCapsule->SetConstraintMode(EDOFMode::None);
	// FallCapsule->bIgnoreRadialImpulse = true;
	// FallCapsule->bIgnoreRadialForce = true;
	// FallCapsule->bApplyImpulseOnDamage = false;

	// Root1->SetStaticMesh(LoadObject<UStaticMeshComponent>()); TODO
	// Root1->SetConstraintMode(EDOFMode::None);
	// Root1->SetGenerateOverlapEvents(false);
	// Root1->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	// Root1->SetCollisionObjectType(ECC_WorldDynamic);
	// Root1->SetCollisionResponseToAllChannels(ECR_Ignore);
	// Root1->SetVisibility(false);
	// Root1->SetHiddenInGame(true);

	// Hinge1->SetSimulatePhysics(true);
	// Hinge1->SetLinearDamping(10.0f);
	// Hinge1->SetEnableGravity(false);
	// Hinge1->SetConstraintMode(EDOFMode::None);
	// Hinge1->SetGenerateOverlapEvents(false);
	// Hinge1->SetCollisionEnabled(ECollisionEnabled::PhysicsOnly);
	// Hinge1->SetCollisionObjectType(ECC_WorldDynamic);
	// Hinge1->SetCollisionResponseToAllChannels(ECR_Ignore);
	// Hinge1->SetVisibility(false);
	// Hinge1->SetHiddenInGame(true);

	// PhysicsConstraint1->bUseAttachParentBound = true;
	// PhysicsConstraint1->ComponentName1.ComponentName = TEXT("Hinge1");
	// PhysicsConstraint1->ComponentName2.ComponentName = TEXT("Root1");
	// PhysicsConstraint1->SetDisableCollision(true);	// Set parents dominates to true
	// PhysicsConstraint1->SetAngularSwing1Limit(ACM_Limited, 10.0f);
	// PhysicsConstraint1->SetAngularSwing2Limit(ACM_Limited, 5.0f);
	// PhysicsConstraint1->SetAngularTwistLimit(ACM_Limited, 10.0f);
	
	// Component Overlap


	// Set value defaults

}

// Called when the game starts or when spawned
void ABaseCharacter::BeginPlay()
{
	Super::BeginPlay();

	// Create components
}

// Called every frame
void ABaseCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ABaseCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}