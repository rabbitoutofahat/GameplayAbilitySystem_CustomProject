// Copyright Druid Mechanics

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemGlobals.h"
#include "AuraAbilitySystemGlobals.generated.h"

/**
 * 
 */
UCLASS()
class AURA_API UAuraAbilitySystemGlobals : public UAbilitySystemGlobals
{
	GENERATED_BODY()

	// Whenever we make an Effect Context for a Gameplay Ability, make it our custom AuraGameplayEffectContext, which includes our own custom booleans such as Blocked and Critical Hits
	virtual FGameplayEffectContext* AllocGameplayEffectContext() const override;
	
};
