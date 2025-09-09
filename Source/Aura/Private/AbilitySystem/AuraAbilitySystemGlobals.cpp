// Copyright Druid Mechanics


#include "AbilitySystem/AuraAbilitySystemGlobals.h"
#include "AuraAbilityTypes.h"

FGameplayEffectContext* UAuraAbilitySystemGlobals::AllocGameplayEffectContext() const
{
	// Whenever we make an Effect Context for a Gameplay Ability, make it our custom AuraGameplayEffectContext, which includes our own custom booleans such as Blocked and Critical Hits
	return new FAuraGameplayEffectContext();
}
