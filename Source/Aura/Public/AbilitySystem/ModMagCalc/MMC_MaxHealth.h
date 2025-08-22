// Copyright Druid Mechanics

#pragma once

#include "CoreMinimal.h"
#include "GameplayModMagnitudeCalculation.h"
#include "MMC_MaxHealth.generated.h"

/**
 * 
 */
UCLASS()
class AURA_API UMMC_MaxHealth : public UGameplayModMagnitudeCalculation
{
	GENERATED_BODY()
	
public:
	UMMC_MaxHealth();

	virtual float CalculateBaseMagnitude_Implementation(const FGameplayEffectSpec& Spec) const override;

private:
	/* 
	* We may want to calculate the value of one attribute based on another attribute, which we call the "backing attribute", or the attribute we wish to "capture"
	* For example, we have decided that the value of MaxHealth is dependent on Vigor, with the override calculation being of the form:
	* 
	* MaxHealth = (Coefficient*(Vigor + Pre-multiplication value)) + Post-multiplication value
	* 
	* Therefore, Vigor is the backing attribute that we wish to capture for our attribute-based calculation of MaxHealth.
	*/
	FGameplayEffectAttributeCaptureDefinition VigorDef; 
};
