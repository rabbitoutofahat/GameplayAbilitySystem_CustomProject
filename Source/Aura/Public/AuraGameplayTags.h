// Copyright Druid Mechanics

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"

/*
 * AuraGameplayTags
 * 
 * Singleton (only one in the whole project) containing native Gameplay Tags
 */
struct FAuraGameplayTags
{
public:
	static const FAuraGameplayTags& Get() { return GameplayTags; }
	static void InitialiseNativeGameplayTags();

	FGameplayTag Attributes_Secondary_Armour;

protected:

private:
	static FAuraGameplayTags GameplayTags;
};
