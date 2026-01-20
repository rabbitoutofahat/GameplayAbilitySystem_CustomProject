// Copyright Druid Mechanics


#include "AbilitySystem/ModMagCalc/MMC_MaxHealth.h"
#include "AbilitySystem/AuraAttributeSet.h"
#include "Interaction/CombatInterface.h"

UMMC_MaxHealth::UMMC_MaxHealth()
{
	MaxHealthDef.AttributeToCapture = UAuraAttributeSet::GetMaxHealthAttribute(); // Static function means we don't need a pointer to the AuraAttributeSet object
	MaxHealthDef.AttributeSource = EGameplayEffectAttributeCaptureSource::Target; // In the case of the player character, they are both the source and the target so you could pick either one here
	/*
	* "Should we capture the backing attribute when the gameplay effect spec is created (true) or when the gameplay effect spec is applied (false)?" 
	* Here it doesn't matter as the effect spec is applied immediately after it is created.
	*/
	MaxHealthDef.bSnapshot = false; 

	RelevantAttributesToCapture.Add(MaxHealthDef);
}

float UMMC_MaxHealth::CalculateBaseMagnitude_Implementation(const FGameplayEffectSpec& Spec) const
{
	// Gather attribute tags from source and target
	const FGameplayTagContainer* SourceTags = Spec.CapturedSourceTags.GetAggregatedTags(); // Aggregated tags = combination of spec and actor tags
	const FGameplayTagContainer* TargetTags = Spec.CapturedTargetTags.GetAggregatedTags();

	// Pass on the captured tags to our evaluation parameters
	FAggregatorEvaluateParameters EvaluationParameters;
	EvaluationParameters.SourceTags = SourceTags;
	EvaluationParameters.TargetTags = TargetTags;

	// Pass in the evaluation parameters to capture the value of the attribute we're interested in, in this case MaxHealth, then store that value on our MaxHealth local variable.
	float MaxHealth = 0.f;
	GetCapturedAttributeMagnitude(MaxHealthDef, Spec, EvaluationParameters, MaxHealth);
	MaxHealth = FMath::Max<float>(MaxHealth, 0.f);

	int32 PlayerLevel = 1;
	if (Spec.GetContext().GetSourceObject()->Implements<UCombatInterface>())
	{
		PlayerLevel = ICombatInterface::Execute_GetPlayerLevel(Spec.GetContext().GetSourceObject());
	}

	return 80.f + 2.5f * MaxHealth + 10.f * PlayerLevel; 
}
