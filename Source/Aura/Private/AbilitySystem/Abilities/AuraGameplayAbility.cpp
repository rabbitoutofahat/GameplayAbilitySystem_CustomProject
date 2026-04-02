// Copyright Druid Mechanics


#include "AbilitySystem/Abilities/AuraGameplayAbility.h"
#include "AbilitySystem/AuraAttributeSet.h"
#include "Character/AuraCharacterBase.h"
#include "AbilitySystem/AuraAbilitySystemComponent.h"

void UAuraGameplayAbility::OnGiveAbility(const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilitySpec& Spec)
{
	Super::OnGiveAbility(ActorInfo, Spec);

	InitialiseChargeCountAttributes(Spec);
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

void UAuraGameplayAbility::InitialiseChargeCountAttributes(const FGameplayAbilitySpec& Spec)
{
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

	// Find the input slot associated with this ability, and grant the appropriate number of charges based on the ability to the charge attributes of that input slot.
    FGameplayTag MaxChargeTag = UAuraAbilitySystemComponent::GetMaxChargeTagFromSpec(Spec);
	FGameplayTag CurrentChargeTag = UAuraAbilitySystemComponent::GetCurrentChargeTagFromSpec(Spec);

    UAuraAttributeSet* AS = Cast<UAuraAttributeSet>(Cast<AAuraCharacterBase>(GetAvatarActorFromActorInfo())->GetAttributeSet());
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

    UAbilitySystemComponent* ASC = Cast<AAuraCharacterBase>(GetAvatarActorFromActorInfo())->GetAbilitySystemComponent();
    ASC->ApplyGameplayEffectToSelf(GEChargeChange, 1.0f, ASC->MakeEffectContext());
}
