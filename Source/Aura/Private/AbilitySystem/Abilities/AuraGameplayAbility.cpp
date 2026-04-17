// Copyright Druid Mechanics


#include "AbilitySystem/Abilities/AuraGameplayAbility.h"
#include "AbilitySystem/AuraAttributeSet.h"
#include "Character/AuraCharacter.h"
#include "AbilitySystem/AuraAbilitySystemComponent.h"
#include "AuraGameplayTags.h"
#include "AbilitySystem/Data/AbilityRechargerInfo.h"
#include "AbilitySystemBlueprintLibrary.h"

void UAuraGameplayAbility::OnGiveAbility(const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilitySpec& Spec)
{
	Super::OnGiveAbility(ActorInfo, Spec);

	InitialiseChargeCountAttributes(ActorInfo, Spec);
}

bool UAuraGameplayAbility::CommitAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, OUT FGameplayTagContainer* OptionalRelevantTags)
{
	if (MaxCharges < 2) return Super::CommitAbility(Handle, ActorInfo, ActivationInfo, OptionalRelevantTags); // Max Charge = 1 => No charge system for the given ability => Exit early and commit ability as normal

    AAuraCharacter* Character = Cast<AAuraCharacter>(ActorInfo->AvatarActor);
    UAuraAttributeSet* AS = Cast<UAuraAttributeSet>(Character->GetAttributeSet());
	UAuraAbilitySystemComponent* ASC = Cast<UAuraAbilitySystemComponent>(ActorInfo->AbilitySystemComponent.Get());

	// If the ability has charges, find the Ability Recharge GE associated with the ability's input and set its duration to match the ability's cooldown
    FGameplayAbilitySpec* Spec = ASC->FindAbilitySpecFromHandle(Handle);
    FRechargerInfo Info = AbilityRechargerInfo->FindRechargerInfoForAbilitySpec(*Spec, false);
	UGameplayEffect* RechargeGE = Cast<UGameplayEffect>(Info.RechargeEffect->GetDefaultObject());

    // Store ability's base cooldown so we can modify its duration without affecting future instances (Is this performant or should FGameplayModifierInfo be used?)
    UGameplayEffect* CooldownGE = CooldownGameplayEffectClass.GetDefaultObject();
	CooldownGE->DurationMagnitude.GetStaticMagnitudeIfPossible(1.f, AbilityCooldown);

    // If an ability has charges, set the time taken to recover a single ability charge to the cooldown of the ability 
    RechargeGE->DurationMagnitude = FScalableFloat(AbilityCooldown);
    FActiveGameplayEffectHandle RechargeHandle = ASC->ApplyGameplayEffectToSelf(RechargeGE, 1.f, ASC->MakeEffectContext()); // Consume a charge via Gameplay Effect
    ASC->AbilityRecharge.Broadcast(RechargeHandle, Info.InputTag); // Broadcast to Ability Charge Widget to change its radial progress bar

    // Determine the ability's cooldown depending on whether there are any more charges remaining
    if (Info.CurrentCharge.GetNumericValue(AS) > 0) CooldownGE->DurationMagnitude = FScalableFloat(ChargeCooldown);
    else // If no charges remaining, get current remaining duration on the RechargeGE to know when the next charge will be available
    {
        float RemainingChargeCooldown = UAbilitySystemBlueprintLibrary::GetActiveGameplayEffectRemainingDuration(Character, RechargeHandle);
        CooldownGE->DurationMagnitude = FScalableFloat(RemainingChargeCooldown);
    }

    bool bCommitAbility = Super::CommitAbility(Handle, ActorInfo, ActivationInfo, OptionalRelevantTags);
    CooldownGE->DurationMagnitude = FScalableFloat(AbilityCooldown); // Reset the cooldown GE's duration magnitude to the base cooldown before returning
    return bCommitAbility;
}

FString UAuraGameplayAbility::GetDescription(int32 Level)
{
    return FString::Printf(TEXT("<Default>%s, </><Level>%d</>"), L"Default Ability Name - LoremIpsum LoremIpsum LoremIpsum LoremIpsum LoremIpsum LoremIpsum LoremIpsum LoremIpsum LoremIpsum LoremIpsum LoremIpsum LoremIpsum LoremIpsum LoremIpsum ", Level);
}

FString UAuraGameplayAbility::GetNextLevelDescription(int32 Level)
{
    return FString::Printf(TEXT("<Default>Next Level: </><Level>%d</> \n<Default>Causes much more damage </>"), Level);
}

FString UAuraGameplayAbility::GetLockedDescription(int32 Level)
{
    return FString::Printf(TEXT("<Default>Spell locked until level: %d</>"), Level);
}

float UAuraGameplayAbility::GetManaCost(float InLevel)
{
    float ManaCost = 0.f;
    if (const UGameplayEffect* CostEffect = GetCostGameplayEffect())
    {
        for (FGameplayModifierInfo Mod : CostEffect->Modifiers)
        {
            if (Mod.Attribute == UAuraAttributeSet::GetManaAttribute())
            {
                Mod.ModifierMagnitude.GetStaticMagnitudeIfPossible(InLevel, ManaCost);
                break;
            }
        }
    }
    return ManaCost;
}

float UAuraGameplayAbility::GetCooldown(float InLevel)
{
	float Cooldown = 0.f;
    if (const UGameplayEffect* CooldownEffect = GetCooldownGameplayEffect()) 
    {
		CooldownEffect->DurationMagnitude.GetStaticMagnitudeIfPossible(InLevel, Cooldown);
	}
    return Cooldown;
}

void UAuraGameplayAbility::InitialiseChargeCountAttributes(const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilitySpec& Spec)
{
    // Find the input slot associated with this ability, and 
    FGameplayTag MaxChargeTag = UAuraAbilitySystemComponent::GetMaxChargeTagFromSpec(Spec);
    FGameplayTag CurrentChargeTag = UAuraAbilitySystemComponent::GetCurrentChargeTagFromSpec(Spec);

    // Charge Tags exist => offensive abilities belonging to player character => Add input charge attributes
    if (MaxChargeTag == FGameplayTag::EmptyTag || CurrentChargeTag == FGameplayTag::EmptyTag) return;

    UGameplayEffect* GEChargeChange = NewObject<UGameplayEffect>(GetTransientPackage(), FName(TEXT("ChargeChange")));
    GEChargeChange->DurationPolicy = EGameplayEffectDurationType::Instant;

    // Append two additional modifiers to the GE for max and current charges, and set them to the max charges value.
    const int32 Index = GEChargeChange->Modifiers.Num();
    GEChargeChange->Modifiers.SetNum(Index + 2);

    FGameplayModifierInfo& MaxChargeInfo = GEChargeChange->Modifiers[Index];
    MaxChargeInfo.ModifierMagnitude = FScalableFloat(MaxCharges);
    MaxChargeInfo.ModifierOp = EGameplayModOp::Override;

    FGameplayModifierInfo& CurrentChargeInfo = GEChargeChange->Modifiers[Index + 1];
    CurrentChargeInfo.ModifierMagnitude = FScalableFloat(MaxCharges);
    CurrentChargeInfo.ModifierOp = EGameplayModOp::Override;

    // Grant the appropriate number of charges based on the ability to the charge attributes of that input slot.
    AAuraCharacter* Character = Cast<AAuraCharacter>(ActorInfo->AvatarActor);
    UAuraAttributeSet* AS = Cast<UAuraAttributeSet>(Character->GetAttributeSet());
    for (const auto& Pair : AS->TagsToAttributes)
    {
        if (Pair.Key.MatchesTagExact(MaxChargeTag))
        {
            MaxChargeInfo.Attribute = Pair.Value();
        }
        else if (Pair.Key.MatchesTagExact(CurrentChargeTag))
        {
            CurrentChargeInfo.Attribute = Pair.Value();
		}
    }
   
    UAbilitySystemComponent* ASC = Character->GetAbilitySystemComponent();
    ASC->ApplyGameplayEffectToSelf(GEChargeChange, 1.0f, ASC->MakeEffectContext());
}
