
#include "Components/BaseComponent.h"

// Sets default values for this component's properties
UBaseComponent::UBaseComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
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
