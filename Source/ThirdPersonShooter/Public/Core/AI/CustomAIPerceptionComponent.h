// All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Perception/AIPerceptionComponent.h"
#include "CustomAIPerceptionComponent.generated.h"

UCLASS()
class THIRDPERSONSHOOTER_API UCustomAIPerceptionComponent : public UAIPerceptionComponent
{
	GENERATED_BODY()

public:
	/** Overriding SetMaxStimulusAge */
	virtual void SetStimulusAge(FAISenseID SenseID, float MaxAge);
	
protected:
	virtual void HandleExpiredStimulus(FAIStimulus& StimulusStore) override;
};