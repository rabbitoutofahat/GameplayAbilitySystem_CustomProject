// Roland Frances


#include "AbilitySystem/ModMagCalc/MMC_HealthRedistribution.h"
#include "AbilitySystem/AuraAttributeSet.h"

UMMC_HealthRedistribution::UMMC_HealthRedistribution()
{
	SourceHealthDef.AttributeToCapture = UAuraAttributeSet::GetHealthAttribute(); 
	SourceHealthDef.AttributeSource = EGameplayEffectAttributeCaptureSource::Source; 
	SourceHealthDef.bSnapshot = false;

	TargetHealthDef.AttributeToCapture = UAuraAttributeSet::GetHealthAttribute();
	TargetHealthDef.AttributeSource = EGameplayEffectAttributeCaptureSource::Target;
	TargetHealthDef.bSnapshot = false;

	RelevantAttributesToCapture.Add(SourceHealthDef);
	RelevantAttributesToCapture.Add(TargetHealthDef);
}

float UMMC_HealthRedistribution::CalculateBaseMagnitude_Implementation(const FGameplayEffectSpec& Spec) const
{
	const FGameplayTagContainer* SourceTags = Spec.CapturedSourceTags.GetAggregatedTags();
	const FGameplayTagContainer* TargetTags = Spec.CapturedTargetTags.GetAggregatedTags();

	FAggregatorEvaluateParameters EvaluationParameters;
	EvaluationParameters.SourceTags = SourceTags;
	EvaluationParameters.TargetTags = TargetTags;

	float SourceHealth = 0.f;
	GetCapturedAttributeMagnitude(SourceHealthDef, Spec, EvaluationParameters, SourceHealth);
	SourceHealth = FMath::Max<float>(SourceHealth, 0.f);

	float TargetHealth = 0.f;
	GetCapturedAttributeMagnitude(TargetHealthDef, Spec, EvaluationParameters, TargetHealth);
	TargetHealth = FMath::Max<float>(TargetHealth, 0.f);

	return 0.5f * (SourceHealth + TargetHealth); // Apply Gameplay Effect to both Source and Target
}