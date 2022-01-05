// All Rights Reserved.

#include "Components/BaseComponent.h"

UBaseComponent::UBaseComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UBaseComponent::Initialize()
{
	Owner = GetOwner();
	SetupComponent();
}

void UBaseComponent::SetupComponent()
{
}