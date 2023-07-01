// Copyright 2022-2023 Danial Kamali. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"

/**
 * A set of tools to handle gameplay-related tasks
 */
class GameplayTools
{
public:
 static void SetFlag(int32& Target, const int Flag);
 static void UnsetFlag(int32& Target, const int Flag);
 static bool HasFlag(const int32 Target, const int Flag);
};
