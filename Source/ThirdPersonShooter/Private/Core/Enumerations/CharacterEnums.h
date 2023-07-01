// Copyright 2022-2023 Danial Kamali. All Rights Reserved.

#pragma once

#include "CharacterEnums.generated.h"

UENUM(BlueprintType, Category = "CharacterEnums")
enum class EMovementState : uint8
{
	Walk	UMETA(DisplayName = "Walk"),
	Run		UMETA(DisplayName = "Run"),
	Sprint	UMETA(DisplayName = "Sprint"),
	Crouch	UMETA(DisplayName = "Crouch")
};
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

UENUM(BlueprintType, Category = "CharacterEnums")
enum class EAIState : uint8
{
	Idle		UMETA(DisplayNAme = "Idle"),
	Fight		UMETA(DisplayNAme = "Fight"),
	Search		UMETA(DisplayNAme = "Search"),
	Reload		UMETA(DisplayNAme = "Reload"),
	Switch		UMETA(DisplayNAme = "Switch Weaopn"),
	LowHealth	UMETA(DisplayName = "Low Health"),
	UseMed		UMETA(DisplayNAme = "Using Med")
};
