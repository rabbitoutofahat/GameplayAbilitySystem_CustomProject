// Copyright Druid Mechanics


#include "AbilitySystem/ExecCalc/ExecCalc_Damage.h"
#include "AbilitySystemComponent.h"
#include "AbilitySystem/AuraAttributeSet.h"

struct AuraDamageStatics // Raw struct whose scope is entirely contained within this .cpp file, therefore not prefixed with F
{
	DECLARE_ATTRIBUTE_CAPTUREDEF(Armour);

	AuraDamageStatics()
	{
		/*
		* Capture definition for an attribute takes the input parameters:
		* (S) Class that owns the attribute
		* (P) Attribute we want to capture
		* (T) Whether we want to capture the attribute from the source or the target
		* (B) Whether we want to snapshot the attribute when the gameplay effect spec is created (true) or when it is applied (false)
		*/
		DEFINE_ATTRIBUTE_CAPTUREDEF(UAuraAttributeSet, Armour, Target, false);
	}
};

static const AuraDamageStatics& DamageStatics() // Function that returns a reference to a static instance of the struct
{
	static AuraDamageStatics DStatics; // Static variable, instantiated only once and persists for the lifetime of the program
	return DStatics;
}

UExecCalc_Damage::UExecCalc_Damage()
{
	RelevantAttributesToCapture.Add(DamageStatics().ArmourDef);
}

void UExecCalc_Damage::Execute_Implementation(const FGameplayEffectCustomExecutionParameters& ExecutionParams, 
	FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const
{
	const UAbilitySystemComponent* SourceASC = ExecutionParams.GetSourceAbilitySystemComponent();
	const UAbilitySystemComponent* TargetASC = ExecutionParams.GetTargetAbilitySystemComponent();

	const AActor* SourceAvatar = SourceASC ? SourceASC->GetAvatarActor() : nullptr;
	const AActor* TargetAvatar = TargetASC ? TargetASC->GetAvatarActor() : nullptr;

	const FGameplayEffectSpec& Spec = ExecutionParams.GetOwningSpec();

	const FGameplayTagContainer* SourceTags = Spec.CapturedSourceTags.GetAggregatedTags();
	const FGameplayTagContainer* TargetTags = Spec.CapturedTargetTags.GetAggregatedTags();
	FAggregatorEvaluateParameters EvaluationParameters;
	EvaluationParameters.SourceTags = SourceTags;
	EvaluationParameters.TargetTags = TargetTags;

	float Armour = 0.f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(DamageStatics().ArmourDef, EvaluationParameters, Armour);
	Armour = FMath::Max<float>(Armour, 0.f);
	++Armour;

	// Fill out an EvaluatedData struct for how we want to modify our target's Armour attribute, then pass it into the execution output
	const FGameplayModifierEvaluatedData EvaluatedData(DamageStatics().ArmourProperty, EGameplayModOp::Additive, Armour);
	OutExecutionOutput.AddOutputModifier(EvaluatedData);
}
