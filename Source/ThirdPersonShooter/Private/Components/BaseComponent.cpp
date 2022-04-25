// Copyright 2022 Danial Kamali. All Rights Reserved.

#include "Components/BaseComponent.h"

UBaseComponent::UBaseComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UBaseComponent::Initialize()
{
}