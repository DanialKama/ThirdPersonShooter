// Copyright Epic Games, Inc. All Rights Reserved.

#include "Characters/BaseCharacter.h"
#include "Components/CapsuleComponent.h"
#include "PhysicsEngine/PhysicsConstraintComponent.h"
#include "Components/HealthComponent.h"
#include "Components/StaminaComponent.h"
#include "Interfaces/CommonInterface.h"
#include "Interfaces/CharacterInterface.h"
#include "Interfaces/CharacterAnimationInterface.h"
#include "Actors/PickupWeapon.h"
#include "Actors/PickupAmmo.h"

// Sets default values
ABaseCharacter::ABaseCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Create components
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
	HealthComponent = CreateDefaultSubobject<UHealthComponent>(TEXT("Health Component"));
	StaminaComponent = CreateDefaultSubobject<UStaminaComponent>(TEXT("Stamina Component"));
	
	// Setup components attachment
	FallCapsule->SetupAttachment(GetMesh());
	Root1->SetupAttachment(GetMesh(), TEXT("Weapon1Socket"));
	Hinge1->SetupAttachment(Root1);
	PhysicsConstraint1->SetupAttachment(Hinge1);
	Root2->SetupAttachment(GetMesh(), TEXT("Weapon2Socket"));
	Hinge2->SetupAttachment(Root2);
	PhysicsConstraint2->SetupAttachment(Hinge2);
	Root3->SetupAttachment(GetMesh(), TEXT("Weapon3Socket"));
	Hinge3->SetupAttachment(Root3);
	PhysicsConstraint3->SetupAttachment(Hinge3);

	// Set component defaults
	static ConstructorHelpers::FObjectFinder<UStaticMesh>CubeAsset(TEXT("StaticMesh'/Engine/BasicShapes/Cube.Cube'"));
	UStaticMesh* Cube = CubeAsset.Object;
	static ConstructorHelpers::FObjectFinder<UStaticMesh>PlaneAsset(TEXT("StaticMesh'/Engine/BasicShapes/Plane.Plane'"));
	UStaticMesh* Plane = PlaneAsset.Object;
	
	FallCapsule->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	
	Root1->SetStaticMesh(Cube);
	Root1->SetGenerateOverlapEvents(false);
	Root1->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	Root1->SetCollisionObjectType(ECC_WorldDynamic);
	Root1->SetCollisionResponseToAllChannels(ECR_Ignore);
	Root1->SetVisibility(false);
	Root1->SetHiddenInGame(true);

	Hinge1->SetStaticMesh(Plane);
	Hinge1->SetSimulatePhysics(true);
	Hinge1->SetEnableGravity(false);
	Hinge1->SetGenerateOverlapEvents(false);
	Hinge1->SetCollisionEnabled(ECollisionEnabled::PhysicsOnly);
	Hinge1->SetCollisionObjectType(ECC_WorldDynamic);
	Hinge1->SetCollisionResponseToAllChannels(ECR_Ignore);
	Hinge1->SetVisibility(false);
	Hinge1->SetHiddenInGame(true);

	PhysicsConstraint1->bUseAttachParentBound = true;
	PhysicsConstraint1->ComponentName1.ComponentName = TEXT("Hinge1");
	PhysicsConstraint1->ComponentName2.ComponentName = TEXT("Root1");
	PhysicsConstraint1->SetDisableCollision(true);
	PhysicsConstraint1->SetAngularSwing1Limit(ACM_Limited, 10.0f);
	PhysicsConstraint1->SetAngularSwing2Limit(ACM_Limited, 5.0f);
	PhysicsConstraint1->SetAngularTwistLimit(ACM_Limited, 10.0f);

	Root2->SetStaticMesh(Cube);
	Root2->SetGenerateOverlapEvents(false);
	Root2->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	Root2->SetCollisionObjectType(ECC_WorldDynamic);
	Root2->SetCollisionResponseToAllChannels(ECR_Ignore);
	Root2->SetVisibility(false);
	Root2->SetHiddenInGame(true);
	
	Hinge2->SetStaticMesh(Plane);
	Hinge2->SetSimulatePhysics(true);
	Hinge2->SetEnableGravity(false);
	Hinge2->SetGenerateOverlapEvents(false);
	Hinge2->SetCollisionEnabled(ECollisionEnabled::PhysicsOnly);
	Hinge2->SetCollisionObjectType(ECC_WorldDynamic);
	Hinge2->SetCollisionResponseToAllChannels(ECR_Ignore);
	Hinge2->SetVisibility(false);
	Hinge2->SetHiddenInGame(true);

	PhysicsConstraint2->bUseAttachParentBound = true;
	PhysicsConstraint2->ComponentName1.ComponentName = TEXT("Hinge1");
	PhysicsConstraint2->ComponentName2.ComponentName = TEXT("Root1");
	PhysicsConstraint2->SetDisableCollision(true);
	PhysicsConstraint2->SetAngularSwing1Limit(ACM_Limited, 10.0f);
	PhysicsConstraint2->SetAngularSwing2Limit(ACM_Limited, 5.0f);
	PhysicsConstraint2->SetAngularTwistLimit(ACM_Limited, 10.0f);

	Root3->SetStaticMesh(Cube);
	Root3->SetGenerateOverlapEvents(false);
	Root3->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	Root3->SetCollisionObjectType(ECC_WorldDynamic);
	Root3->SetCollisionResponseToAllChannels(ECR_Ignore);
	Root3->SetVisibility(false);
	Root3->SetHiddenInGame(true);
	
	Hinge3->SetStaticMesh(Plane);
	Hinge3->SetSimulatePhysics(true);
	Hinge3->SetEnableGravity(false);
	Hinge3->SetGenerateOverlapEvents(false);
	Hinge3->SetCollisionEnabled(ECollisionEnabled::PhysicsOnly);
	Hinge3->SetCollisionObjectType(ECC_WorldDynamic);
	Hinge3->SetCollisionResponseToAllChannels(ECR_Ignore);
	Hinge3->SetVisibility(false);
	Hinge3->SetHiddenInGame(true);

	PhysicsConstraint3->bUseAttachParentBound = true;
	PhysicsConstraint3->ComponentName1.ComponentName = TEXT("Hinge1");
	PhysicsConstraint3->ComponentName2.ComponentName = TEXT("Root1");
	PhysicsConstraint3->SetDisableCollision(true);
	PhysicsConstraint3->SetAngularSwing1Limit(ACM_Limited, 10.0f);
	PhysicsConstraint3->SetAngularSwing2Limit(ACM_Limited, 5.0f);
	PhysicsConstraint3->SetAngularTwistLimit(ACM_Limited, 10.0f);
	
	// Overlap functions
	FallCapsule->OnComponentBeginOverlap.AddDynamic(this, &ABaseCharacter::OnFallCapsuleBeginOverlap);

	// Initialize variables
	bIsAlive = true;
	bDoOnceMoving = true;
	bDoOnceStopped = true;
}

// Called when the game starts or when spawned
void ABaseCharacter::BeginPlay()
{
	Super::BeginPlay();

	AnimInstance = GetMesh()->GetAnimInstance();
	// Detected if the interfaces is present on anim instance
	if(AnimInstance->GetClass()->ImplementsInterface(UCharacterAnimationInterface::StaticClass()))
	{
		bCharacterAnimationInterface = true;
	}
	
	CharacterTagContainer.AddTag(TeamTag);
	FallCapsule->SetRelativeLocation(MeshLocationOffset);
	FallCapsule->IgnoreActorWhenMoving(this, true); // Fall Capsule should ignore self to not register a fall when overlap with self

	// Create material instances for every material on mesh, mainly used for death dither effect
	TArray<UMaterialInterface*> Materials = GetMesh()->GetMaterials();
	for (int32 i = 0; i < Materials.Num(); i++)
	{
		Materials.Add(GetMesh()->CreateDynamicMaterialInstance(i, Materials[i]));
	}

	HealthComponent->Initialize();
	StaminaComponent->Initialize();
}

// Called every frame
void ABaseCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// Check if character stopped or moving
	if(GetVelocity().Size() == 0.0f)
	{
		if(bDoOnceStopped)
		{
			if(bCharacterAnimationInterface)
			{
				ICharacterAnimationInterface::Execute_SetMovementState(AnimInstance, PreviousMovementState);
			}
			
			GetWorld()->GetTimerManager().SetTimer(IdleTimer, this, &ABaseCharacter::PlayIdleAnimation, FMath::FRandRange(60.0f, 90.0f), true);
			bDoOnceMoving = true;
		}
	}
	else
	{
		if(bDoOnceMoving)
		{
			CharacterIsOnMove();
			bDoOnceStopped = true;
		}
	}

	// If ragdoll state is true then keep mesh and capsule together
	if(bRagdollState)
	{
		// If this is not the player then no need to set capsule location after character death
		if(bIsAlive || IsPlayerControlled())
		{
			CalculateCapsuleLocation();
			GetCapsuleComponent()->SetWorldLocation(MeshLocation);
		}
	}
}

// Called to bind functionality to input
void ABaseCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

void ABaseCharacter::CalculateCapsuleLocation()
{
	FVector CapsuleLocation;
	FHitResult HitResult;
	const FVector Start = GetMesh()->GetSocketLocation(TEXT("pelvis"));
	const FVector End = Start + FVector(0.0f, 0.0f, -1.0f) * 100.0f;
	FCollisionQueryParams CollisionQueryParams;
	CollisionQueryParams.AddIgnoredActor(this);
	
	const bool bHit = GetWorld()->LineTraceSingleByChannel(HitResult, Start, End, ECC_Visibility, CollisionQueryParams);

	if(bHit)
	{
		CapsuleLocation = HitResult.Location + MeshLocationOffset;
	}
	else
	{
		CapsuleLocation = GetMesh()->GetSocketLocation(TEXT("pelvis")) + MeshLocationOffset;
	}
	
	MeshLocation = FMath::VInterpTo(MeshLocation, CapsuleLocation,  GetWorld()->GetDeltaSeconds(), 2.5f);
}

void ABaseCharacter::PlayIdleAnimation()
{
	
}

void ABaseCharacter::CharacterIsOnMove()
{
	
}

void ABaseCharacter::SetMovementState_Implementation(EMovementState InMovementState, bool bRelatedToCrouch, bool bRelatedToProne)
{
	
}

void ABaseCharacter::Interact_Implementation()
{
	if(Pickup)
	{
		switch(ItemType)
		{
		case 0:
			// Weapon
			APickupWeapon* Weapon;
			Weapon = Cast<APickupWeapon>(Pickup);
			PickupWeapon(Weapon);
			break;
		case 1:
			// Ammo
			APickupAmmo* Ammo;
			Ammo = Cast<APickupAmmo>(Pickup);
			PickupAmmo(Ammo);
			break;
		case 2:
			// Health
			break;
		}
	}
}

void ABaseCharacter::PickupWeapon(APickupWeapon* NewWeapon)
{
	
}

void ABaseCharacter::PickupAmmo(APickupAmmo* NewAmmo)
{
	
}

// Overlaps
void ABaseCharacter::OnFallCapsuleBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
	UPrimitiveComponent* OtherComp,int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	
}