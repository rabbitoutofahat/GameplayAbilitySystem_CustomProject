// Roland Frances


#include "AbilitySystem/Data/AbilityRechargerInfo.h"
#include "Aura/AuraLogChannels.h"
#include "AbilitySystem/AuraAbilitySystemComponent.h"

FRechargerInfo UAbilityRechargerInfo::FindRechargerInfoForAbilitySpec(const FGameplayAbilitySpec& AbilitySpec, bool bLogNotFound) const
{
	for (const FRechargerInfo& Info : RechargerInformation)
	{
		if (Info.InputTag.MatchesTagExact(UAuraAbilitySystemComponent::GetInputTagFromSpec(AbilitySpec)))
		{
			return Info;
		}
	}

	if (bLogNotFound)
	{
		UE_LOG(LogAura, Error, TEXT("Can't find Info for Abiity [%s] on RechargerInfo [%s]."), *UAuraAbilitySystemComponent::GetAbilityTagFromSpec(AbilitySpec).ToString(), *GetNameSafe(this));
	}

	return FRechargerInfo();
}
