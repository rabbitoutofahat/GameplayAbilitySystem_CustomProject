// Roland Frances

#pragma once

#include "CoreMinimal.h"
#include "GameplayModMagnitudeCalculation.h"
#include "MMC_SummonDreglingCost.generated.h"

/**
 * 
 */
UCLASS()
class AURA_API UMMC_SummonDreglingCost : public UGameplayModMagnitudeCalculation
{
	GENERATED_BODY()

public:
	virtual float CalculateBaseMagnitude_Implementation(const FGameplayEffectSpec& Spec) const override;
	
};
