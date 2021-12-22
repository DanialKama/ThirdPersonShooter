// All Rights Reserved.

#include "Core/AI/CustomAIPerceptionComponent.h"

void UCustomAIPerceptionComponent::SetStimulusAge(FAISenseID SenseID, float MaxAge)
{
	if (!ensureMsgf(SenseID.IsValid(), TEXT("Sense must exist to update max age")))
	{
		return;
	}

	if (MaxActiveAge.IsValidIndex(SenseID) == false)
	{
		MaxActiveAge.AddUninitialized(SenseID - MaxActiveAge.Num() + 1);
	}
	MaxActiveAge[SenseID] = MaxAge;
}

void UCustomAIPerceptionComponent::HandleExpiredStimulus(FAIStimulus& StimulusStore)
{
	StimulusStore.MarkExpired();
}