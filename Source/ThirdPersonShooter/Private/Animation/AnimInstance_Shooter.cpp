// Copyright 2022-2023 Danial Kamali. All Rights Reserved.

#include "AnimInstance_Shooter.h"

#include "Actors/Interactable/PickupWeapon.h"
#include "Characters/BaseCharacter.h"
#include "Engine/AssetManager.h"
#include "Engine/StreamableManager.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "PhysicalMaterials/PhysicalMaterial.h"

UAnimInstance_Shooter::UAnimInstance_Shooter()
{
	// Initialize variables
	bIsFalling = false;
	bIsCrouched = false;
	bIsArmed = false;
	bIsAiming = false;
}

void UAnimInstance_Shooter::NativeInitializeAnimation()
{
	CharacterOwner = Cast<ABaseCharacter>(GetSkelMeshComponent()->GetOwner());
	
	if (CharacterOwner && GetWorld()->HasBegunPlay())
	{
		LoadAssets();
	}
}

void UAnimInstance_Shooter::NativeUpdateAnimation(float DeltaSeconds)
{
	if (CharacterOwner == nullptr)
	{
		return;
	}

	if (CharacterOwner->CurrentWeapon)
	{
		bIsArmed = true;
		bIsAiming = CharacterOwner->bIsAiming;
		
		if (bIsAiming)
		{
			// Base aim rotation
			AimPitch = FRotator::NormalizeAxis(CharacterOwner->Controller->GetControlRotation().Pitch);
			RecoilAlpha = FMath::FInterpConstantTo(RecoilAlpha, 0.0f, DeltaSeconds, RecoilControlTime);
			LeftHandLocation = CharacterOwner->CurrentWeapon->SkeletalMesh->GetSocketLocation(FName("LeftHandAimSocket"));
		}
		else
		{
			LeftHandLocation = CharacterOwner->CurrentWeapon->SkeletalMesh->GetSocketLocation(FName("LeftHandSocket"));
		}
	}
	else
	{
		bIsArmed = false;
	}

	bIsFalling = CharacterOwner->GetCharacterMovement()->MovementMode == MOVE_Falling;
	if (bIsFalling)
	{
		return;
	}

	bIsCrouched = CharacterOwner->bIsCrouched;

	Speed = FVector::DotProduct(CharacterOwner->GetCharacterMovement()->Velocity, CharacterOwner->GetRootComponent()->GetForwardVector());
	Direction = FVector::DotProduct(CharacterOwner->GetCharacterMovement()->Velocity, CharacterOwner->GetRootComponent()->GetRightVector());
}

void UAnimInstance_Shooter::LoadAssets()
{
	TArray<FSoftObjectPath> Paths;
	
	WalkOnDirtLoaded = WalkOnDirt.Get();
	if (WalkOnDirtLoaded == nullptr)
	{
		Paths.Add(WalkOnDirt.ToSoftObjectPath());
	}

	WalkOnStoneLoaded = WalkOnStone.Get();
	if (WalkOnStoneLoaded == nullptr)
	{
		Paths.Add(WalkOnStone.ToSoftObjectPath());
	}

	WalkOnWoodLoaded = WalkOnWood.Get();
	if (WalkOnWoodLoaded == nullptr)
	{
		Paths.Add(WalkOnWood.ToSoftObjectPath());
	}
	
	RunOnDirtLoaded = RunOnDirt.Get();
	if (RunOnDirtLoaded == nullptr)
	{
		Paths.Add(RunOnDirt.ToSoftObjectPath());
	}

	RunOnStoneLoaded = RunOnStone.Get();
	if (RunOnStoneLoaded == nullptr)
	{
		Paths.Add(RunOnStone.ToSoftObjectPath());
	}
	
	RunOnWoodLoaded = RunOnWood.Get();
	if (RunOnWoodLoaded == nullptr)
	{
		Paths.Add(RunOnWood.ToSoftObjectPath());
	}
	
	if (Paths.IsEmpty())
	{
		return;
	}

	UAssetManager::GetStreamableManager().RequestAsyncLoad(Paths, FStreamableDelegate::CreateWeakLambda(this, [this]
	{
		if (WalkOnDirtLoaded == nullptr)
		{
			WalkOnDirtLoaded = WalkOnDirt.Get();
		}

		if (WalkOnStoneLoaded == nullptr)
		{
			WalkOnStoneLoaded = WalkOnStone.Get();
		}

		if (WalkOnWoodLoaded == nullptr)
		{
			WalkOnWoodLoaded = WalkOnWood.Get();
		}
		
		if (RunOnDirtLoaded == nullptr)
		{
			RunOnDirtLoaded = RunOnDirt.Get();
		}

		if (RunOnStoneLoaded == nullptr)
		{
			RunOnStoneLoaded = RunOnStone.Get();
		}
		
		if (RunOnWoodLoaded == nullptr)
		{
			RunOnWoodLoaded = RunOnWood.Get();
		}
	}));
}

void UAnimInstance_Shooter::PlayFootstepSound(bool bIsWalking) const
{
	UWorld* MyWorld = GetWorld();
	
	const FVector Start = GetSkelMeshComponent()->GetOwner()->GetActorLocation();

	FCollisionQueryParams CollisionParams;
	// Ignore owner
	CollisionParams.AddIgnoredActor(GetSkelMeshComponent()->GetOwner());
	
	FHitResult HitResult;
	if (MyWorld->LineTraceSingleByChannel(HitResult, Start, Start - FVector(0.0f, 0.0f, 150.0f), ECC_Visibility, CollisionParams) == false)
	{
		return;
	}
	
	USoundBase* SoundToPlay;

	// TODO: Fix switch statements
	if (bIsWalking)
	{
		switch (HitResult.PhysMaterial->SurfaceType)
		{
			case SurfaceType1:
				SoundToPlay = WalkOnDirtLoaded;
				break;
			case SurfaceType2:
				SoundToPlay = WalkOnStoneLoaded;
				break;
			case SurfaceType3:
				SoundToPlay = WalkOnWoodLoaded;
				break;
			default:
				SoundToPlay = WalkOnDirtLoaded;
		}
	}
	else
	{
		switch (HitResult.PhysMaterial->SurfaceType)
		{
			case SurfaceType1:
				SoundToPlay = RunOnDirtLoaded;
				break;
			case SurfaceType2:
				SoundToPlay = RunOnStoneLoaded;
				break;
			case SurfaceType3:
				SoundToPlay = RunOnWoodLoaded;
				break;
			default:
				SoundToPlay = RunOnDirtLoaded;
		}
	}

	UGameplayStatics::PlaySoundAtLocation(MyWorld, SoundToPlay, HitResult.ImpactPoint);
}
