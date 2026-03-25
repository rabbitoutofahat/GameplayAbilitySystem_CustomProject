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
    /*
    * This function is called multiple times per cast of the ability, ultimately returning all possible float values, but the Summon Dregling Ability instance 
    * is only valid once per cast, which is when we actually want to determine the ability's cost and commit the ability.
    */
    if (!SummonDreglingAbility) return 0.f;

    /*
    * If Portent of Destruction (upgrade for Summon Dregling) procs, this particular ability instance of Summon Dregling is free to cast regardless of the 
    * player's current resources, otherwise it is the value specified by the ability's scalable float member variable. 
    */ 
    const FGameplayTagContainer& ActorTags = GetSourceActorTags(Spec);
    if (ActorTags.HasTagExact(FAuraGameplayTags::Get().Buff_PortentOfDestruction)) return 0.f;
    else return SummonDreglingAbility->AbilityCost.GetValueAtLevel(SummonDreglingAbility->GetAbilityLevel());
}
