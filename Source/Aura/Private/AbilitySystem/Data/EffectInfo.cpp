// Roland Frances


#include "AbilitySystem/Data/EffectInfo.h"
#include "Aura/AuraLogChannels.h"

FAuraEffectInfo UEffectInfo::FindEffectInfoForTag(const FGameplayTag& EffectTag, bool bLogNotFound) const
{
	for (const FAuraEffectInfo& Info : EffectInformation)
	{
		if (Info.EffectTag.MatchesTagExact(EffectTag))
		{
			return Info;
		}
	}

	if (bLogNotFound)
	{
		UE_LOG(LogAura, Error, TEXT("Can't find Info for Effect Tag [%s] on EffectInfo [%s]."), *EffectTag.ToString(), *GetNameSafe(this));
	}

	return FAuraEffectInfo();
}
