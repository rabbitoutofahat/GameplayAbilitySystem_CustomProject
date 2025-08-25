// Copyright Druid Mechanics


#include "AbilitySystem/AuraAbilitySystemComponent.h"
#include "AuraGameplayTags.h"

void UAuraAbilitySystemComponent::AbilityActorInfoSet() 
{
	OnGameplayEffectAppliedDelegateToSelf.AddUObject(this, &UAuraAbilitySystemComponent::EffectApplied);
}

void UAuraAbilitySystemComponent::EffectApplied(UAbilitySystemComponent* AbilitySystemComponent, 
	const FGameplayEffectSpec& EffectSpec, FActiveGameplayEffectHandle ActiveEffectHandle) // When an effect is applied, call this function (we want to show stuff in the HUD)
{
	FGameplayTagContainer TagContainer;
	EffectSpec.GetAllAssetTags(TagContainer); // Fill the tag container with our GameplayEffectAssetTags (NOT GrantedTags)
	
	// Any class that binds to our EffectAssetTag's delegate will receive a tag container filled with the asset tags (in this case we want to bind our overlay widget controller)
	EffectAssetTags.Broadcast(TagContainer); 
}
