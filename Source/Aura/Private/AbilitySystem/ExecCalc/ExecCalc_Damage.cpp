// Copyright Druid Mechanics


#include "AbilitySystem/ExecCalc/ExecCalc_Damage.h"
#include "AbilitySystemComponent.h"
#include "AbilitySystem/AuraAttributeSet.h"
#include "AuraGameplayTags.h"
#include "AbilitySystem/Data/CharacterClassInfo.h"
#include "AbilitySystem/AuraAbilitySystemLibrary.h"
#include "Interaction/CombatInterface.h"

struct AuraDamageStatics // Raw struct whose scope is entirely contained within this .cpp file, therefore not prefixed with F
{
	DECLARE_ATTRIBUTE_CAPTUREDEF(Armour);
	DECLARE_ATTRIBUTE_CAPTUREDEF(ArmourPierce);
	DECLARE_ATTRIBUTE_CAPTUREDEF(BlockChance);

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
		DEFINE_ATTRIBUTE_CAPTUREDEF(UAuraAttributeSet, ArmourPierce, Source, false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UAuraAttributeSet, BlockChance, Target, false);
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
	RelevantAttributesToCapture.Add(DamageStatics().ArmourPierceDef);
	RelevantAttributesToCapture.Add(DamageStatics().BlockChanceDef);
}

void UExecCalc_Damage::Execute_Implementation(const FGameplayEffectCustomExecutionParameters& ExecutionParams, 
	FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const
{
	const UAbilitySystemComponent* SourceASC = ExecutionParams.GetSourceAbilitySystemComponent();
	const UAbilitySystemComponent* TargetASC = ExecutionParams.GetTargetAbilitySystemComponent();

	AActor* SourceAvatar = SourceASC ? SourceASC->GetAvatarActor() : nullptr; // We cannot make these const if we want to get their player level via the combat interface
	AActor* TargetAvatar = TargetASC ? TargetASC->GetAvatarActor() : nullptr;

	ICombatInterface* SourceCombatInterface = Cast<ICombatInterface>(SourceAvatar);
	ICombatInterface* TargetCombatInterface = Cast<ICombatInterface>(TargetAvatar);

	const UCharacterClassInfo* CharacterClassInfo = UAuraAbilitySystemLibrary::GetCharacterClassInfo(SourceAvatar);

	const FGameplayEffectSpec& Spec = ExecutionParams.GetOwningSpec();

	const FGameplayTagContainer* SourceTags = Spec.CapturedSourceTags.GetAggregatedTags();
	const FGameplayTagContainer* TargetTags = Spec.CapturedTargetTags.GetAggregatedTags();
	FAggregatorEvaluateParameters EvaluationParameters;
	EvaluationParameters.SourceTags = SourceTags;
	EvaluationParameters.TargetTags = TargetTags;

	// Get Damage Set by Caller Magnitude
	float Damage = Spec.GetSetByCallerMagnitude(FAuraGameplayTags::Get().Damage);

	// Capture BlockChance on Target, and determine if there was a successful Block
	float TargetBlockChance = 0.f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(DamageStatics().BlockChanceDef, EvaluationParameters, TargetBlockChance); // Pass captured BlockChance attribute value into our local TargetBlockChance variable
	TargetBlockChance = FMath::Max<float>(TargetBlockChance, 0.f);

	// Halve the damage if there is a successful block
	const bool bBlocked = FMath::RandRange(0.f, 100.f) < TargetBlockChance;
	if (bBlocked) Damage *= 0.5f;

	// Capture Armour on Target and ArmourPierce on Source
	float TargetArmour = 0.f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(DamageStatics().ArmourDef, EvaluationParameters, TargetArmour); 
	TargetArmour = FMath::Max<float>(TargetArmour, 0.f);

	float SourceArmourPierce = 0.f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(DamageStatics().ArmourPierceDef, EvaluationParameters, SourceArmourPierce);
	SourceArmourPierce = FMath::Max<float>(SourceArmourPierce, 0.f);

	FRealCurve* ArmourPierceCurve = CharacterClassInfo->DamageCalculationCoefficients->FindCurve(FName("ArmourPenetration"), FString());
	const float ArmourPierceCoeff = ArmourPierceCurve->Eval(SourceCombatInterface->GetPlayerLevel());
    // ArmourPierce ignores a percentage of the Target's Armour
	const float EffectiveArmour = TargetArmour *= (100 - SourceArmourPierce * ArmourPierceCoeff) / 100.f;

	FRealCurve* EffectiveArmourCurve = CharacterClassInfo->DamageCalculationCoefficients->FindCurve(FName("EffectiveArmour"), FString());
	const float EffectiveArmourCoeff = EffectiveArmourCurve->Eval(TargetCombatInterface->GetPlayerLevel());
	// Armour ignores a percentage of incoming damage
	Damage *= (100 - EffectiveArmour * EffectiveArmourCoeff) / 100.f;

	// Fill out an EvaluatedData struct for how we want to modify our target's attribute(s), then pass it into the execution output
	const FGameplayModifierEvaluatedData EvaluatedData(UAuraAttributeSet::GetIncomingDamageAttribute(), EGameplayModOp::Additive, Damage);
	OutExecutionOutput.AddOutputModifier(EvaluatedData);
}
