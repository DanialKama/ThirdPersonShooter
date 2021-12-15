// All Rights Reserved.

#pragma once

#include "CharacterEnums.generated.h"

UENUM(BlueprintType, Category = "CharacterEnums")
enum class EMovementState : uint8
{
	Walk	UMETA(DisplayName = "Walk"),
	Run		UMETA(DisplayName = "Run"),
	Sprint	UMETA(DisplayName = "Sprint"),
	Crouch	UMETA(DisplayName = "Crouch"),
	Prone	UMETA(DisplayName = "Prone")
};

UENUM(BlueprintType, Category = "CharacterEnums")
enum class EInVehicleState : uint8
{
	Driver		UMETA(DisplayName = "Driver"),
	Gunner		UMETA(DisplayName = "Gunner"),
	Passenger	UMETA(DisplayName = "Passenger")
};

UENUM(BlueprintType, Category = "CharacterEnums")
enum class EHealthState : uint8
{
	Full			UMETA(DisplayName = "Health is full"),
	Low				UMETA(DisplayName = "Health is low"),
	RecoveryStarted	UMETA(DisplayName = "Health recovery started"),
	RecoveryStopped	UMETA(DisplayName = "Health recovery stopped"),
	Death			UMETA(DisplayName = "Death")
};