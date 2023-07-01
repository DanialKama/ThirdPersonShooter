// Copyright 2022-2023 Danial Kamali. All Rights Reserved.

#include "GameplayTools.h"

void GameplayTools::SetFlag(int32& Target, const int Flag)
{
	Target |= Flag;
}

void GameplayTools::UnsetFlag(int32& Target, const int Flag)
{
	Target &= ~Flag;
}

bool GameplayTools::HasFlag(const int32 Target, const int Flag)
{
	return (Target & Flag) == Flag;
}
