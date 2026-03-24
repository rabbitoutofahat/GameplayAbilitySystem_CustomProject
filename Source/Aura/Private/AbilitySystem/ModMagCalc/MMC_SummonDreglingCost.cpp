// Roland Frances


#include "AbilitySystem/ModMagCalc/MMC_SummonDreglingCost.h"
#include "AbilitySystem/Abilities/Vessel/SummonDregling.h"
#include "Character/PlayableClasses/Vessel.h"
#include "AbilitySystemComponent.h"
#include "AbilitySystem/AuraAbilitySystemComponent.h"
#include "AuraGameplayTags.h"

float UMMC_SummonDreglingCost::CalculateBaseMagnitude_Implementation(const FGameplayEffectSpec& Spec) const
{
    const USummonDregling* SummonDreglingAbility = Cast<USummonDregling>(Spec.GetContext().GetAbilityInstance_NotReplicated());
    if (!SummonDreglingAbility) return 0.f;

    AVessel* Vessel = Cast<AVessel>(SummonDreglingAbility->GetAvatarActorFromActorInfo());
    if (Vessel)
    {
        // If Portent of Destruction (upgrade for Summon Dregling) procs, Summon Dregling can be used for free regardless of current amount of souls so we need to set cost to 0 
        UAuraAbilitySystemComponent* VesselASC = Cast<UAuraAbilitySystemComponent>(Vessel->GetAbilitySystemComponent());
        if (VesselASC && VesselASC->HasMatchingGameplayTag(FAuraGameplayTags::Get().Buff_PortentOfDestruction)) return 0.f;
    }

    return SummonDreglingAbility->AbilityCost.GetValueAtLevel(SummonDreglingAbility->GetAbilityLevel());
}
