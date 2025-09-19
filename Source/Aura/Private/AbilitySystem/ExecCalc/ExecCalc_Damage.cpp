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
	DECLARE_ATTRIBUTE_CAPTUREDEF(CritChance);
	DECLARE_ATTRIBUTE_CAPTUREDEF(CritRes);
	DECLARE_ATTRIBUTE_CAPTUREDEF(CritDamage);

	DECLARE_ATTRIBUTE_CAPTUREDEF(FireRes);
	DECLARE_ATTRIBUTE_CAPTUREDEF(LightningRes);
	DECLARE_ATTRIBUTE_CAPTUREDEF(ArcaneRes);
	DECLARE_ATTRIBUTE_CAPTUREDEF(PhysicalRes);

	// Important to map Tags to Capture Definitions so when we're looping through Set By Caller Magnitudes we can find the corresponding Resistance Attribute to capture
	TMap<FGameplayTag, FGameplayEffectAttributeCaptureDefinition> TagsToCaptureDefs;

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
		DEFINE_ATTRIBUTE_CAPTUREDEF(UAuraAttributeSet, CritChance, Source, false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UAuraAttributeSet, CritRes, Target, false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UAuraAttributeSet, CritDamage, Source, false);

		DEFINE_ATTRIBUTE_CAPTUREDEF(UAuraAttributeSet, FireRes, Target, false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UAuraAttributeSet, LightningRes, Target, false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UAuraAttributeSet, ArcaneRes, Target, false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UAuraAttributeSet, PhysicalRes, Target, false);

		const FAuraGameplayTags& Tags = FAuraGameplayTags::Get();

		TagsToCaptureDefs.Add(Tags.Attributes_Secondary_Armour, ArmourDef);
		TagsToCaptureDefs.Add(Tags.Attributes_Secondary_ArmourPenetration, ArmourPierceDef);
		TagsToCaptureDefs.Add(Tags.Attributes_Secondary_BlockChance, BlockChanceDef);
		TagsToCaptureDefs.Add(Tags.Attributes_Secondary_CriticalHitChance, CritChanceDef);
		TagsToCaptureDefs.Add(Tags.Attributes_Secondary_CriticalHitResistance, CritResDef);
		TagsToCaptureDefs.Add(Tags.Attributes_Secondary_CriticalHitDamage, CritDamageDef);

		TagsToCaptureDefs.Add(Tags.Attributes_Resistance_Fire, FireResDef);
		TagsToCaptureDefs.Add(Tags.Attributes_Resistance_Lightning, LightningResDef);
		TagsToCaptureDefs.Add(Tags.Attributes_Resistance_Arcane, ArcaneResDef);
		TagsToCaptureDefs.Add(Tags.Attributes_Resistance_Physical, PhysicalResDef);
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
	RelevantAttributesToCapture.Add(DamageStatics().CritChanceDef);
	RelevantAttributesToCapture.Add(DamageStatics().CritResDef);
	RelevantAttributesToCapture.Add(DamageStatics().CritDamageDef);

	RelevantAttributesToCapture.Add(DamageStatics().FireResDef);
	RelevantAttributesToCapture.Add(DamageStatics().LightningResDef);
	RelevantAttributesToCapture.Add(DamageStatics().ArcaneResDef);
	RelevantAttributesToCapture.Add(DamageStatics().PhysicalResDef);
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
	FGameplayEffectContextHandle EffectContextHandle = Spec.GetContext();

	// Take the source and target tags from the spec and pass them into the evaluation parameters for AttemptCalculateCapturedAttributeMagnitude()
	const FGameplayTagContainer* SourceTags = Spec.CapturedSourceTags.GetAggregatedTags();
	const FGameplayTagContainer* TargetTags = Spec.CapturedTargetTags.GetAggregatedTags();
	FAggregatorEvaluateParameters EvaluationParameters;
	EvaluationParameters.SourceTags = SourceTags;
	EvaluationParameters.TargetTags = TargetTags;

	// Get Damage Set by Caller Magnitude for each Damage Type found in our DamageTypesToResistances container
	float Damage = 0.f;
	for (const auto& Pair : FAuraGameplayTags::Get().DamageTypesToResistances)
	{
		const FGameplayTag DamageTypeTag = Pair.Key;
		const FGameplayTag ResistanceTag = Pair.Value;

		checkf(AuraDamageStatics().TagsToCaptureDefs.Contains(ResistanceTag), TEXT("TagsToCaptureDefs doesn't contain Tag: [%s] in ExecCalc_Damage"), *ResistanceTag.ToString());
		const FGameplayEffectAttributeCaptureDefinition CaptureDef = AuraDamageStatics().TagsToCaptureDefs[ResistanceTag]; // We've mapped damage types to resistances, then use the map of tags to capture defintions on the resistance tags
        
		float Resistance = 0.f;
		ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(CaptureDef, EvaluationParameters, Resistance);
		Resistance = FMath::Clamp<float>(Resistance, 0.f, 100.f);

		// Get the Gameplay Ability's Set By Caller Magnitudes for each Damage Type Tag. For example, the only non-zero Magnitude for FireBolt should be for the Damage_Fire tag
		float DamageTypeValue = Spec.GetSetByCallerMagnitude(DamageTypeTag, false); // bool WarnIfNotFound set to false to turn off log warnings
		DamageTypeValue += (100.f - Resistance) / 100.f;
		Damage += DamageTypeValue;
	}

	// Capture BlockChance on Target, and determine if there was a successful Block
	float TargetBlockChance = 0.f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(DamageStatics().BlockChanceDef, EvaluationParameters, TargetBlockChance); // Pass captured BlockChance attribute value into our local TargetBlockChance variable
	TargetBlockChance = FMath::Max<float>(TargetBlockChance, 0.f);

	// Halve the damage if there is a successful block
	const bool bBlocked = FMath::RandRange(0.f, 100.f) < TargetBlockChance;
	UAuraAbilitySystemLibrary::SetIsBlockedHit(EffectContextHandle, bBlocked);
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

	// Capture CritChance on Source, and determine if there was a successful Critical Hit
	float SourceCritChance = 0.f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(DamageStatics().CritChanceDef, EvaluationParameters, SourceCritChance);
	SourceCritChance = FMath::Max<float>(SourceCritChance, 0.f);

	// Capture CritResistance on Target
	float TargetCritRes = 0.f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(DamageStatics().CritResDef, EvaluationParameters, TargetCritRes);
	TargetCritRes = FMath::Max<float>(TargetCritRes, 0.f);

	FRealCurve* CritResCurve = CharacterClassInfo->DamageCalculationCoefficients->FindCurve(FName("CriticalHitResistance"), FString());
	const float CritResCoeff = CritResCurve->Eval(TargetCombatInterface->GetPlayerLevel());
	// Critical Hit Resistance reduces the attacker's Critical Hit Chance 
	const float EffectiveCritChance = SourceCritChance - TargetCritRes * CritResCoeff;

	// Capture CritDamage on Source
	float SourceCritDamage = 0.f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(DamageStatics().CritDamageDef, EvaluationParameters, SourceCritDamage);
	SourceCritDamage = FMath::Max<float>(SourceCritDamage, 0.f);

	// If there is a successful Critical Hit, double the base damage and add the Source's CritDamage
	const bool bCrit = FMath::RandRange(0.f, 100.f) < EffectiveCritChance;
	UAuraAbilitySystemLibrary::SetIsCriticalHit(EffectContextHandle, bCrit);
	if (bCrit) Damage = Damage * 2.f + SourceCritDamage;

	// Fill out an EvaluatedData struct for how we want to modify our target's attribute(s), then pass it into the execution output
	const FGameplayModifierEvaluatedData EvaluatedData(UAuraAttributeSet::GetIncomingDamageAttribute(), EGameplayModOp::Additive, Damage);
	OutExecutionOutput.AddOutputModifier(EvaluatedData);
}
