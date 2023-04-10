// Copyright 2022-2023 Danial Kamali. All Rights Reserved.

#include "BaseComponent.h"

// TODO: Do not use base component
UBaseComponent::UBaseComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
	PrimaryComponentTick.bStartWithTickEnabled = false;
}
