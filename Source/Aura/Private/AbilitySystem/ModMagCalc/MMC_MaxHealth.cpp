// Copyright Druid Mechanics


#include "AbilitySystem/ModMagCalc/MMC_MaxHealth.h"
#include "AbilitySystem/AuraAttributeSet.h"
#include "Interaction/CombatInterface.h"

UMMC_MaxHealth::UMMC_MaxHealth()
{
	VigorDef.AttributeToCapture = UAuraAttributeSet::GetVigorAttribute(); // Static function means we don't need a pointer to the AuraAttributeSet object
	VigorDef.AttributeSource = EGameplayEffectAttributeCaptureSource::Target; // In the case of the player character, they are both the source and the target so you could pick either one here
	/*
	* "Should we capture the backing attribute when the gameplay effect spec is created (true) or when the gameplay effect spec is applied (false)?" 
	* Here it doesn't matter as the effect spec is applied immediately after it is created.
	*/
	VigorDef.bSnapshot = false; 

	RelevantAttributesToCapture.Add(VigorDef);
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

	// Pass in the evaluation parameters to capture the value of the attribute we're interested in, in this case Vigor, then store that value on our Vigor local variable.
	float Vigor = 0.f;
	GetCapturedAttributeMagnitude(VigorDef, Spec, EvaluationParameters, Vigor);
	Vigor = FMath::Max<float>(Vigor, 0.f);

	ICombatInterface* CombatInterface = Cast<ICombatInterface>(Spec.GetContext().GetSourceObject());
	const int32 PlayerLevel = CombatInterface->GetPlayerLevel();

	return 80.f + 2.5f * Vigor + 10.f * PlayerLevel; // Base vigor value of 80, with each point invested in vigor contributing 2.5, and every level contributing 10.
}
