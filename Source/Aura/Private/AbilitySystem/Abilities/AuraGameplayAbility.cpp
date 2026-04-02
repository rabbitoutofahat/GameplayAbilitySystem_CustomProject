// Copyright Druid Mechanics


#include "AbilitySystem/Abilities/AuraGameplayAbility.h"
#include "AbilitySystem/AuraAttributeSet.h"
#include "Character/AuraCharacterBase.h"

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

void UAuraGameplayAbility::ApplyStackChangeGameplayEffect(const FGameplayAbilitySpec& Spec)
{
    UGameplayEffect* GEStackChange = NewObject<UGameplayEffect>(GetTransientPackage(), FName(TEXT("StackChange")));

    GEStackChange->DurationPolicy = EGameplayEffectDurationType::Instant;

    // Append two additional modifiers to the GE for max and current stacks, and set them to the max stacks value.
    const int32 Index = GEStackChange->Modifiers.Num();
    GEStackChange->Modifiers.SetNum(Index + 2);

    FGameplayModifierInfo& MaxStackInfo = GEStackChange->Modifiers[Index];
    MaxStackInfo.ModifierMagnitude = FScalableFloat(MaxStacks);
    MaxStackInfo.ModifierOp = EGameplayModOp::Override;

    FGameplayModifierInfo& CurrentStackInfo = GEStackChange->Modifiers[Index + 1];
    CurrentStackInfo.ModifierMagnitude = FScalableFloat(MaxStacks);
    CurrentStackInfo.ModifierOp = EGameplayModOp::Override;

    //MaxStackInfo.Attribute = UAbilitiesStackSet::GetMaxAttributeByInputID(Spec.InputID);
    //CurrentStackInfo.Attribute = UAbilitiesStackSet::GetCurrentAttributeByInputID(Spec.InputID);

    UAbilitySystemComponent* ASC = Cast<AAuraCharacterBase>(GetAvatarActorFromActorInfo())->GetAbilitySystemComponent();
    ASC->ApplyGameplayEffectToSelf(GEStackChange, 1.0f, ASC->MakeEffectContext());
}
